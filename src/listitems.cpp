#include "tablelistmodel.h"
#include "activetablelist.h"
#include "ticketlistmodel.h"

#include "table.h"
#include "sale.h"
#include "user.h"
#include "menu.h"
#include <iostream>

void TicketListItem::setRoles()
{
    mRoles[IdRole] = "item_id";
    mRoles[NameRole] = "item_name";
    mRoles[CommentRole] = "item_comment";
    mRoles[PriceRole] = "item_total";
    mRoles[SubitemRole] = "item_is_sub";
    mRoles[SubItemIdRole] = "item_subid";
    mRoles[ParentIdRole] = "item_parentid";
    mRoles[MenuIdRole] = "item_menuid";
    mRoles[DisplayIdRole] = "item_displayid";
    mRoles[FinalRole] = "item_isfinal";
    mRoles[RefillRole] = "item_refill";
    mRoles[MultiplierRole] = "item_multiplier";
    mRoles[VoidRole] = "item_void";
}

QVariant TicketListItem::data(int pRoleName) const
{
    if( !order ) {
        switch(pRoleName) {
        case IdRole:
        case PriceRole:
        case SubitemRole:
        case ParentIdRole:
        case DisplayIdRole:
        case MenuIdRole:
        case MultiplierRole:
            return QVariant::fromValue(-1);
        case NameRole:
            return QVariant::fromValue(QString("DELETED"));
        };

        return QVariant::fromValue(QString(""));
    }

    switch(pRoleName) {
    case IdRole:
        return QVariant::fromValue(order->id);
    case NameRole:
        if( order->name.length() )
            return QVariant::fromValue(QString(order->name.c_str()));
        else
            return QVariant::fromValue(QString("undefined"));
    case CommentRole:
        return QVariant::fromValue(order->comment);
    case PriceRole:
        return QVariant::fromValue(order->total);
    case SubitemRole:
        return QVariant::fromValue(order->subitem);
    case SubItemIdRole:
        if(order->subitem && order->parent)
            return QVariant::fromValue(order->sub_id);
    case ParentIdRole:
        if(order->definition && order->definition->parent) {
            return QVariant::fromValue(order->definition->parent->id);
        }
        else { //Things like comments have no parrents for example
            return QVariant::fromValue(-1);
        }
        break;
    case MenuIdRole:
            return QVariant::fromValue(order->menu_id);
    case DisplayIdRole:
        return QVariant::fromValue(order->display_id);
    case FinalRole:
        return QVariant::fromValue(order->final);
    case RefillRole:
        return QVariant::fromValue(order->refill);
    case MultiplierRole:
        return QVariant::fromValue(order->multiply);
    case VoidRole:
        return QVariant::fromValue(order->void_flag);
    }

    return QVariant::fromValue(QString("Undefined"));
}

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
    roles[SaleCountRole] = "table_salecount";
    roles[ReservedRole] = "table_reserved";
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
    };

    return QVariant::fromValue(QString("Undefined table value(ROLE ERROR)"));
}


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
    roles[AmountRole]   = "sale_amount";
    roles[CustomerRole] = "table_customer";
    roles[SaleIdRole]   = "sale_id";
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
    };

    return QVariant::fromValue(QString("Undefined(Role error)"));
}
