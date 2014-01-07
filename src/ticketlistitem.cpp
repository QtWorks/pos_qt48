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
    mRoles[QuantityRole] = "item_quantity";
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
        case QuantityRole:
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
    case QuantityRole:
        return QVariant::fromValue(order->quantity);
    case VoidRole:
        return QVariant::fromValue(order->void_flag);
    }

    return QVariant::fromValue(QString("Undefined"));
}
