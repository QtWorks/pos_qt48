#include "tablelistmodel.h"
#include "table.h"
#include "sale.h"
#include "user.h"

//Table
TableListItem::TableListItem(Table* ptr) :
    ListItem(nullptr),
    table(ptr),
    roles()
{
    setRoles();
}

void TableListItem::setRoles() {
    roles[IdRole] = "table_id";
    roles[SeatsRole] = "table_seats";
    roles[XRole] = "table_x";
    roles[YRole] = "table_y";
    roles[UserRole] = "table_user";
    roles[SaleRole] = "table_sale";
    roles[NameRole] = "table_name";
    roles[SaleCountRole] = "table_salecount";
    roles[ReservedRole] = "table_reserved";
    roles[GuestsRole] = "table_guests";
}

QVariant TableListItem::data(int pRoleName) const {
    switch(pRoleName) {
        case IdRole:
            return QVariant::fromValue(table->id);
        case SeatsRole:
            return QVariant::fromValue(table->seats);
        case XRole:
            return QVariant::fromValue(table->x);
        case YRole:
            return QVariant::fromValue(table->y);
        case UserRole:
            return QVariant::fromValue( (table->user) ? table->user->id : -1 );
        case SaleRole:
            return QVariant::fromValue( (table->sale) ? table->sale->id : -1 );
        case SaleCountRole:
            return QVariant::fromValue( table->sale_count );
        case ReservedRole:
            return QVariant::fromValue( QString(table->reserve.c_str()) );
        case NameRole:
            return QVariant::fromValue( QString(table->name.c_str()));
        case GuestsRole:
            return QVariant::fromValue( (table->sale) ? table->sale->guests : -1 );
    };

    return QVariant::fromValue(QString("Undefined table value(ROLE ERROR)"));
}
