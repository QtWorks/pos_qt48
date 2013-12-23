#include "user.h"
#include "sale.h"
#include "item.h"
#include <iostream>
#include <algorithm>
#include <utility>

User::User(std::unique_ptr<Item>&& data) :
    payload(std::move(data)),
    first_name(payload->property<std::string>("first_name")),
    last_name(payload->property<std::string>("last_name")),
    title(payload->property<std::string>("title")),
    id(payload->property<int>("id")),
    level(payload->property<int>("level")),
    clockedin(payload->property<int>("login")),
    session(payload->property<int>("session")),
    cashedin(payload->property<int>("cashedin")),
    staff_bank(payload->property<int>("staff_bank")),
    pass(payload->property<std::string>("pass")),
    bank(payload->property<double>("bank")),
    merchant(payload->property<int>("merchant")),
    cashier(payload->property<int>("cashier")),
    cash_received(payload->property<double>("cash_received")),
    credit_received(payload->property<double>("credit_received")),
    total_sales(payload->property<double>("total_sales")),
    total_change(payload->property<double>("total_change"))
{
}

User::~User() {
}

void User::attach_sale(Sale* sale_ptr)
{
    if( std::find( sales.begin(), sales.end(), sale_ptr ) == sales.end() ) {
        sales.push_back( sale_ptr );

        active_sales++;
        sale_ptr->session = session;
        sale_ptr->user = this;
    }
}

void User::detach_sale(Sale* sale_ptr)
{
    if( !sales.size() )
        return;
    auto it = std::find(sales.begin(), sales.end(), sale_ptr);
    if( it != sales.end() ) {
        sales.erase( it );
        active_sales--;
        sale_ptr->user = nullptr;
    }
}

std::unique_ptr<Item> User::get_data(void)
{
    Item* data = new Item;

    data->push_property("first_name", first_name);
    data->push_property("last_name", last_name);
    data->push_property("title", title);
    data->push_property("id",   id);
    data->push_property("level",    level);
    data->push_property("login",    clockedin);
    data->push_property("pass",     pass);
    data->push_property("bank",     bank);
    data->push_property("merchant", merchant);
    data->push_property("cashier", cashier);
    data->push_property("cashedin", cashedin);
    data->push_property("staff_bank", staff_bank);
    data->push_property("session", session);

    return std::unique_ptr<Item>(data);
}

void User::add_timecard(std::unique_ptr<Item>&& timecard)
{
    Time clockin ( static_cast<time_t>( timecard->property<int>("start_epoch") ) );

    if( clockin.yd == today.yd ) {
        clocked_seconds_today += timecard->property<int>("seconds");
    }

    hv.push_back( std::move(timecard) );
}

int User::seconds_timed_total()
{
    int seconds = 0;

    for( const auto& timecard : hv ) {
        seconds += timecard->property<int>("seconds");
    }

    return seconds;
}
