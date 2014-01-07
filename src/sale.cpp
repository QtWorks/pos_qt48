#include "sale.h"
#include "menu.h"
#include "table.h"
#include "user.h"
#include "ordereditem.h"

Sale::Sale(std::unique_ptr<Item>&& data) :
    payload(std::move(data)),
    id(payload->property<int>("id")),
    total(payload->property<double>("amt")),
    tendered(payload->property<double>("tender")),
    change(payload->property<double>("change")),
    tax(payload->property<double>("tax")),
    cc_paid(payload->property<double>("cc_paid")),
    tip_percent(payload->property<int>("tip")),
    open(payload->property<int>("open")),
    item_count(payload->property<int>("item_count")),
    table_number(payload->property<int>("table")),
    user_id(payload->property<int>("emp")),
    session(payload->property<int>("session")),
    guests(payload->property<int>("guests")),
    start_t(payload->property<std::string>("start_t"))
{
    tax = (sub_total * tax_percent);
}

Sale::~Sale()
{
    items.clear();
}

void Sale::pay_cash(const double& amount)
{
    tendered += amount;
    if( (amount - owed) > 0.0099f ) {
        change += amount - owed;
    }
    owed = ( (owed - amount) > 0.0099f ) ? (owed - amount) : 0.0f;
}

void Sale::pay_credit(const double& amount)
{
    if( (owed - amount) > 0.0099f ) {
        owed -= amount;
        cc_paid += amount;
        return;
    }

    cc_paid += owed;
    owed = 0;
}

int Sale::add_abstract(const std::unique_ptr<OrderedItem>& item)
{
    if( item->comment ) { //Comment
        /*if( item->name.substr(1, 4) != "Note" )
            item->name = std::string( " Note: " ) + item->name;*/
        active_order->addItem( item.get() );
        return 0;
    }
    else if( item->total ) { //Modifier
        active_order->addItem( item.get() );
        return 0;
    }

    //Fail
    return 1;
}

int Sale::add_concrete(const std::unique_ptr<OrderedItem>& item)
{
    bool hasitem = (active_order && active_order->hasItem(item->menu_id) > -1);

    if ( !item->comment && item->subitem && active_order && !hasitem &&
         !active_order->final ) {
        //if subitem attach it to a parent.

        // Corrupted data. Do nothing. TODO: add catch/throw/warning
        if( !item->definition || !item->definition->parent )
            return 1;

        // Not a valid subitem for active order. Id mis-match, do nothing.
        if( item->definition->parent->id != active_order->definition->id ) {
            return 1;
        }

        //attach subitem to parent
        active_order->addItem(item.get());
        return 0;
    }
    else if ( item->subitem && !active_order ) {
        //subitem with no parent, do nothing.
        return 1;
    }
    //A sub item that has a finalized parent is a REFILL
    else if( active_order && item->subitem && item->definition->parent->id == active_order->definition->id )
    {
        if( !hasitem ) { //Selected item does not exist in the active order. Upgrade
            //If the refill is for an item that cost EXTRA, it cannot be refilled
            //without being previously ordered

            //Convert a refill to an UPGRADE
            item->name = std::string("Upgrade: ") + item->name;
            item->final = false;
            active_order->addItem( item.get() );

        } else {
            // Item exists in the active order. Refill
            item->total = 0.0f;
            item->price = 0.0f;
            item->subitem = false;
            item->parent = nullptr;
            item->name = std::string("Refill: ") + item->name;
        }

        return 0;
    }
    //Subitem and parent mismatch
    else if( item->subitem && active_order && active_order->definition->id != item->definition->parent->id ) {
        //This does not supose to happen. The menu should only show subitems available for the active item!
        return 1;
    }

    return 0;
}

