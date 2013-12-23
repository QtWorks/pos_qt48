#include "server.h"
#include "../sale.h"


//Read aggregate command for fetching sale summary
template<> RES_UPTR
DataServer::command(ReadSalesSummaryCmd* cmd)
{
    sql << "SELECT DISTINCT extract(" << cmd->dist << " from start_t) as " << cmd->dist
            << ",sum(amt),count(amt) as sale_count FROM SALES WHERE amt > 0";
    if( cmd->year > 0 ) {
        sql << " AND extract(year from start_t) = " << cmd->year;
    }
    if( cmd->month > 0 ) {
        sql << " AND date_part('month', start_t) = " << cmd->month;
    }
    if( cmd->day > 0 ) {
        sql << " AND date_part('day', start_t) = " << cmd->day;
    }

    sql << " GROUP by " << cmd->dist << " ORDER by " << cmd->dist << " DESC;";

    run_sql();

    RES_UPTR result(new Result);

    for( int i = 0; i < tuple_count(); i++ ) {
        std::unique_ptr<Item>  report(new Item);

        report->push_property("label", cmd->dist);
        report->push_property("year",  (!cmd->year) ? read_int(i, "year") : cmd->year);
        report->push_property("month", (!cmd->month && cmd->year)  ? read_int(i, "month") : cmd->month);
        report->push_property("day", (!cmd->day && cmd->year && cmd->month) ? read_int(i, "day") : cmd->day);
        report->push_property("amt", read_double(i, "sum"));
        report->push_property("sale_count", read_int(i, "sale_count"));
        report->push_property("tax", 0.0f);
        result->items.push_back(std::move(report));
    }

    clr_sql();

    return result;
}

template<> RES_UPTR DataServer::command(ReadSaleCmd* cmd)
{
    sql << "select * from sales where id = " << cmd->id;

    run_sql();

    RES_UPTR result(new Result);
    if( tuple_count() ) {
        std::unique_ptr<Item> sale(new Item);

        sale->push_property("amt",      read_double(0,  "amt"));
        sale->push_property("emp",      read_int(   0,  "emp"));
        sale->push_property("tax",      read_double(0,  "tax"));
        sale->push_property("table",    read_int(   0,  "table_num"));
        sale->push_property("tender",   read_double(0,  "tender"));
        sale->push_property("open",     1);
        sale->push_property("id",       read_int(   0,  "id"));
        sale->push_property("item_count", read_int( 0,  "item_count"));
        sale->push_property("session",  read_int(   0,  "session"));
        sale->push_property("cc_paid", read_double( 0,  "cc_paid"));
        sale->push_property("change", read_double(  0,  "change"));
        sale->push_property("start_t", read_cstring( 0, "start_t"));
        result->items.push_back(std::move(sale));
    }

    clr_sql();

    return result;
}

template<> RES_UPTR DataServer::command(ReadOpenSalesAllCmd* UNUSED(cmd))
{
    sql << "select * from sales where open = true order by end_t DESC;";
    run_sql();

    RES_UPTR result( new Result );
    for(int i = 0; i < tuple_count(); i++){
        std::unique_ptr<Item> sale(new Item);
        sale->push_property("amt",      read_double(i,    "amt"));
        sale->push_property("emp",      read_int(i,      "emp"));
        sale->push_property("tax",      read_double(i,    "tax"));
        sale->push_property("table",    read_int(i,      "table_num"));
        sale->push_property("tender",   read_double(i,    "tender"));
        sale->push_property("open",     1);
        sale->push_property("id",       read_int(i,      "id"));
        sale->push_property("item_count", read_int(i,    "item_count"));
        sale->push_property("session",  read_int(i,     "session"));
        sale->push_property("cc_paid", read_double(i, "cc_paid"));
        sale->push_property("change", read_double(i, "change"));
        sale->push_property("start_t", read_cstring(i, "start_t"));

        result->items.push_back(std::move(sale));
    }

    clr_sql();

    return std::move( result );
}

