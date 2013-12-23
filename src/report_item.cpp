#include "reports.h"

ItemSaleReportItem::ItemSaleReportItem(std::unique_ptr<Item>&& item_ptr, MenuItem* menuitem_ptr)
    :   ListItem(nullptr),
        menu_item(menuitem_ptr),
        item(std::move(item_ptr))
{
    set_roles();
}

void ItemSaleReportItem::set_roles()
{
    roles[IdRole] = "mitem_id";
    roles[NameRole] = "mitem_name";
    roles[TotalRole] = "mitem_sale_total";
    roles[CountRole] = "mitem_sale_count";
}

QVariant ItemSaleReportItem::data(int roleid) const
{
    switch( roleid ) {
    case IdRole:
        return QVariant::fromValue( menu_item->id );
    case NameRole:
        return QVariant::fromValue( QString(menu_item->name.c_str()) );
    case TotalRole:
        return QVariant::fromValue( item->property<double>("total") );
    case CountRole:
        return QVariant::fromValue( item->property<int>("count") );
    };

    return QVariant::fromValue(QString("Undefined"));
}

HourSaleReportItem::HourSaleReportItem(std::unique_ptr<Item>&& item_ptr)
    :   ListItem(nullptr),
        item(std::move(item_ptr))
{
    set_roles();
}

void HourSaleReportItem::set_roles()
{
    roles[NameRole] = "hour_name";
    roles[TotalRole] = "hour_sale_total";
    roles[CountRole] = "hour_sale_count";
}

QVariant HourSaleReportItem::data(int roleid) const
{
    switch( roleid ) {
    case NameRole:
        return QVariant::fromValue( item->property<int>("name") );
    case TotalRole:
        return QVariant::fromValue( item->property<double>("total") );
    case CountRole:
        return QVariant::fromValue( item->property<int>("sale_count") );
    };

    return QVariant::fromValue(QString("undefined"));
}

EmpSaleReportItem::EmpSaleReportItem(std::unique_ptr<Item>&& item_ptr, User* user_ptr)
    :   ListItem(nullptr),
        user(user_ptr),
        item(std::move(item_ptr))
{
    set_roles();
}

void EmpSaleReportItem::set_roles()
{
    roles[IdRole] = "user_id";
    roles[FirstNameRole] = "user_firstname";
    roles[LastNameRole] = "user_lastname";
    roles[TotalRole] = "user_sale_total";
    roles[SaleCountRole] = "user_sale_count";
}

QVariant EmpSaleReportItem::data(int roleid) const
{
    //if( !user || !item ) return QVariant::fromValue(QString("Undefined"));

    switch( roleid ) {
    case IdRole:
        return QVariant::fromValue( user->id );
    case FirstNameRole:
        return QVariant::fromValue( QString(user->first_name.c_str()) );
    case LastNameRole:
        return QVariant::fromValue( QString(user->last_name.c_str()) );
    case TotalRole:
        return QVariant::fromValue( item->property<double>("total") );
    case SaleCountRole:
        return QVariant::fromValue( item->property<int>("sale_count") );
    };

    return QVariant::fromValue(QString("Undefined"));
}

SaleReportItem::SaleReportItem(std::unique_ptr<Item>&& data_ptr) :
    ListItem(nullptr),
    item(std::move(data_ptr))
{
    setRoles();
    if( item ) {
        label = item->property<std::string>("label");
        amt = item->property<double>("amt");
        tax = item->property<double>("tax");
        year = item->property<int>("year");
        month = item->property<int>("month");
        day = item->property<int>("day");
        item_count = item->property<int>("item_count");
        sid = item->property<int>("id");
        emp = item->property<int>("emp");
        table_num = item->property<int>("table_num");
        sale_count = item->property<int>("sale_count");
    }
}

SaleReportItem::~SaleReportItem()
{
}

void SaleReportItem::setRoles()
{
    mRoles[IdRole] = "item_id";
    mRoles[LabelRole] = "item_label";
    mRoles[YearRole] = "item_year";
    mRoles[MonthRole] = "item_month";
    mRoles[HourRole] = "item_hour";
    mRoles[MinuteRole] = "item_minute";
    mRoles[DayRole] = "item_day";
    mRoles[TotalRole] = "item_total";
    mRoles[ItemCountRole] = "item_itemcount";
    mRoles[EmpRole] = "emp";
    mRoles[TableRole] = "table_num";
    mRoles[AmountRole] = "item_amount";
    mRoles[SaleCount] = "item_salecount";
}

QVariant SaleReportItem::data(int roleName) const
{
    switch( roleName ) {
    case IdRole:
        return QVariant::fromValue(sid);
    case LabelRole:
        return QVariant::fromValue(QString(label.c_str()));
    case YearRole:
        return QVariant::fromValue(year);
    case MonthRole:
        return QVariant::fromValue(month);
    case HourRole:
        return QVariant::fromValue(item->property<int>("hour"));
    case MinuteRole:
        return QVariant::fromValue(item->property<int>("minute"));
    case DayRole:
        return QVariant::fromValue(day);
    case TotalRole:
        return QVariant::fromValue(amt + tax);
    case AmountRole:
        return QVariant::fromValue(amt);
    case ItemCountRole:
        return QVariant::fromValue(item_count);
    case EmpRole:
        return QVariant::fromValue(emp);
    case TableRole:
        return QVariant::fromValue(table_num);
    case SaleCount:
        return QVariant::fromValue(sale_count);
    }

    return QVariant::fromValue(QString("Undefined"));
}