OrderedItem* Sale::addItem(std::unique_ptr<OrderedItem>&& item_ptr, bool iscounted, int qty)
{
    std::unique_ptr<OrderedItem> item = std::move(item_ptr);
    item->quantity = qty;
    item->total = item->total * qty;
    item->id = items.size();
    item->sale_id = id;

    if( item->abstract() ) {
        if( add_abstract( item ) ) return nullptr;
    }
    else {
        if( add_concrete( item ) ) return nullptr;
    }

    if ( iscounted ) {
        sub_total += item->total;
        tax = sub_total * tax_percent;
        total = sub_total + tax;
        owed = total - cc_paid - tendered;
    }

    //Add item to unsaved buffer in-case the changes need to be un-done
    unsaved.push_back( item.get() );
    items_before_save++;

    //NOTE: Anytime we add an Order to the sale(NOT subitem or refill type),
    //we need to set it to active. This helps greatly in a lot of logic
    //Removing this breaks a lot of things
    auto ret = item.get();

    if(!item->subitem) {
        if( !item->refill )
            active_order = item.get();
        items.push_back(std::move(item));
    }
    else { //Attach subitem to sale
        //Insert isntead of push back so that items are neatly organized by default
        auto before = begin(items) + (active_order->id + active_order->sub_count );
        items.insert( before, std::move(item) );
    }

    item_count+= qty;
    refresh_ids();

    return ret;
}

OrderedItem* Sale::addItem(const MenuItem* menu_item, bool iscounted, int qty)
{
    std::unique_ptr<OrderedItem> order(new OrderedItem(menu_item, nullptr));
    return addItem(std::move(order), iscounted, qty);
}

OrderedItem* Sale::add_item_qty(const MenuItem* menu_item, const int& qty, bool iscounted )
{
    std::unique_ptr<OrderedItem> order(new OrderedItem(menu_item, nullptr));
    order->quantity = qty;

    return addItem(std::move(order), iscounted);
}

void Sale::add_comment(const std::string& comment, const std::unique_ptr<MenuItem>& blank) {
    std::unique_ptr<OrderedItem> ci( new OrderedItem(blank.get(), nullptr));
    ci->subitem = true;
    ci->comment = true;
    ci->name = comment;
    addItem(std::move(ci), false);
}

void Sale::add_comment(std::string text)
{
    std::unique_ptr<OrderedItem> comment(new OrderedItem(text));
    comment->comment = true;
    addItem(std::move(comment), false);
}

void Sale::add_price_modifier(const double& amount)
{
    if( !active_order ) return;

    std::unique_ptr<Item> item(new Item);
    double amt = amount - active_order->total;
    item->push_property( "amount", amt );
    if( amt < 0 )
        item->push_property("name", "Discount" );
    else
        item->push_property("name", "Charge" );

    std::unique_ptr<OrderedItem> order(new OrderedItem(nullptr, std::move(item)));

    order->subitem = true;

    addItem(std::move(order), true);
}


void Sale::delete_order(const int& item_id)
{
    auto it = std::find_if(std::begin(items), std::end(items),
                           [&]( std::unique_ptr<OrderedItem>& od) { return od->id == item_id; });
    if( it == std::end(items) ) return;

    OrderedItem* item = (*it).get();
    if(item->parent) {
        //If a subitem is being removed, set active_order to its parent
        active_order = item->parent;
        item->parent->delItem(item->sub_id);
    }

    if ( item->sub_count ) {
        for (auto sub : item->subitems) {
            sub->parent = nullptr;
            delete_order(sub->id);
        }
    }
    if (active_order == item) {
        active_order = nullptr;
    }

    //Push the pointer to a list of items removed
    removed.push_back(item->unique_id);
    remove_count++;

    item->clear();
    sub_total -= item->total;
    tax = sub_total * tax_percent;
    total = sub_total + tax;
    owed = total - cc_paid - tendered;
    item_count -= item->quantity;

    auto un_it = std::find_if( std::begin(unsaved), std::end(unsaved),
                               [&]( OrderedItem* cmp ) { return cmp->id == item_id; });
    if( un_it != std::end(unsaved) ) { //Item is not finalized yet, no void needed;
        unsaved.erase( un_it );
        items_before_save--;
    }
    else {
        un_it = std::find_if( std::begin(voids), std::end(voids),
                          [&] ( OrderedItem* cmpd )  { return cmpd->id == item_id; });
        if( un_it != std::end(voids) ) {
            voids.erase( un_it );
            void_count--;
        }
    }

    items.erase(it);
}

