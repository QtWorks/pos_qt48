#include "cmain.h"
#include <QObject>
#include <QtDebug>
#include "salelistitem.h"

CMain::CMain(QObject* parent) :
    QObject(parent),
    mRootObject(nullptr),
    mData(&Data::Instance())
{
    model.user_sales = std::unique_ptr<ListModel>(new ListModel(new SaleListItem()));
    model.tables = std::unique_ptr<ListModel>(new ListModel(new TableListItem()));
    model.table_sales = std::unique_ptr<ListModel>(new ListModel(new SaleListItem()));
    model.all_sales = std::unique_ptr<ListModel>(new ListModel(new SaleListItem()));
}

int CMain::check_user_login(const std::string& pass)
{
    auto user = mData->find_user(pass);
    return ( user ) ? user->id : 0;
}

void CMain::log_out_user()
{
    mData->logout_user();
}


void CMain::update_table_list()
{

    model.tables->clear();
    for(const auto& table : mData->getTableData() ) {
        model.tables->appendRow(new TableListItem( table.get() ));
    }
}

void CMain::update_sale_list(int saleid, int user_id)
{
    model.user_sales->clear();

    auto picked_sale = mData->find_sale(saleid);
    if ( picked_sale ) {
        if( picked_sale->user->id == user_id ){
            model.user_sales->appendRow(new SaleListItem( picked_sale ));
        }
    }

    for( const auto& sale : mData->getSaleData() ) {
        auto user = sale->user;
        if ( user_id == user->id && sale->id != saleid && sale->open ) {
            model.user_sales->appendRow( new SaleListItem( sale.get() ) );
        }
    }
}

void CMain::update_ts_model()
{
    model.table_sales->clear();

    if( mData->active_table ) {
        for( const auto& sale : mData->active_table->get_sales() ) {
            model.table_sales->appendRow(new SaleListItem(sale));
        }
    }
}

void CMain::refresh()
{
    update_sale_list( (mData->active_sale) ? mData->active_sale->id : -1, mData->active_user->id );
    update_table_list();
    update_ts_model();


    emit tableSaleModelChanged();
    emit tableListModelChanged();
    emit activeSaleModelChanged();
}

void CMain::refreshActiveSales()
{
    update_sale_list( (mData->active_sale) ? mData->active_sale->id : -1, mData->active_user->id );
    emit activeSaleModelChanged();
}

void CMain::refreshTables()
{
    update_table_list();
    emit tableListModelChanged();
}

int CMain::login_user(const int& pUser_Id)
{
    int result = mData->login_user(pUser_Id);
    if ( result ) {
        mData->set_active_user(pUser_Id);
        update_sale_list(-1, pUser_Id);
    }

    return result;
}


void CMain::activate_table(const int& table_id)
{
    mData->set_active_table( mData->find_table( table_id ) );
    update_ts_model();
}

void CMain::reserve_table(const int& tableid, const QString& rstring)
{
    mData->reserve_table( tableid, rstring.toStdString() );
    update_table_list();
}

QString CMain::table_isreserved(const int& tableid)
{
    auto table = mData->find_table(tableid);
    if( table ) {
        return QString( table->reserve.c_str() );
    }

    return QString("");
}

int CMain::addNewSale(const int& tableid)
{
    int ret = mData->new_sale( tableid );
    if( mData->active_table ) {
        for( const auto& sale : mData->active_table->get_sales() ) {
            if( sale->id == ret ) {
                model.table_sales->appendRow(new SaleListItem(sale));
                return ret;
            }
        }
    }

    return ret;
}

int CMain::check_user_level(const QString& pass)
{
    auto user = mData->find_user( pass.toStdString() );
    if( user ) {
        return user->level;
    }

    return -1;
}

int CMain::get_user_id(const QString& pass)
{
    auto user = mData->find_user( pass.toStdString() );
    if( user )
        return user->id;
    return -1;
}

ListModel* CMain::allSaleModel()
{
    model.all_sales->clear();
    auto& sales = mData->getSaleData();
    for( const auto& sale : sales ) {
        model.all_sales->appendRow(new SaleListItem(sale.get()));
    }

    return model.all_sales.get();
}
