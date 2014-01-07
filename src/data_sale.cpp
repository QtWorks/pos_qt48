#include "data.h"
#include "server/server.h"
#include "connect.h"
#include <algorithm>
#include <iterator>

int DataHandler::new_sale(int table_id) {
    std::unique_ptr<Sale> sale = Factory::EmptySale();
    sale->table_number = table_id;
    sale->user_id = active_user->id;
    sale->open = true;
    attach_sale( sale.get() );

    int id = sale->id = max_saleid;
    std::vector<std::unique_ptr<Item>> item_vector;
    auto result = dserver->command(new WriteSaleCmd(sale->flatten(), true) );
    if( result->size() ) {
        id = result->items[0]->property<int>("id");
        sale->id = id;
        sale->start_t = result->items[0]->property<std::string>("start_t");
    }
    Sales.push_back( std::move( sale ) );

    return id;
}

Sale* DataHandler::find_sale(int sale_id)
{
    for(const auto& sale : Sales) {
        if(sale->id == sale_id) {
            return sale.get();
        }
    }

    return nullptr;
}

Sale* DataHandler::set_active_sale(const int& sale_id) {
    active_sale = find_sale(sale_id);
    return active_sale;
}

int DataHandler::get_active_sale_id(void) const { return (active_sale) ? active_sale->id : -1; }
Sale* DataHandler::get_active_sale() const { return active_sale; }

int DataHandler::add_sale(std::unique_ptr<Sale>&& sale_ptr)
{
    std::unique_ptr<Sale> sale = std::move(sale_ptr);
    if( active_user && sale->user_id != active_user->id ) {
        sale->user_id = active_user->id;
    }

    attach_sale( sale.get() );

    int id = sale->id;

    Sales.push_back( std::move( sale ) );


    return id;
}

void DataHandler::attach_sale(Sale* sale)
{
    auto user = find_user( sale->user_id );
    sale->user = user;
    if( user ) {
        user->attach_sale( sale );
    }
    auto table = find_table( sale->table_number );
    sale->table = table;
    if( table ) {
        table->attach_sale( sale );
    }
}

void DataHandler::detach_sale(Sale* sale)
{
    if( sale->table )
        sale->table->detach_sale( sale );
    if( sale->user )
        sale->user->detach_sale( sale );
}

void DataHandler::remove_sale(Sale* sale_ptr)
{
    auto it = std::find_if( std::begin(Sales), std::end(Sales),
                            [&]( std::unique_ptr<Sale>& order ) { return order->id == sale_ptr->id; } );
    detach_sale( sale_ptr );
    (*it).reset( nullptr );
    Sales.erase( it );
}

void DataHandler::erase_sale(int sale_id)
{
    dserver->command( new DeleteSaleCmd(sale_id) );
}

void DataHandler::write_sale(Sale* sale, bool isnew )
{
    if( sale->void_count ) {
        auto& voids = sale->get_voids();

        std::for_each( std::begin(voids), std::end(voids),
                       [&]( OrderedItem* vd ){
                        log_order_void( vd );
                        dserver->command( new DeleteOrderCmd( vd->unique_id ) );
        }) ;

        sale->finalize_voids();
    }
    dserver->command( new WriteSaleCmd(sale->flatten(), isnew) );
    auto items = sale->get_unsaved_items();

    std::for_each( std::begin(items), std::end(items),
           [&]( OrderedItem* order ){

            auto result = dserver->command( new WriteOrderCmd(sale->start_t, std::unique_ptr<Item>( order->to_item() )) );
            order->unique_id = result->item(0)->property<int>("id");
            log_order_new( order );
    }) ;
}

void DataHandler::write_new_sale(Sale* sale)
{
    auto& data = sale->get_items();

    for( auto& order : data ) {
        auto result = dserver->command( new WriteOrderCmd(sale->start_t, std::unique_ptr<Item>( order->to_item() )) );
        order->unique_id = result->item(0)->property<int>("id");
    }

    dserver->command( new WriteSaleCmd(sale->flatten(), true) );
}

void DataHandler::save_sale(const int& sale_id, bool isnew)
{
    Sale* sale = find_sale( sale_id );
    if( sale ) {
        if( sale->items_before_save || sale->void_count )
            printer.print_order( sale );

        if( isnew )
            write_new_sale( sale );
        else
            write_sale( sale, isnew );

        //Update user data
        update_user_sale_totals(sale->user);

        sale->finalize_unsaved();
    }
}

