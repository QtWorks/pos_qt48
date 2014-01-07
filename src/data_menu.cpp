#include "data.h"
#include "server/server.h"
#include "connect.h"

void DataHandler::reset_menu()
{
    auto res = dserver->command(new ReadMenuCmd);
    mmenu.clear();

    for(auto&& item_def : res->items )
    {
        std::unique_ptr<MenuItem> item(new MenuItem(std::move(item_def)));
        mmenu.add_item( std::move(item) );
    }
}


int DataHandler::create_new_menuitem(const int& parent_id, const std::string& name)
{
    //anything with a negative parent_id is parentless and is a category
    auto result = dserver->command( new WriteNewMenuItemCmd(parent_id, name) );
    if( result ){
        result = dserver->command( new ReadMenuCmd( result->items[0]->property<int>("id") ) );

        if( result ) {
            std::unique_ptr<MenuItem> item( new MenuItem(std::move(result->items[0])) );
            int id = item->id;
            auto parent = find_menu_item( parent_id );
            if( parent && !MenuItem::iscat(parent) )
                item->subitem = true;

            mmenu.add_item( std::move(item) );
            return id;
        }
    }
    return -1; //Error
}

void DataHandler::save_menuitem(const int& item_id)
{
    auto item = find_menu_item( item_id );
    if( item )
        dserver->command( new WriteMenuItemCmd( item->get_data() ) );
}

void DataHandler::delete_menuitem(const int& item_id)
{
    auto item = find_menu_item(item_id);
    item->deleted = 1;

    dserver->command( new MarkMenuItemDeletedCmd(item_id) );

    save_menuitem( item_id );
    mmenu.delete_item( item_id );
}

bool DataHandler::menuitem_isused(const int& menuid)
{
    auto item = find_menu_item( menuid );
    if( item ) {
        for( const auto& sale : Sales ) {
            if( sale->hasitem(menuid) ) return true;
        }
    }
    return false;
}
