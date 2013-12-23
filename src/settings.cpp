#include "settings.h"
#include "menulistmodel.h"

Settings::Settings(QObject* parent)
    :   QObject(parent),
        data( Data::Instance() ),
        menu_list( std::unique_ptr<ListModel>(new ListModel(new MenuListItem())) ),
        user_list( std::unique_ptr<ListModel>(new ListModel(new UserSettingsItem())) )
{
    reset_menu();
    reset_user_list();
}

void Settings::reset_menu()
{
    menu_list->clear();
    for( const auto& item : data.getMenuData() ) {
        if(MenuItem::isCategory(item) && !item->deleted)
            menu_list->appendRow(new MenuListItem(item.get()));
    }

    menu_list->appendRow(new MenuListItem("__ADD_ITEM__"));
}

int Settings::new_item(const int& parent_id, const QString& name)
{
    int id = data.create_new_menuitem( parent_id, name.toStdString() );
    auto parent = data.find_menu_item( parent_id );

    save_item( id );

    if( parent ) {
        select( parent_id );
    } else {
        reset_menu();
    }

    return id;
}

void Settings::delete_user()
{
    if( !user ) return;
    data.delete_user( user->id );
    user = nullptr;
}

void Settings::delete_item(const int& id)
{
    if( selection && selection->id == id ) {
        selection = nullptr;
    }
    auto parent = data.find_menu_item( id )->parent;

    data.delete_menuitem( id );

    if( parent ) {
        select( parent->id );
    }
    else {
        reset_menu();
    }
}

void Settings::save_item( const int& id ) { data.save_menuitem( id ); }
void Settings::set_itemname(const QString& name)
{
    if( !selection ) return;
    selection->name = name.toStdString();
    int id = selection->id;
    if( selection->parent ) {
        select( selection->parent->id );
    }
    select( id );
    emit item_nameChanged();
    save_item( selection->id );
}
void Settings::set_itemprice(const double& price)
{
    if( !selection ) return;
    selection->price = price;

    int id =  selection->id;
    if( selection->parent ) {
        select( selection->parent->id );
    }
    select( id );
    emit item_priceChanged();
    save_item( selection->id );
}

void Settings::select(const int& id)
{
    selection = data.find_menu_item( id );

    emit item_nameChanged();
    emit item_priceChanged();
    emit item_idChanged();
    emit item_parentidChanged();
    emit item_parentnameChanged();

    if( !selection ) {
        reset_menu();
        return;
    }

    menu_list->clear();

    menu_list->appendRow(new MenuListItem("__BACK_ITEM__"));

    for( auto sub : selection->subitems() ) {
        if( sub->cat == selection->id && !sub->deleted) {
            menu_list->appendRow(new MenuListItem(sub));
        }
    }

    menu_list->appendRow(new MenuListItem("__ADD_ITEM__"));
}

void Settings::select_user(const int& user_id)
{
    user = data.find_user(user_id);

    emit user_nameChanged();
    emit user_first_nameChanged();
    emit user_last_nameChanged();
    emit user_idChanged();
    emit user_merchantChanged();
    emit user_titleChanged();
    emit user_levelChanged();
    emit user_passwordChanged();
}

void Settings::reset_user_list()
{
    const auto& users = data.getUserData();
    user_list->clear();
    std::for_each(begin(users), end(users),
                  [&](const std::unique_ptr<User>& u) {
                    user_list->appendRow( new UserSettingsItem(u.get()) );
    });
    user_list->appendRow( new UserSettingsItem(nullptr) );
}

int Settings::create_new_user()
{
    user = data.create_new_user();
    if( user )
        return user->id;

    return -1;
}

void Settings::save_user(const int& user_id)
{
    data.save_user_data(user_id);
}
