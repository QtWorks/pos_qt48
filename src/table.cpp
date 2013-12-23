#include "item.h"
#include "table.h"
#include "sale.h"

Table::Table(std::unique_ptr<Item>&& data) :
    payload(std::move(data)),
    id(payload->property<int>("id")),
    seats(payload->property<int>("seats")),
    user_id(payload->property<int>("uid")),
    x(payload->property<int>("x")),
    y(payload->property<int>("y")),
    w(payload->property<int>("w")),
    h(payload->property<int>("h")),
    name(payload->property<std::string>("name")),
    reserve(payload->property<std::string>("reserve"))
{
}

void Table::attach_sale(Sale* as)
{
    as->table = this;
    user = as->user;
    sale = as;
    sales.push_back(as);
    sale_count++;
}

void Table::detach_sale(Sale* as)
{
    for( auto it = sales.begin(); it != sales.end(); it++ ) {
        if( (*it) == as ) {

            sale = nullptr;
            (*it) = nullptr;

            sales.erase( it );
            sale_count--;

            return;
        }
    }
}
