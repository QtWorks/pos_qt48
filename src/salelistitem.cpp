#include "salelistitem.h"
#include "sale.h"
#include "table.h"

//Active sales( User Screen )
SaleListItem::SaleListItem(Sale* ptr) :
    ListItem(nullptr),
    sale(ptr),
    roles()
{
    setRoles();
}

void SaleListItem::setRoles() {
    roles[IdRole]       = "table_id";
    roles[TableNameRole] = "table_name";
    roles[AmountRole]   = "sale_amount";
    roles[CustomerRole] = "table_customer";
    roles[SaleIdRole]   = "sale_id";
    roles[UserIdRole]   = "user_id";
    roles[GuestsRole]   = "table_guests";
}

QVariant SaleListItem::data(int pRoleName) const {
    if(!sale) {
        return QVariant::fromValue(QString("Undefined"));
    }

    switch(pRoleName) {
        case IdRole:
            return QVariant::fromValue(sale->table_number);
        case AmountRole:
            if(sale) {
                return QVariant::fromValue(sale->total);
            }
            return QVariant::fromValue(QString("Undefined Amount"));
        case CustomerRole:
            if(sale) {
                return QVariant::fromValue(-1);//mSalePtr->mTable->mCount);
            }
            return QVariant::fromValue(QString("Undefined Customer Count"));
        case SaleIdRole:
            if(sale) {
                return QVariant::fromValue(sale->id);
            }
            return QVariant::fromValue(QString("Undefined sale"));
        case UserIdRole:
            if( sale && sale->user )
                return QVariant::fromValue(sale->user_id);
            else
                return QVariant::fromValue( -1 );
        case TableNameRole:
            return QVariant::fromValue(QString(sale->table->name.c_str()));
        case GuestsRole:
            return QVariant::fromValue(sale->guests);
    };

    return QVariant::fromValue(QString("Undefined(Role error)"));
}
