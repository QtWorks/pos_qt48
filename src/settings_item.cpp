#include "settings.h"

UserSettingsItem::UserSettingsItem(User* user_ptr)
    :   ListItem(nullptr),
        user(user_ptr)
{
    set_roles();
}

void UserSettingsItem::set_roles()
{
    roles[FirstNameRole] = "user_first_name";
    roles[LastNameRole] = "user_last_name";
    roles[TitleRole] = "user_title";
    roles[PasswordRole] = "user_password";
    roles[MerchantRole] = "user_merchant";
    roles[IdRole] = "user_id";
    roles[LevelRole] = "user_level";
    roles[FullNameRole] = "user_name";
}

QVariant UserSettingsItem::data(int roleid) const
{
    if( !user ) {
        switch( roleid ) {
        case FirstNameRole:
        case LastNameRole:
        case PasswordRole:
        case FullNameRole:
        case TitleRole:
            return QVariant::fromValue(QString("add_user"));
        case MerchantRole:
        case IdRole:
        case LevelRole:
            return QVariant::fromValue(-1);
        };

        return QVariant::fromValue( QString("undefined") );
    }

    switch( roleid ) {
    case FullNameRole:
        return QVariant::fromValue( QString((user->first_name + " " + user->last_name).c_str()) );
    case FirstNameRole:
        return QVariant::fromValue( QString(user->first_name.c_str()) );
    case LastNameRole:
        return QVariant::fromValue( QString( user->last_name.c_str()) );
    case TitleRole:
        return QVariant::fromValue( QString(user->title.c_str()) );
    case LevelRole:
        return QVariant::fromValue( user->level );
    case PasswordRole:
        return QVariant::fromValue( QString( user->pass.c_str()) );
    case MerchantRole:
        return QVariant::fromValue( user->merchant );
    case IdRole:
        return QVariant::fromValue( user->id );
    }

    return QVariant::fromValue( QString("undefined") );
}