bool DataHandler::pay_sale(Sale* sale, const double& amount, const bool& print) {

    save_user_data( sale->user );
    dserver->command(new AddPaymentCmd(sale->id, amount, "CASH"));
    sale->pay_cash( amount );

    if ( print ) printer.print( sale );

    if( !sale->owed ) {
        sale->open = false;
        save_sale(sale->id);

        detach_sale( sale );
        set_active_table( nullptr );

        remove_sale( sale );

        return true;
    }

    save_sale(sale->id);

    //Error
    return false;
}

bool DataHandler::pay_sale(const int& sale_id, const double& amount, const bool& print)
{
   Sale* sale = find_sale( sale_id );
   if( sale ) {
       return pay_sale( sale, amount, print);
   }

   return false;
}

bool DataHandler::pay_sale_credit(Sale* sale, const double& amount, const bool& print)
{
    save_user_data( sale->user );

    dserver->command(new AddPaymentCmd(sale->id, amount, "CREDIT"));
    sale->pay_credit( amount );

    if( print ) printer.print( sale );

    if( !sale->owed ) {
        sale->open = false;
        save_sale( sale->id );
        detach_sale( sale );
        set_active_table( nullptr );

        remove_sale( sale );

        return true;
    }

    save_sale( sale->id );

    return false;
}

void DataHandler::close_sale(const int& sale_id)
{
    Sale* sale = find_sale( sale_id );
    if( sale ) {
        sale->open = false;
        save_sale( sale->id );
        detach_sale( sale );
        set_active_table( nullptr );

        remove_sale( sale );
    }
}


void DataHandler::sale_transfer_item(Sale* to, Sale* from, OrderedItem* order)
{
    std::vector<std::unique_ptr<OrderedItem>> transfers;
    //Clone & move the selected order + suborders
    std::unique_ptr<OrderedItem> clone( new OrderedItem( order->definition, std::move(order->data) ) );
    //NOTE: the commands get fired off here and unique id's get explicitly copied
    //FIXME: above line can mangle the  unique_id
    clone->unique_id = order->unique_id;
    transfers.push_back( std::move(clone) );

    for( const auto& sub : order->subitems ) {
        std::unique_ptr<OrderedItem> sub_clone( new OrderedItem( sub->definition, std::move(sub->data) ) );
        //NOTE: the commands get fired off here and unique id's get explicitly copied
        //FIXME: above line can mangle the  unique_id
       // dserver->command( new MoveOrderCmd( sub->unique_id, to->id ) );
        sub_clone->unique_id = sub->unique_id;
        transfers.push_back( std::move(sub_clone) );
    }
    //Delete the order
    from->delItem( order->id );
    //Order is now null
    order = nullptr;

    //At this point the order(s) has(ve) not went anywhere.
    //The data is missing from the system, but its present in the DB still.
    for( auto& mo : transfers ) {
        //Transfer it here to the sale
        log_order_transfer( mo.get(), from->id, to->id, to->user_id );
        auto mov = to->addItem( std::move(mo), true );
        dserver->command( new MoveOrderCmd(mov->unique_id, to->id) );
        if( mov->parent )
            dserver->command( new WriteOrderModCmd(mov->unique_id, mov->parent->unique_id) );
    }
    //Save everything to the DB
    //save_sale( to->id, false );
    from->finalize_changes();
    to->finalize_changes();
    dserver->command( new WriteSaleCmd( to->flatten(), false ) );
    dserver->command( new WriteSaleCmd( from->flatten(), false) );

    //Totals should not change... unless the user changed!
    //update_user_sale_totals(from->user);
    if( to->user_id != from->user_id ) {
        update_user_sale_totals( from->user );
        update_user_sale_totals( to->user );
    }
}

void DataHandler::save_ordereditem(OrderedItem* order)
{
}

void DataHandler::void_ordereditem(Sale* sale, const int& orderid)
{
    auto order = sale->get_order_ptr( orderid );
    if( order ) { //Check it exists
        dserver->command( new DeleteOrderCmd( order->unique_id ) );
    }
}

void DataHandler::transfer_sale_ownership(const int& tf_sale_id, const int& tf_user_id)
{
    auto tf_user = find_user(tf_user_id);
    auto tf_sale = find_sale(tf_sale_id);
    if( tf_user->session > 0 ) {
        if( tf_sale->user ) {
            tf_sale->user->detach_sale( tf_sale );
        }
        tf_user->attach_sale( tf_sale );
    }
}
