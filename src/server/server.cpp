#include "server.h"
#include<ctype.h>
#include "../sale.h"
#include "../menu.h"
#include "../itemfactory.h"
#include<QtDebug>

#define RES_UPTR    std::unique_ptr<Result>

DataServer::DataServer() :
    connected(false),
    pg_conn(nullptr),
    pg_result(nullptr),
    pid(0)
{
    pid = getpid();

    const char* connInfo = "dbname = posdb";

    if(PQping(connInfo) != PQPING_NO_RESPONSE){
        pg_conn = PQconnectdb(connInfo);

    } else {
        qDebug() << "No response from postgres server";
    }

    if(PQstatus(pg_conn) != CONNECTION_OK){
        qDebug() << "Could not connect to postgres";
        PQfinish(pg_conn);
    } else {
        connected = true;
    }
}

DataServer::~DataServer()
{

    PQfinish(pg_conn);
}

template <typename T> RES_UPTR
DataServer::command(T UNUSED(t) )
{
    return std::unique_ptr<Result>(new Result);
}

template<> RES_UPTR DataServer::command(ReadConfigCmd* UNUSED(cmd)) {
    sql << "SELECT NEXTVAL(pg_get_serial_sequence('sales', 'id'));";
    run_sql();

    Result* res = new Result;

    std::unique_ptr<Item> info(new Item);
    if( tuple_count() ) {
        info->push_property( "max_sale_id", read_int(0, "nextval") );
    }

    clr_sql();

    res->items.push_back(std::move(info));

    RES_UPTR ptr( res );
    return std::move( ptr );
}

template<> RES_UPTR DataServer::command(WriteLogCmd* cmd)
{
    sql << "INSERT into log(type, tag, stamp, user_id) values(";
    sql << cmd->type << ",'" << cmd->tag << "', now(), " << cmd->user << ");";
    run_sql();
    clr_sql();
    return nullptr;
}

template<> RES_UPTR DataServer::command(ReadTableAllCmd* UNUSED(pCmd))
{
    sql << "SELECT * FROM tables ORDER by id ASC";
    run_sql();

    RES_UPTR result(new Result);

    for(int i = 0; i < tuple_count(); i++){
        std::unique_ptr<Item> table(new Item);

        table->push_property("id",      read_int(i, "id"));
        table->push_property("seats",   read_int(i, "seats"));
        table->push_property("x",       read_int(i, "x"));
        table->push_property("y",       read_int(i, "y"));
        table->push_property("w",       read_int(i, "w"));
        table->push_property("h",       read_int(i, "h"));
        table->push_property("name",    read_cstring(i, "name"));
        table->push_property("reserve", read_cstring(i, "reserve"));
        table->push_property("uid",     -1);

        result->items.push_back(std::move(table));
    }

    clr_sql();

    return std::move( result );
}

template<> std::unique_ptr<Result> DataServer::command(NewTableCmd* cmd)
{
    sql << "insert into tables(seats, x, y, w, h, name) ";
    sql << "values(" << cmd->seats << ",";
    sql << cmd->x << "," << cmd->y << "," << cmd->w << "," << cmd->h << ",";
    sql << "'" << cmd->name << "') ";

    run_sql();
    clr_sql();

    return std::unique_ptr<Result>(nullptr);
}

template<> std::unique_ptr<Result> DataServer::command(WriteTableCmd* cmd)
{
    sql << "update tables set(reserve) = ";
    sql << "('" << cmd->name << "') ";
    sql << "where id = " << cmd->id;

    run_sql();
    clr_sql();

    return nullptr;
}

template<> std::unique_ptr<Result> DataServer::command(ReserveTableCmd* cmd)
{
    sql << "update tables set(reserve) = ";
    sql << "('" << cmd->reserve << "') ";
    sql << "where id = " << cmd->id;

    run_sql();
    clr_sql();

    return nullptr;
}

template<> std::unique_ptr<Result> DataServer::command(WriteNewMenuItemCmd *cmd)
{
    sql << "insert into menu_item(cat, name) ";
    sql << "values(" << cmd->parent_id << ",";
    sql << "'" << cmd->name << "') ";
    sql << "returning id;";

    run_sql();

    std::unique_ptr<Result> result( new Result );

    if( tuple_count() ) {
        Item* item = new Item;
        item->push_property("id", read_int(0, "id"));
        result->items.push_back( std::unique_ptr<Item>(item) );
    }

    clr_sql();

    return result;
}

template<> std::unique_ptr<Result> DataServer::command(MarkMenuItemDeletedCmd *cmd)
{
    sql << "UPDATE sold_items SET(name) = ('DELETED ITEM: '||name)";
    sql << " WHERE menu_id = " << cmd->id;

    run_sql();
    clr_sql();

    return std::unique_ptr<Result>(nullptr);
}

template<> std::unique_ptr<Result> DataServer::command(WriteMenuItemCmd *cmd)
{
    sql << "UPDATE menu_item";
    sql << " SET(name, cat, menu_id, subitem, recurring, price, addon, deleted, refill)";
    sql << " = (";
    sql << "'" << cmd->data->property<std::string>("name") << "',";
    sql << cmd->data->property<int>("cat") << ",";
    sql << cmd->data->property<int>("menu_id") << ",";
    sql << ((cmd->data->property<int>("subitem")) ? "TRUE" : "FALSE" ) << ",";
    sql << ((cmd->data->property<int>("recurring")) ? "TRUE" : "FALSE" ) << ",";
    sql << cmd->data->property<double>("price") << ",";
    sql << ((cmd->data->property<int>("addon")) ? "TRUE" : "FALSE") << ",";
    sql << ((cmd->data->property<int>("deleted")) ? "TRUE" : "FALSE") << ",";
    sql << cmd->data->property<int>("refill");

    sql << ") where id = " << cmd->data->property<int>("id");

    run_sql();
    clr_sql();

    return nullptr;
}

template<> RES_UPTR DataServer::command(ReadMenuCmd* cmd)
{
    sql << "SELECT id,"
            << "name, cat, menu_id,"
            << "subitem::int, recurring::int,"
            << "price,refill,addon::int, deleted::int "
            << "FROM menu_item WHERE menu_id = 0";
    if( cmd->item_id > -1 ) {
        sql << " and id = " << cmd->item_id;
    }
    sql << " ORDER by id ASC";

    run_sql();

    RES_UPTR result( new Result );

    for(int i = 0; i < tuple_count(); i++){
        Item item;
        item.push_property("id", read_int(i, "id"));
        item.push_property("name", read_cstring(i, "name"));
        item.push_property("cat", read_int(i, "cat"));
        item.push_property("menu_id", read_int(i, "menu_id"));
        item.push_property("subitem", read_int(i, "subitem"));
        item.push_property("recurring", read_int(i, "recurring"));
        item.push_property("price", read_double(i, "price"));
        item.push_property("addon", read_int(i, "addon"));
        item.push_property("deleted", read_int(i, "deleted"));
        item.push_property("refill", read_int(i, "refill"));

        std::unique_ptr<Item> ptr(new Item(item));
        result->items.push_back(std::move(ptr));
    }

    clr_sql();

    return std::move( result );
}