template<> RES_UPTR DataServer::command(ReadSalesDetailsDateCmd* cmd)
{
    sql << "select * from sales where extract(" << cmd->selection << " from start_t) = " << cmd->val;

    run_sql();
    clr_sql();

    RES_UPTR result(nullptr);

    return result;
}

//Write the sale to database
template<> RES_UPTR DataServer::command(WriteSaleCmd* cmd)
{
    if( cmd->isnew ) {
        sql << "INSERT INTO sales(id, start_t, ";
    }
    else {
        sql  << "UPDATE sales SET(";
    }
    sql  << "end_t,emp,amt,tax,tender,open,table_num,item_count,session,cc_paid,change)";
    if( cmd->isnew ) {
        sql << " VALUES(DEFAULT, now(), ";
    }
    else {
        sql << " = ( ";
    }
        sql << "now(),"
            << cmd->info->property<int>("emp") << ","
            << cmd->info->property<double>("amt") << ","
            << cmd->info->property<double>("tax") << ","
            << cmd->info->property<double>("tender") << ","
            << "CAST(" << cmd->info->property<int>("open") << " as BOOLEAN),"
            << cmd->info->property<int>("table") << ","
            << cmd->info->property<int>("item_count") << ","
            << cmd->info->property<int>("session") << ","
            << cmd->info->property<double>("cc_paid") << ","
            << cmd->info->property<double>("change") << " )";
    if( !cmd->isnew ) {
        sql << " WHERE id = " << cmd->info->property<int>("id") << " AND emp = " << cmd->info->property<int>("emp");
    }
    sql << " RETURNING id, start_t;";

    //execute query here
    run_sql();
    RES_UPTR result(new Result);

    std::string start_t;

    //When a new sale is written return the new id to system
    if( tuple_count() ) {
        start_t = read_cstring(0, "start_t");
        if( cmd->isnew ) {
            Item* sale = new Item;
            sale->push_property("id", read_int(0, "id"));
            sale->push_property("start_t", start_t);

            result->items.push_back(std::unique_ptr<Item>(sale));
        }
    }
    clr_sql();
    return result;
}

template<> RES_UPTR DataServer::command(ReadOpenSalesEmpCmd* pCmd)
{
    sql  << "SELECT date_part('year', start_t) as year,"
            << "date_part('month', start_t) as month,"
            << "date_part('day', start_t) as day,"
            << "date_part('hour', start_t) as hour,"
            << "date_part('minute', start_t) as minute,"
            << "amt, emp, tax, table_num, tender, id, item_count,session "
            << "FROM sales WHERE emp = " << pCmd->mEmpId << " AND "
            << "item_count > 0 ORDER BY start_t DESC;";
    run_sql();
    clr_sql();

    return std::move( std::unique_ptr<Result>(nullptr) );
}

template<> RES_UPTR DataServer::command(ReadSaleDataCmd* pCmd)
{
    sql << "SELECT id,menu_id,name,modifies,amount FROM sold_items WHERE sale_id = " << pCmd->mSaleId;
    sql << " ORDER BY id ASC;";
    run_sql();

    RES_UPTR result(new Result);

    for( int i = 0; i < tuple_count(); i++) {

        Item* order = new Item;
        order->push_property("uid",         read_int(i, "id"));
        order->push_property("menu_id",     read_int(i, "menu_id"));
        order->push_property("name",        read_cstring(i, "name") );
        order->push_property("modifies",    read_int(i, "modifies") );
        order->push_property("amount",      read_double(i, "amount") );

        result->items.push_back(std::unique_ptr<Item>(order));
    }

    clr_sql();

    return std::move( result );
}

template<> RES_UPTR DataServer::command( DeleteSaleCmd* cmd )
{
    sql << "delete from sales where id = " << cmd->id;
    run_sql(); clr_sql();
    sql << "delete from sold_items where sale_id = " << cmd->id;
    run_sql(); clr_sql();

    return nullptr;
}
