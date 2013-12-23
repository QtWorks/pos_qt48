#include "settings.h"

ListModel* Settings::menuModel() { return menu_list.get(); }
ListModel* Settings::userModel() { return user_list.get(); }
QString Settings::item_name()
{
    if( selection )
        return QString( selection->name.c_str() );
    return QString("");
}
double Settings::item_price() { return ( selection ) ? selection->price : 0.0f; }
int Settings::item_id() { return ( selection ) ? selection->id : -1; }
int Settings::item_parentid() { return ( selection && selection->parent ) ? selection->parent->id : -1; }
QString Settings::item_parentname()
{
    if( selection && selection->parent )
        return QString( selection->parent->name.c_str() );
    return QString("Categories");
}
bool Settings::menuitem_modifiable(const int& menuid) { return !data.menuitem_isused(menuid); }
QString Settings::user_name() { return (user) ? QString( (user->first_name + " " + user->last_name).c_str()) : QString("undefined"); }
QString Settings::user_first_name() { return (user) ? QString( user->first_name.c_str() ) : QString("undefined"); }
QString Settings::user_last_name() { return (user) ? QString( user->last_name.c_str() ) : QString("undefined"); }
QString Settings::user_title() { return (user) ? QString( user->title.c_str() ) : QString("undefined"); }
QString Settings::user_password() { return (user) ? QString( user->pass.c_str() ) : QString("undefined"); }
int Settings::user_id() { return (user) ? user->id : -1; }
int Settings::user_merchant() { return (user) ? user->merchant : 0; }
int Settings::user_level() { return (user) ? user->level : -1; }

void Settings::set_user_password(const QString& pass)
{
    user->pass = pass.toStdString();
    emit user_passwordChanged();
    save_user( user->id );
}

void Settings::set_user_firstname(const QString& first_name)
{
    user->first_name = first_name.toStdString();
    emit user_first_nameChanged();
    emit user_nameChanged();
    save_user( user->id );
}
void Settings::set_user_lastname(const QString& last_name)
{
    user->last_name = last_name.toStdString();
    emit user_last_nameChanged();
    emit user_nameChanged();
    save_user( user->id );
}
void Settings::set_user_title(const QString& title)
{
    user->title = title.toStdString();
    emit user_titleChanged();
}
void Settings::set_user_merchant(const int& merchant)
{
    user->merchant = merchant;
    emit user_merchantChanged();
}
void Settings::set_user_level(const int& level)
{
    user->level = level;
    save_user(user->id);
    emit user_levelChanged();
}

QString Settings::find_user_first_name(const int& user_id)
{
    auto u = data.find_user( user_id );
    if( u ) {
        return QString( u->first_name.c_str() );
    }

    return QString("undefined");
}

QString Settings::find_user_last_name(const int& user_id)
{
    auto u = data.find_user( user_id );
    if( u ) {
        return QString( u->last_name.c_str() );
    }

    return QString("undefined");
}
