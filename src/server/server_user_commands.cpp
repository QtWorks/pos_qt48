#include "server.h"
#include "../item.h"
#include "../connect.h"
#include <utility>

template<> std::unique_ptr<Result> DataServer::command(CreateEmployeeCmd* UNUSED(cmd))
{
    sql << "INSERT INTO employee(level) VALUES(0) RETURNING id";
    run_sql();

    std::unique_ptr<Result> result( new Result );
    if( tuple_count() ) {
        std::unique_ptr<Item> user(new Item);
        user->push_property("id",   read_int(0, "id"));

        result->items.push_back(std::move(user));
    }

    clr_sql();

    return result;
}

//Read All employees
template<> std::unique_ptr<Result> DataServer::command(ReadEmployeeCmd* pCmd)
{
    sql << "SELECT * FROM employee";
    if(pCmd->mEmpId > 0)
        sql << " WHERE id = " << pCmd->mEmpId;
    run_sql();

    std::unique_ptr<Result> result( new Result );

    for(int i = 0; i < tuple_count(); i++ ) {
        std::unique_ptr<Item> user(new Item);

        user->push_property("first_name",   read_cstring(i, "first_name"));
        user->push_property("last_name",    read_cstring(i, "last_name"));
        user->push_property("pass",         read_cstring(i, "pass"));
        user->push_property("login",        read_int(i, "login"));
        user->push_property("id",           read_int(i, "id"));
        user->push_property("level",        read_int(i, "level"));
        user->push_property("title",        read_cstring(i, "title"));
        user->push_property("bank",         read_double(i, "bank"));
        user->push_property("merchant",     read_int(i, "merchant"));
        user->push_property("cashier",      read_int(i, "cashier"));
        user->push_property("cashedin" ,    read_int(i, "cashedin"));
        user->push_property("staff_bank",   read_int(i, "staff_bank"));
        user->push_property("session",      read_int(i, "session"));

        result->items.push_back(std::move(user));
    }

    clr_sql();

    for( auto& user : result->items ) {
        double total_sales = 0.0f;
        double cash_received = 0.0f;
        double credit_received = 0.0f;
        double total_change = 0.0f;

        if( user->property<int>("cashedin") ) {
            sql << "SELECT coalesce(sum(amt),0) as total_sales,";
            sql << "coalesce(sum(tender),0) as cash_received,";
            sql << "coalesce(sum(cc_paid),0) as credit_received,";
            sql << "coalesce(sum(change),0) as total_change,";
            sql << "coalesce(sum(amt - (tender - change)),0) as pending_sales";
            sql << " from sales WHERE open = FALSE AND emp = ";
            sql << user->property<int>("id") << " AND session = " << user->property<int>("session");

            run_sql();

            if( tuple_count() ) {
                total_sales = read_double(0, "total_sales");
                cash_received = read_double(0, "cash_received");
                credit_received = read_double(0, "credit_received");
                total_change = read_double(0, "total_change");
            }

            clr_sql();
        }
        user->push_property("total_sales", total_sales);
        user->push_property("cash_received", cash_received);
        user->push_property("credit_received", credit_received);
        user->push_property("total_change", total_change);
    }



    return std::move( result );
}

template<> std::unique_ptr<Result> DataServer::command(WriteEmployeeCmd* cmd)
{
    if( cmd->data->property<int>("id") != -1 ) {
        sql << "UPDATE employee ";
        sql << "SET(first_name, last_name, pass, login,"
               << "level, title, bank, merchant, cashier, cashedin, staff_bank, session) = ";
    }
    sql << "(" << "'" << cmd->data->property<std::string>("first_name") << "',";
    sql << "'" << cmd->data->property<std::string>("last_name") << "',";
    sql << "'" << cmd->data->property<std::string>("pass") << "',";
    sql << cmd->data->property<int>("login") << ",";
    sql << cmd->data->property<int>("level") << ",";
    sql << "'" << cmd->data->property<std::string>("title") << "',";
    sql << cmd->data->property<double>("bank") << ",";
    sql << cmd->data->property<int>("merchant") << ",";
    sql << cmd->data->property<int>("cashier") << ",";
    sql << cmd->data->property<int>("cashedin") << ",";
    sql << cmd->data->property<int>("staff_bank") << ",";
    sql << cmd->data->property<int>("session") << ")";

    if( cmd->data->property<int>("id") != -1 ) {
        sql << " WHERE id = " << cmd->data->property<int>("id") << ";";
    }

    run_sql();


    clr_sql();

    return std::move( std::unique_ptr<Result>(nullptr) );
}

template<> std::unique_ptr<Result> DataServer::command(ClockinEmployeeCmd* cmd)
{
    sql << "INSERT INTO timeclock(emp, clockin, stamp) VALUES(";
    sql << cmd->id << ", " << 1 << ", NOW() ) ";
    sql << "RETURNING date_part('year', stamp) as year,"
           << "date_part('month', stamp) as month, date_part('day', stamp) as day, date_part('hour', stamp) as hour,"
           << "date_part('minute', stamp) as minute, date_part('second', stamp) as sec"
           << ",extract(epoch from now())::int as session;" ;

    run_sql();

    std::unique_ptr<Item> time(new Item);

    time->push_property("year", read_double(0, "hour"));
    time->push_property("month", read_double(0, "month"));
    time->push_property("day", read_double(0, "day"));
    time->push_property("hour", read_double(0, "hour"));
    time->push_property("minute", read_double(0, "minute"));
    time->push_property("sec", read_double(0, "sec"));
    time->push_property("session", read_int(0, "session"));

    std::unique_ptr<Result> result(new Result);
    result->items.push_back(std::move(time));

    clr_sql();

    return std::move( result );
}

