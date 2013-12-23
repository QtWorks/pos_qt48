#ifndef TABLELISTMODEL_HEADER
#define TABLELISTMODEL_HEADER

#include "listmodel.h"

class Table;

class TableListItem : public ListItem
{
    Table* table = nullptr;
    QHash<int, QByteArray> roles;

    virtual void setRoles();

    public:
        TableListItem(Table* ptr = nullptr);
        ~TableListItem(){}

    public:
        virtual QHash<int, QByteArray> roleNames() const
        {
            return roles;
        }

        virtual QString id() const {
            return QString("Undefined");
        }

    public:
        enum Roles {
            IdRole = Qt::UserRole + 1,
            SeatsRole,
            XRole,
            YRole,
            UserRole,
            SaleRole,
            SaleCountRole,
            ReservedRole,
            RoleCount
        };

    public:
        virtual QVariant data(int pRoleName) const;
};

#endif //TABLELISTMODEL_HEADER