void Sale::void_item(const int& itemid, const int& user_id)
{
    auto vit = std::find_if(std::begin(items), std::end(items),
                            [&](const std::unique_ptr<OrderedItem>& item) { return item->id == itemid; });
    if( vit != std::end(items) ) {
        (*vit)->void_flag = user_id;
        for( auto& sub : (*vit)->subitems ) {
            void_item( sub->id, user_id );
        }
        voids.push_back( (*vit).get() );
        void_count++;

    }
}

void Sale::empty()
{
    while( item_count ) {
       delete_order( item_count - 1 );
    }
}

void Sale::reset_removed()
{
    removed.clear();
    remove_count = 0;
}

void Sale::finalize_voids()
{
    if( !voids.size() ) {
        void_count = 0;
        refresh_ids();
        return;
    }

    auto v = voids[ voids.size() - 1 ];
    if( v->parent && !v->parent->void_flag )
        delete_order( v->id );
    else if( !v->subitem )
        delete_order( v->id );

    finalize_voids();
}

void Sale::finalize_unsaved()
{
    for(const auto& make_final : unsaved) {
        if( !make_final ) continue; //this really shouldn't happen. Being careful
        make_final->final = true;
    }

    items_before_save = 0;
    unsaved.clear();
}

void Sale::discard_unsaved()
{
    if( !unsaved.size() ) {
        items_before_save = 0;
        return;
    }

    auto trash = unsaved[unsaved.size()-1];
    delete_order( trash->id );
    discard_unsaved();
}

void Sale::discard_voids()
{
    if( !voids.size() ) {
        void_count = 0;
        return;
    }

    auto unvoid = voids[voids.size() - 1];
    unvoid->void_flag = false;
    auto vit = std::find_if(std::begin(voids), std::end(voids),
                            [&]( OrderedItem* v ) { return v == unvoid; });
    if( vit != std::end(voids) )
        voids.erase( vit );

    discard_voids();
}

void Sale::discard_changes()
{
    discard_unsaved();
    discard_voids();
}

void Sale::finalize_changes()
{
    finalize_unsaved();
    finalize_voids();
}

void Sale::delItem(const int& item_id)
{
    delete_order(item_id);
    refresh_ids();
}

// Fix the id's after an add/delete
void Sale::refresh_ids()
{
    int display = 1;
    int id      = 0;
    for( const auto& item : items ) {
        item->id = id;
        if( item->subitem ) {
            item->display_id = item->sub_id;
        }
        else {
            item->display_id = display;
            display++;
        }
        id++;
    }
}

void Sale::selectItem(const int& item_id)
{
    if( item_id < 0 ) return;

    if((unsigned)item_id > items.size()) {
        return;
    }

   active_order = items[item_id].get();
}

//The data gets 'flattened' into a single Item with properties
std::unique_ptr<Item> Sale::flatten(void) const
{
    Item* item = new Item;

    for( const auto& prop : payload->properties ) {
        item->copy_property( prop );
    }

    item->property<int>("id") = id;
    item->property<int>("session") = session;
    item->property<double>("cc_paid") = cc_paid;
    item->property<double>("change") = change;
    item->property<int>("guests") = guests;
    item->property<int>("tip") = tip_percent;

    return std::unique_ptr<Item>(item);
}


bool Sale::hasitem(const int& menuid) const
{
    for(const auto& item : items ) {
        if( item->menu_id == menuid ) return true;
    }

    return false;
}

OrderedItem* Sale::get_order_ptr(const int& orderid) const
{
    auto it = std::find_if( std::begin(items), std::end(items),
                              [&]( const std::unique_ptr<OrderedItem>& od) { return (od->id == orderid); });

    if( it != std::end(items) )
        return nullptr;

    return (*it).get();
}
