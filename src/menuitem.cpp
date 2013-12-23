#include "menuitem.h"
#include "item.h"
#include<algorithm>

MenuItem::MenuItem(std::unique_ptr<Item>&& def) :
    payload(std::move(def)),
    id(payload->property<int>("id")),
    name(payload->property<std::string>("name")),
    cat(payload->property<int>("cat")),
    menu_id(payload->property<int>("menu_id")),
    subitem(payload->property<int>("subitem")),
    recurring(payload->property<int>("recurring")),
    price(payload->property<double>("price")),
    addon(payload->property<int>("addon")),
    deleted(payload->property<int>("deleted")),
    refill(payload->property<int>("refill"))
{
}

MenuItem::MenuItem(MenuItem&& moveit) :
    MenuItem(std::move(moveit.payload))
{
    subs = std::move(moveit.subs);
    sub_count = std::move(moveit.sub_count);
}

void MenuItem::add_item(MenuItem* sub)
{
    sub_count++;
    subs.push_back(sub);
    sub->parent = this;

    if(price < 0.0001f && (cat > -1)) {
       incomplete = true;
    }
}

void MenuItem::delete_item(const int& item_id)
{
    auto it = std::find_if(begin(subs), end(subs),
                           [&](const MenuItem* p) { return p->id == item_id; });
    if( it != end(subs) ) { //Item found, erase it
        (*it)->parent = nullptr;
        sub_count--;
        subs.erase( it );
    }
}

std::unique_ptr<Item> MenuItem::get_data()
{
    Item* item = new Item;
    for (const auto& property : payload->properties ) {
        item->copy_property( property );
    }

    return std::unique_ptr<Item>(item);
}
