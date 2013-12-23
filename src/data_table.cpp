#include "data.h"

Sale* DataHandler::find_sale_at_table(const int& table_id) const
{
    for(const auto& sale : Sales) {
        if( sale->table_number == table_id)
            return sale.get();
    }

    return nullptr;
}

void DataHandler::save_table(const int& tableid)
{
    auto table = find_table(tableid);
    if( table ) {
        dserver->command(new WriteTableCmd(tableid, table->name));
    }
}

void DataHandler::reserve_table(const int& tableid, const std::string& rstring)
{
    auto table = find_table( tableid );
    if( table ) {
        table->reserve = rstring;
        dserver->command(new ReserveTableCmd(tableid, table->reserve));
    }
}
