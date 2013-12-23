#include <QObject>
#include "menuinfo.h"
#include "menulistmodel.h"
#include "data.h"

MenuInfo::MenuInfo(QObject* parent) :
    QObject(parent),
    data(Data::Instance()),
    category_model(new ListModel(new MenuListItem())),
    item_model(new ListModel(new MenuListItem())),
    sub_model(new ListModel(new MenuListItem()))
{
    for(const auto& item : data.getMenuData() ) {
        if(!item->deleted && MenuItem::isCategory(item)) {
            category_model->appendRow(new MenuListItem(item.get()));
        }
    }
}

void MenuInfo::update_cat_list()
{
    category_model->clear();
    for(const auto& item : data.getMenuData() ) {
        if( !item->deleted && MenuItem::isCategory( item ) ) {
            category_model->appendRow( new MenuListItem(item.get()) );
        }
    }
}

bool MenuInfo::iscat(const int& item_id)
{
    return MenuItem::iscat( data.find_menu_item(item_id) );
}

bool MenuInfo::hassubs(const int& item_id)
{
    return ( data.find_menu_item(item_id)->sub_count > 0 );
}

int MenuInfo::create_new_item(const int& parent_id, const QString& name)
{
    int id = data.create_new_menuitem( parent_id, name.toStdString() );

    if( parent_id > -1 ) {
        emit itemModelChanged();
        auto sub = data.find_menu_item( id );
        sub->subitem = 1;
        save_menuitem( id );

        select( parent_id );
    }
    else {
        update_cat_list();
    }

    return id;
}

void MenuInfo::save_menuitem(const int& menuitem_id)
{
    data.save_menuitem( menuitem_id );
}

void MenuInfo::delete_menuitem(const int& menuitem_id)
{
    if( current_item->id == menuitem_id ) {
        current_item = nullptr;
        selected_id = -1;
    }

    auto parent = data.find_menu_item( menuitem_id )->parent;

    data.delete_menuitem( menuitem_id );

    if( parent ) {
        select( parent->id );
    }

    update_cat_list();
}

void MenuInfo::change_itemname(const QString& newname)
{
    current_item->name = newname.toStdString();
    data.save_menuitem( current_item->id );
    update_cat_list();

    emit item_nameChanged();
}

void MenuInfo::set_item_price(const double& price)
{
    current_item->price = price;
    data.save_menuitem( current_item->id );

    emit item_priceChanged();
}

void MenuInfo::navigate(const int& item_id )
{
    auto item = data.find_existing_menu_item(item_id);
    if( !item ) return;

    if( MenuItem::isCategory(item) ) item_model->clear();
    if ( !item->sub_count ) {
        /*if( item->parent ) {
            navigate( item->parent->id );
        }*/
        return;
    }

    item_model->clear();

    for ( auto sub : item->subitems() ) {
        if ( !sub->deleted && !MenuItem::isCategory(sub) && (sub->cat == item->id) ) {
            item_model->appendRow(new MenuListItem(sub));
        }
    }

}

void MenuInfo::append_to_sale(const int& item_id)
{
    auto sale = data.get_active_sale();

    if( sale ) {
        sale->addItem( data.find_existing_menu_item( item_id ), true);
    }
}

void MenuInfo::add_comment(const QString& comment)
{
    auto sale = data.get_active_sale();
    if( sale ) {
        sale->add_comment( comment.toStdString() );
        /*std::unique_ptr<Item> item(new Item);

        item->push_property("name", comment.toStdString());
        std::unique_ptr<OrderedItem> order(new OrderedItem(nullptr, std::move(item)));

        sale->addItem(std::move(order), false);*/
    }
}

//Return true if item is being added to ticket
/*bool MenuInfo::select_menuitem_ptr(const std::unique_ptr<MenuItem>& item)
{
    selected_id = item->id;
    current_item = item.get();

    navigate(item);

    if(MenuItem::isCategory(item)) {
        selected_id = item->id;
        return false;
    }


    if ( item->price || item->incomplete || item->subitem ) {
        append_to_sale();
        return true;
    }

    return false;
}*/

bool MenuInfo::select(const int& item_id)
{
    current_item = data.find_menu_item( item_id );
    selected_id = ((current_item) ? current_item->id : -1);

    if( !current_item ) return false;

    emit selectedChanged();
    emit item_priceChanged();
    emit item_nameChanged();

    navigate( item_id );

    if( MenuItem::iscat(current_item) ) return false;

    if( current_item->price || current_item->incomplete || current_item->subitem ) {
        append_to_sale( item_id );
        return true;
    }

    return false;
}

int         MenuInfo::catCount()    { return 0;                 }
int         MenuInfo::selected()    { return selected_id;       }
double      MenuInfo::item_price()  { return (current_item) ? current_item->price : 0.0f; }
QString     MenuInfo::item_name()   { return (current_item) ? QString( current_item->name.c_str() ) : QString(""); }
ListModel*  MenuInfo::catModel()    { return category_model;    }
ListModel*  MenuInfo::itemModel()   { return item_model;        }
ListModel*  MenuInfo::subModel()    { return sub_model;         }
