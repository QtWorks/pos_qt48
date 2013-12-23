#include "userinfo.h"
#include <iostream>

UserInfo::UserInfo(QObject* parent) :
    QObject( parent ),
    data(Data::Instance())
{
}

int UserInfo::login(const QString& password)
{
    user = data.find_user( password.toStdString() );
    if( user ) {
        data.login_user( user->id );
        data.set_active_user( user );

        emit firstNameChanged();
        emit lastNameChanged();
        emit titleChanged();
        emit uidChanged();
        emit levelChanged();
        emit clockedinChanged();
        emit cashedinChanged();

        data.fill_user_hours( user );
        if( user->session ) {
            data.update_user_sale_totals( user );
        }
        return user->id;
    }

    return -1;
}

void UserInfo::logout()
{
    data.logout_user();

    refresh();
}

void UserInfo::emit_sale_signals()
{
    emit bankChanged();
    emit merchantChanged();
    emit cashierChanged();
    emit staff_bankChanged();
    emit active_salesChanged();
    emit total_salesChanged();
    emit pending_salesChanged();
    emit cash_receivedChanged();
    emit credit_receivedChanged();
    emit cash_tenderedChanged();
}

void UserInfo::refresh()
{
    emit firstNameChanged();
    emit lastNameChanged();
    emit titleChanged();
    emit uidChanged();
    emit levelChanged();
    emit clockedinChanged();
    emit cashedinChanged();

    emit_sale_signals();
}

void UserInfo::clockin()
{
    data.clockin_user( user );
    emit clockedinChanged();
}

int UserInfo::clockout()
{
    int seconds = data.clockout_user( user );
    emit clockedinChanged();
    return seconds;
}

int UserInfo::seconds_on_clock_today()
{
    return user->clocked_seconds_today;
}

void UserInfo::start_staff_bank()
{
    //Sanity check
    if( user->cashedin )
        return;

    user->staff_bank = 1;
    cashin();

    emit staff_bankChanged();

    data.save_user_data( user );
}

void UserInfo::make_cashier(double start_amount)
{
    //Sanity check
    if( user->cashedin )
        return;

    user->bank = start_amount;
    emit bankChanged();

    cashin();

    data.save_user_data( user );
}

void UserInfo::cashin() {
    if( !user->cashedin ) {
        user->cashedin = true;
        user->cash_received = 0.0f;
        user->credit_received = 0.0f;
        user->total_sales = 0.0f;
        user->pending_sales_total = 0.0f;

        user->session = static_cast<int>(time(nullptr));

        refresh();
    }
}

double UserInfo::cashout(double cash_count) {
    double diff = user->bank + user->total_sales - user->credit_received - cash_count;

    if( user->cashedin ) {
        user->cashedin = 0;

        user->bank = 0;
        user->staff_bank = 0;
        user->session = 0;

        emit bankChanged();
        emit cashedinChanged();
        emit staff_bankChanged();

        data.save_user_data( user );
    }

    return diff;
}