template<> std::unique_ptr<Result> DataServer::command(ClockoutEmployeeCmd* cmd)
{
    sql << "INSERT INTO timeclock(emp, clockin, stamp) VALUES(";
    sql << cmd->id << ", " << 0 << ", NOW() );";


    run_sql();
    clr_sql();

    //Run another command and return a new timecard item(last timecard)
    sql <<  "SELECT "
            << "max(cin.stamp) as start_time, "
            << "cout.stamp as end_time, "
            << "extract(epoch from max(cin.stamp))::int as start_epoch, "
            << "extract(epoch from cout.stamp)::int as end_epoch, "
            << "abs(extract( epoch from max(cin.stamp)) - extract( epoch from cout.stamp) )::int as seconds "
            << "FROM "
            << "timeclock as cin "
            << "INNER JOIN "
            << "timeclock as cout ON ( "
            <<    "cin.emp = cout.emp AND "
            <<    "cout.stamp > cin.stamp )"
            << "WHERE cin.clockin = 1 "
            << "AND cout.clockin = 0 "
            << "AND cin.emp = " << cmd->id
            << " GROUP by cout.stamp ORDER BY cout.stamp DESC LIMIT 1;";
    run_sql();

    std::unique_ptr<Result> result(new Result);

    if( tuple_count() ) {
        std::unique_ptr<Item> timecard(new Item);

        timecard->push_property( "start_epoch", read_int(0, "start_epoch") );
        timecard->push_property( "end_epoch", read_int(0, "end_epoch") );
        timecard->push_property( "start_time", read_cstring(0, "start_time") );
        timecard->push_property( "end_time", read_cstring(0, "end_time") );
        timecard->push_property( "seconds", read_int(0, "seconds") );

        result->items.push_back(std::move(timecard));
    }

    clr_sql();

    return std::move( result );
}

template<> std::unique_ptr<Result> DataServer::command(ReadEmployeeHours* cmd)
{
    sql <<  "SELECT "
            << "max(cin.stamp) as start_time, "
            << "cout.stamp as end_time, "
            /*<< "extract(year from max(cin.stamp))::int as start_yr,"
            << "extract(month from max(cin.stamp))::int as start_mo,"
            << "extract(day from max(cin.stamp))::int as start_dy,"*/
            << "extract(epoch from max(cin.stamp))::int as start_epoch, "
            << "extract(epoch from cout.stamp)::int as end_epoch, "
            << "abs(extract( epoch from max(cin.stamp)) - extract( epoch from cout.stamp) )::int as seconds "
            << "FROM "
            << "timeclock as cin "
            << "INNER JOIN "
            << "timeclock as cout ON ( "
            <<    "cin.emp = cout.emp AND "
            <<    "cout.stamp > cin.stamp )"
            << "WHERE cin.clockin = 1 "
            << "AND cout.clockin = 0 "
            << "AND cin.emp = " << cmd->uid
            << " GROUP by cout.stamp";
    if( cmd->limit ) {
        sql << " LIMIT " << cmd->limit;
    }
    sql << ";";

    run_sql();

    std::unique_ptr<Result> result(new Result);

    for( int i = 0; i < tuple_count(); i++ ) {
        std::unique_ptr<Item> hours(new Item);

        hours->push_property("start_epoch", read_int(i, "start_epoch") );
        hours->push_property("end_epoch", read_int(i, "end_epoch") );
        hours->push_property("start_time",  read_cstring(i, "start_time") );
        hours->push_property("end_time", read_cstring(i, "end_time") );
        hours->push_property("seconds", read_int(i, "seconds") );

        result->items.push_back(std::move(hours));
    }

    clr_sql();

    return std::move( result );
}

template<> std::unique_ptr<Result> DataServer::command(ReadEmployeeSaleTotals* cmd)
{
    sql << "select "
        << "coalesce(sum(amt),0) as total_sales,"
        << "coalesce(sum(amt - (tender - change)),0) as pending_sales,"
        << "coalesce(sum(tender),0) as cash_received,"
        << "coalesce(sum(cc_paid),0) as credit_received,"
        << "coalesce(sum(change),0) as total_change"
        << " from sales where emp = " << cmd->emp;
    if( cmd->session != -1 ) {
        sql << " and session = " << cmd->session;
    }

    run_sql();

    std::unique_ptr<Result> result(new Result);

    if( tuple_count() ) {
        std::unique_ptr<Item> totals(new Item);

        totals->push_property("total_sales", read_double(0, "total_sales"));
        totals->push_property("cash_received", read_double(0, "cash_received"));
        totals->push_property("credit_received", read_double(0, "credit_received"));
        totals->push_property("total_change", read_double(0, "total_change"));
        totals->push_property("pending_sales", read_double(0, "pending_sales"));

        result->items.push_back(std::move(totals));
    }

    clr_sql();

    return result;
}

template<> std::unique_ptr<Result> DataServer::command(RemoveEmployeeCmd* cmd)
{
    sql << "DELETE FROM employee WHERE ID=" << cmd->id;
    run_sql();
    clr_sql();
    return nullptr;
}
