#include "server/server.h"

template<> RES_UPTR DataServer::command(WriteOrderCmd * cmd)
{
    sql << "INSERT INTO sold_items(name, menu_id, sale_id, amount, modifies, order_id, tstamp) VALUES(";
    sql << "'" << cmd->name << "'," << cmd->menu_id  << "," << cmd->sale_id << "," << cmd->amount
        << "," << cmd->modifies << "," << cmd->order_id << ",'" << cmd->start_t << "')";
    sql << " RETURNING id;";

    run_sql();

    std::unique_ptr<Result> result(new Result);
    Item *item = new Item;
    item->push_property("id", read_int(0, "id"));
    result->push_back(item);

    clr_sql();

    return result;
}

template<> RES_UPTR DataServer::command(DeleteOrderCmd* cmd)
{
    sql << "delete from sold_items WHERE ";
    sql << "id = " << cmd->id << ";";

    run_sql();
    clr_sql();

    return std::unique_ptr<Result>(nullptr);
}

template<> RES_UPTR DataServer::command(DeleteAllSaleOrdersCmd* cmd)
{
    sql << "delete from sold_items WHERE sale_id = " << cmd->id;
    run_sql();
    clr_sql();

    return std::unique_ptr<Result>(nullptr);
}

template<> RES_UPTR DataServer::command(InsertOrderCmd* cmd)
{
    sql << "INSERT INTO sold_items(name, menu_id, sale_id, amount, modifies, order_id, tstamp)";
    sql << " VALUES(" << cmd->name << "," << cmd->menu_id << "," << cmd->sale_id << "," << cmd->amount;
    sql << "," << cmd->modifies << "," << cmd->order_id << "now() ) ";

    run_sql();
    clr_sql();

    return std::unique_ptr<Result>(nullptr);
}

template<> RES_UPTR DataServer::command( MoveOrderCmd* cmd )
{
    sql << "UPDATE sold_items set(sale_id) = (" << cmd->to << ") where id = " << cmd->id;
    run_sql();
    clr_sql();

    return nullptr;
}

template<> RES_UPTR DataServer::command( WriteOrderModCmd* cmd )
{
    sql << "UPDATE sold_items set(modifies) = (" << cmd->to << ") where id = " << cmd->id;
    run_sql();
    clr_sql();

    return nullptr;
}

