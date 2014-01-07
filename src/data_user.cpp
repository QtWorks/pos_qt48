#include "data.h"
#include "server/server.h"
#include "connect.h"

#include <utility>
#include <algorithm>

User* DataHandler::create_new_user()
{
    auto result = dserver->command( new CreateEmployeeCmd() );
    if( result->size() ) {
        result = dserver->command( new ReadEmployeeCmd(result->items[0]->property<int>("id")) );
        return add_user( std::move(result->items[0]) );
    }

    return nullptr;
}

User* DataHandler::add_user(std::unique_ptr<Item>&& user_def)
{
    //Users.push_back( std::move( User(std::move(user_def)) ) );
    auto user = new User( std::forward< std::unique_ptr<Item>&& >(user_def) );
    Users.emplace_back( user );
    return user;
}

void DataHandler::delete_user(const int& user_id)
{
    auto ut = std::find_if(begin(Users), end(Users),
                            [&](std::unique_ptr<User>& p) { return p->id == user_id; });
    if( ut != end(Users) ) {
        dserver->command( new RemoveEmployeeCmd(user_id) );
        Users.erase( ut );
    }
}

User* DataHandler::find_user(const int& user_id)
{
    auto u = std::find_if(begin(Users), end(Users),
                          [&](std::unique_ptr<User>& p) { return p->id == user_id; });
    return (u != end(Users)) ? (*u).get() : nullptr;
    /*for( auto& user : Users ) {
        if( user->id == user_id )
            return user.get();
    }

    return nullptr;*/
}

User* DataHandler::find_user(const std::string& password)
{
    auto u = std::find_if(begin(Users), end(Users),
                          [&](const std::unique_ptr<User>& p) { return p->pass == password; });
    return (u != end(Users)) ? (*u).get() : nullptr;
    /*for( auto& user : Users ) {
        if( user->pass == password )
           return user.get();
    }

    return nullptr;*/
}

void DataHandler::set_active_user(const int& user_id)
{
    active_user = find_user(user_id);
}

void DataHandler::set_active_user(User* user)
{
    active_user = user;
}

int DataHandler::login_user(int user_id)
{
    set_active_user(user_id);

    if( active_user )
        return active_user->level;
    else
        return 0;
}

void DataHandler::logout_user()
{
    active_user = nullptr;
}

void DataHandler::save_user_data(const int& user_id)
{
    save_user_data( find_user(user_id) );
}

void DataHandler::save_user_data(User* user)
{
    dserver->command( new WriteEmployeeCmd( user->get_data() ) );
}

void DataHandler::clockin_user(User* user)
{
    if( !user->clockedin ) {
        auto res = dserver->command( new ClockinEmployeeCmd( user->id ) );

        user->clockedin = true;

        save_user_data(user);

    }
}

int DataHandler::clockout_user(User* user)
{
    int seconds = 0;
    if( user->clockedin ) {
        auto res = dserver->command( new  ClockoutEmployeeCmd( user->id ) );

        user->clockedin = false;

        //add the resulting timecard
        for( auto& tm : res->items ) {
            seconds = tm->property<int>("seconds");
            user->add_timecard( std::move( tm ) );
            /*
            user->timecard.label = user->first_name;
            user->timecard.start = tm->property<std::string>("start");
            user->timecard.end = tm->property<std::string>("end");
            user->timecard.start_e = tm->property<int>("start_epoch");
            user->timecard.end_e = tm->property<int>("end_epoch");
            */
        }

        //printer.print(&user->timecard);

        save_user_data( user );
    }

    return seconds;
}

void DataHandler::fill_user_hours(User* user)
{
    auto res = dserver->command( new ReadEmployeeHours( user->id ) );

    for( auto& timecard : res->items ) {
        user->add_timecard( std::move( timecard ) );
    }
}

void DataHandler::update_user_sale_totals(User* user)
{
    auto res = dserver->command(new ReadEmployeeSaleTotals( user->id, user->session ));
    if( res->size() ) {
        auto& totals = res->items[0];

        user->total_sales = totals->property<double>("total_sales");
        user->cash_received = totals->property<double>("cash_received");
        user->credit_received = totals->property<double>("credit_received");
        user->total_change = totals->property<double>("total_change");
        user->pending_sales_total = totals->property<double>("pending_sales");
    }
}

void DataHandler::print_session(Session *session)
{
    printer.print( session );
}

