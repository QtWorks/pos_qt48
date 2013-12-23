#include "menu.h"
#include "item.h"
#include <algorithm>

MenuHandler::MenuHandler()
{
    std::unique_ptr<Item> item(new Item);
    item->push_property("id", -1);
    item->push_property("name", "");
    item->push_property("cat", 0);
    item->push_property("menu_id", 0);
    item->push_property("subitem", 0);
    item->push_property("recurring", 0);
    item->push_property("price", 0.0f);
    item->push_property("addon", 0);
    item->push_property("deleted", 0);
    item->push_property("refill", 0);

    blank_item = std::move(std::unique_ptr<MenuItem>(new MenuItem(std::move(item))) );
}

MenuItem* MenuHandler::find_item(const int& find_id)
{
    auto p = find_existing_item( find_id );
    if( !p ) {
        auto i = std::find_if(begin(deleted_items), end(deleted_items),
                         [&](const std::unique_ptr<MenuItem>& d) { return d->id == find_id; });

        return (i != end(deleted_items) ) ? (*i).get() : nullptr;
    }

    return p;
}

MenuItem* MenuHandler::find_existing_item(const int& find_id)
{
    auto i = std::find_if(begin(items), end(items),
                          [&](const std::unique_ptr<MenuItem>& e) { return e->id == find_id; });
    return (i != end(items)) ? (*i).get() : nullptr;
}

void MenuHandler::delete_item(const int& item_id)
{
    auto it = std::find_if( begin(items) , end(items),
                            [&](const std::unique_ptr<MenuItem>& e) { return e->id == item_id; });
    //item found remove it
    if( it != end(items) ) {
        if( (*it)->parent )
            (*it)->parent->delete_item( (*it)->id );

        items.erase( it );
    }
}

void MenuHandler::add_item(std::unique_ptr<MenuItem>&& item)
{
    std::unique_ptr<MenuItem> ptr = std::move(item);

    if( ptr->deleted ) {
        deleted_items.push_back( std::move(ptr) );
    }
    else {
        //Category. Bind existing items to it.
        if( MenuItem::iscat(ptr) ) {
            std::for_each(begin(items), end(items),
                          [&](const std::unique_ptr<MenuItem>& e) {
                            if( e->cat == ptr->id )
                                ptr->add_item( e.get() );
                          });
        }
        else { //Regular item. Bind it to its parent
            auto p = std::find_if(begin(items), end(items),
                                  [&](std::unique_ptr<MenuItem>& e) { return e->id == ptr->cat; });
            if( p != end(items) )
                (*p)->add_item( ptr.get() );
        }

        items.push_back( std::move(ptr) );
    }
}
