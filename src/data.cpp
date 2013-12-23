#include "data.h"
#include "server/server.h"
#include <algorithm>

DataHandler::DataHandler() :
    active_table(),
    active_user(0),
    mScreenHeight(-1),
    mScreenWidth(-1),
    dserver(&Server::Instance()),
    mmenu(),
    printer(Printer::Instance())
{
    max_saleid = 0;

    //Menu Data
    reset_menu();
    /*for(auto&& item_def : res->items ) {
        std::unique_ptr<MenuItem> item(new MenuItem(std::move(item_def)));
        mmenu.addItem(std::move(item));
    }*/

    //Table data
    auto res = dserver->command(new ReadTableAllCmd);
    if ( res->size() ) {

        for(auto&& item_def : res->items ) {
            Tables.push_back(std::unique_ptr<Table>(new Table(std::move(item_def))));
        }
    }

    //Users/Employees
    res = dserver->command(new ReadEmployeeCmd);
    for( auto&& item_def : res->items ) {
        auto user = add_user( std::move(item_def) );
        fill_user_hours( user );
        if( user->session )
            update_user_sale_totals( user );
    }

    //Get all currently OPEN sales
    res = dserver->command(new ReadOpenSalesAllCmd);

    for( auto& item_def : res->items ) {
        std::unique_ptr<Sale> sale(new Sale(std::move(item_def)));
        add_sale( std::move(sale) );
    }

    //Lets populate open sales with order items
    for(auto& sale_item : Sales) {

        if( !sale_item->item_count) continue;

        auto sale_data = dserver->command(new ReadSaleDataCmd(sale_item->id, sale_item->item_count));

        //NOTE: its important to set item count to zero
        //as a sale will automatically update its item count upon added item/subitem
        //we already fetched the data from database with fetched item_count
        //so it will double if not zeroed
        sale_item->item_count = 0;

        for( auto& order : sale_data->items ) {

            auto menu_item = find_existing_menu_item( order->property<int>("menu_id") );

            std::unique_ptr<OrderedItem> oi(new OrderedItem(menu_item, std::move(order) ));
            sale_item->addItem(std::move(oi), true);
        }

        //Finalize all the changes
        sale_item->finalize_unsaved();
    }
}

const int& DataHandler::GetScreenHeight()   const { return mScreenHeight; }
const int& DataHandler::GetScreenWidth()    const { return mScreenWidth; }
struct tm* DataHandler::now()               const { return localtime( &now_t ); }
void DataHandler::SetScreenWidth(int pWidth)      { mScreenWidth = (pWidth < 0) ? mScreenWidth : pWidth; }
void DataHandler::SetScreenHeight(int pHeight)    { mScreenHeight = (pHeight < 0 ) ? mScreenHeight : pHeight; }

//Logs
void DataHandler::log_order_new(OrderedItem* order)
{
    auto sale = find_sale(order->sale_id);
    dserver->command( new WriteLogCmd(LogType::NewOrder, sale->user_id,
                      "ITEM S# " + std::to_string(sale->id) + " O#" + std::to_string(order->unique_id) + "," + order->name));
}

void DataHandler::log_order_void(OrderedItem* order)
{
    dserver->command( new WriteLogCmd(LogType::Void, order->void_flag,
                      "VOID S# " + std::to_string(order->sale_id) + " O#" + std::to_string(order->unique_id) + "," + order->name));
}

void DataHandler::log_order_transfer(OrderedItem* order, const int& from_id, const int& to_id, const int& user_id)
{
    dserver->command( new WriteLogCmd(LogType::TransferOrder, user_id,
                      "SPLIT S#" + std::to_string(from_id) + " -> S#" + std::to_string(to_id) + " O#" + std::to_string(order->unique_id)));
}
