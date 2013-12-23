#ifndef ACTIVE_TABLE_LIST_HEADER
#define ACTIVE_TABLE_LIST_HEADER

#include "listmodel.h"

class Sale;

class SaleListItem : public ListItem
{
    Sale* sale  =   nullptr;
    QHash<int, QByteArray> roles;

    void setRoles();

    public:
        //ctor
        SaleListItem(Sale* sale_ptr = nullptr);

        //dtor
        ~SaleListItem() {}

    public:
        inline QHash<int, QByteArray>   roleNames() const { return roles; }
        inline QString                  id()        const { return QString("Undefined"); }

    public:
        QVariant data(int pRoleName) const;

    public:
        enum Roles {
            IdRole = Qt::UserRole + 1,
            AmountRole,
            CustomerRole,
            SaleIdRole,
            RoleCount
        };
};

#endif
