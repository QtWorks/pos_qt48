#include "server.h"
#include "../connect.h"

template<> std::unique_ptr<Result>
DataServer::command(ReadSaleSummaryDateCmd* cmd)
{
    sql  << "SELECT (amt - tax) as amt, tax,id,item_count,table_num,emp, date_part('hour', start_t) AS hour,"
            << " date_part('minute', start_t) as minute, session FROM sales WHERE amt > 0"
            << " AND date_part('year', start_t) = " << cmd->year
            << " AND date_part('month', start_t) = " << cmd->month
            << " AND date_part('day', start_t) = " << cmd->day
            << " ORDER BY hour DESC;";
    run_sql();

    std::unique_ptr<Result> result(new Result);

    for(int i = 0; i < tuple_count(); i++){
        std::unique_ptr<Item> sale(new Item);

        sale->push_property("label", "sale");
        sale->push_property("year", cmd->year);
        sale->push_property("month", cmd->month);
        sale->push_property("day", cmd->day);
        sale->push_property("hour",     read_int(i,      "hour"));
        sale->push_property("minute", read_int(i, "minute"));
        sale->push_property("amt",      read_double(i,    "amt"));
        sale->push_property("item_count", read_int(i, "item_count"));
        sale->push_property("emp",  read_int(i, "emp"));
        sale->push_property("table_num", read_int(i, "table_num"));
        sale->push_property("id", read_int(i, "id"));
        sale->push_property("session", read_int(i, "session"));
        sale->push_property("tax", read_double(i, "tax"));
        sale->push_property("sale_count", 1);

        result->items.push_back(std::move(sale));
    }

    clr_sql();

    return result;
}

template<> std::unique_ptr<Result>
DataServer::command(ReadSalesStatsUserCmd* cmd)
{
    if( cmd->user > 0 ) {
        sql << "select emp, sum(amt), count(amt) as sale_count from sales where amt > 0 and emp = " << cmd->user;
    }
    else {
        sql << "select distinct on (emp) emp, sum(amt), count(amt) as sale_count from sales where amt > 0;";
    }
    run_sql();

    std::unique_ptr<Result> result(new Result);

    for( int i = 0; i < tuple_count(); i++ ) {
        Item* sale_stat = new Item;
        sale_stat->push_property("user", read_int(i, "emp"));
        sale_stat->push_property("total", read_double(i, "sum"));
        sale_stat->push_property("sale_count", read_double(i, "sale_count"));
        result->items.push_back( std::unique_ptr<Item>(sale_stat) );
    }

    clr_sql();

    return result;
}

template<> std::unique_ptr<Result> DataServer::command(CompleteReportDateCmd* cmd)
{
    sql << "select coalesce(sum(amt), 0) as total_sales,";
    sql << "coalesce(count(amt), 0) as sale_count,";
    sql << "coalesce(sum(cc_paid), 0) as credit_charges,";
    sql << "coalesce(sum(amt - cc_paid), 0) as cash_total,";
    sql << "coalesce(sum(tender), 0) as cash_tendered ";
    sql << "from sales where amt > 0 and open = false";

    if( cmd->y != -1 )
        sql << " and extract(year from start_t) = " << cmd->y;
    if( cmd->m != -1 )
        sql << " and extract(month from start_t) = " << cmd->m;
    if( cmd->d != -1 )
        sql << " and extract(day from start_t) = " << cmd->d;

    //sql << " ;";

    run_sql();

    std::unique_ptr<Result> result(new Result);

    for( int i = 0; i < tuple_count(); i++ ) {
        Item* sale_stat = new Item;
        sale_stat->push_property("total_sales", read_double(i, "total_sales"));
        sale_stat->push_property("credit_charges", read_double(i, "credit_charges"));
        sale_stat->push_property("cash_total", read_double(i, "cash_total"));
        sale_stat->push_property("cash_tendered", read_double(i, "cash_tendered"));
        sale_stat->push_property("sale_count", read_int(i, "sale_count"));
        result->items.push_back( std::unique_ptr<Item>(sale_stat) );
    }

    clr_sql();

    return result;
}

template<> std::unique_ptr<Result> DataServer::command(DateReportUserCmd* cmd)
{
    sql << "select distinct on (emp) emp, sum(amt), count(amt) as sale_count from sales where amt > 0";
    if( cmd->y != -1 )
        sql << " and extract(year from start_t) = " << cmd->y;
    if( cmd->m != -1 )
        sql << " and extract(month from start_t) = " << cmd->m;
    if( cmd->d != -1 )
        sql << " and extract(day from start_t) = " << cmd->d;

    sql << " group by emp;";

    run_sql();

    std::unique_ptr<Result> result(new Result);

    for( int i = 0; i < tuple_count(); i++ ) {
        Item* sale_stat = new Item;
        sale_stat->push_property("id", read_int(i, "emp"));
        sale_stat->push_property("total", read_double(i, "sum"));
        sale_stat->push_property("sale_count", read_int(i, "sale_count"));
        result->items.push_back( std::unique_ptr<Item>(sale_stat) );
    }

    clr_sql();

    return result;
}

template<> std::unique_ptr<Result> DataServer::command(DateReportItemCmd* cmd)
{
    sql << "select distinct menu_id, sum(amount), count(amount) from sold_items where extract(year from tstamp) = " << cmd->y;
    if( cmd->m != -1 )
        sql << " and extract(month from tstamp) = " << cmd->m;
    if( cmd->d != -1 )
        sql << " and extract(day from tstamp) = " << cmd->d;

    sql << " group by menu_id order by sum desc;";

    run_sql();

    std::unique_ptr<Result> result(new Result);

    for( int i = 0; i < tuple_count(); i++ ) {
        Item* sold_item = new Item;
        sold_item->push_property("name", "");//read_cstring(i, "name"));
        sold_item->push_property("menu_id", read_int(i, "menu_id"));
        sold_item->push_property("total", read_double(i, "sum"));
        sold_item->push_property("count", read_int(i, "count"));

        result->items.push_back( std::unique_ptr<Item>(sold_item) );
    }

    clr_sql();

    return result;
}

template<> std::unique_ptr<Result> DataServer::command(DateReportHourCmd* cmd)
{
    sql << "select distinct extract(hour from start_t) as hour, sum(amt) as total, count(amt) as sale_count"
        << " from sales where amt > 0 ";
    if( cmd->y > -1 ) sql << "and extract(year from start_t) = " << cmd->y;
    if( cmd->m > -1 ) sql << " and extract(month from start_t) = " << cmd->m;
    if( cmd->d > -1 ) sql << " and extract(day from start_t) = " << cmd->d;

    sql << " group by hour order by total desc;";

    run_sql();
    std::unique_ptr<Result> result(new Result);

    for( int i = 0; i < tuple_count(); i++ ) {
        Item* hour_stat = new Item;
        hour_stat->push_property("name", read_int(i, "hour"));
        hour_stat->push_property("total", read_double(i, "total"));
        hour_stat->push_property("sale_count", read_int(i, "sale_count"));

        result->items.push_back( std::unique_ptr<Item>(hour_stat) );
    }

    clr_sql();

    return result;
}
