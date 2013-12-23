#ifndef MENULISTMODEL_HEADER
#define MENULISTMODEL_HEADER

#include "menu.h"
#include "listmodel.h"

class MenuListItem : public ListItem
{
    MenuItem*   item = nullptr;
    bool        selected = false;
    std::string tag;

    public:
        MenuListItem(MenuItem* pItemPtr = nullptr) :
            ListItem(nullptr),
            item(pItemPtr),
            mRoles()
        {
            setRoles();
        }

        MenuListItem(const std::string& Tag)
            :   ListItem(nullptr),
                item(nullptr),
                tag(Tag),
                mRoles()
        {
            setRoles();
        }

        ~MenuListItem() {}

    public:
        QHash<int, QByteArray> roleNames() const
        {
            return mRoles;
        }

        QString id() const {
            return QString("Undefined");
        }

    public:
        enum Roles {
            IdRole = Qt::UserRole + 1,
            NameRole,
            DescriptionRole,
            PriceRole,
            SubItemRole,
            SelectedRole,
            ParentIdRole,
            CategoryRole,
            RoleCount
        };

    public:
        QVariant data(int pRoleName) const {
            if( !item ) {
                switch( pRoleName ) {
                case IdRole:
                case SubItemRole:
                case SelectedRole:
                case PriceRole:
                case ParentIdRole:
                case CategoryRole:
                    return QVariant::fromValue(0.0f);
                case NameRole:
                    return QVariant::fromValue( QString(tag.c_str()) );
                };

                return QVariant::fromValue(QString("undefined"));
            }
            switch(pRoleName){
                case IdRole:
                    return QVariant::fromValue(item->id);
                case NameRole:
                    return QVariant::fromValue( QString( item->name.c_str() ) );
                case SubItemRole:
                    return QVariant::fromValue( item->subitem );
                case SelectedRole:
                    return QVariant::fromValue( selected );
                case PriceRole:
                    return QVariant::fromValue( item->price );
                case ParentIdRole:
                    if( item->parent )
                        return QVariant::fromValue( item->parent->id );
                    else
                        return QVariant::fromValue( -1 );
                case CategoryRole: {
                    auto parent = item;
                    while( parent->parent ) {
                        parent = parent->parent;
                    }
                    return QVariant::fromValue( parent->id );
                }

            };
            return QVariant::fromValue(QString("Undefined"));
        }

    private:
        inline void setRoles() {
            mRoles[IdRole] = "item_id";
            mRoles[NameRole] = "item_name";
            mRoles[DescriptionRole] = "item_description";
            mRoles[SubItemRole] = "item_issub";
            mRoles[PriceRole] = "item_price";
            mRoles[SelectedRole] = "item_selected";
            mRoles[ParentIdRole] = "item_parentid";
            mRoles[CategoryRole] = "item_catid";
        }

    private:
        QHash<int, QByteArray> mRoles;
};

#endif //MENULISTMODEL_HEADER
