#ifndef TICKETLISTMODEL_HEADER
#define TICKETLISTMODEL_HEADER

#include "ordereditem.h"
#include "listmodel.h"

class TicketListItem : public ListItem
{
    friend class TicketInfo;
    OrderedItem* order = nullptr;
    int unique_id = -1;
    QString state = "default";
    QHash<int, QByteArray>  mRoles;

    public:
        TicketListItem(OrderedItem* item = nullptr) :
            ListItem(0),
            order( item )
        {
            setRoles();
            if( order ) {
                unique_id = order->unique_id;
            }
        }

        ~TicketListItem(){}

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
            CommentRole,
            PriceRole,
            SubitemRole,
            SubItemIdRole,
            ParentIdRole,
            MenuIdRole,
            DisplayIdRole,
            FinalRole,
            RefillRole,
            MultiplierRole,
            VoidRole,
            RoleCount
        };

    public:
        QVariant data(int pRoleName) const;

    private:
        void setRoles();

};

#endif

