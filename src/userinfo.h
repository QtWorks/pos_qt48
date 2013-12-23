#ifndef USERINFO_HEADER
#define USERINFO_HEADER

#include<QObject>
#include"data.h"

class UserInfo : public QObject {
    Q_OBJECT
    Q_PROPERTY(QString firstName READ firstName NOTIFY firstNameChanged)
    Q_PROPERTY(QString lastName READ lastName NOTIFY lastNameChanged)
    Q_PROPERTY(QString title READ title NOTIFY titleChanged)
    Q_PROPERTY(int clockedin READ clockedin NOTIFY clockedinChanged)
    Q_PROPERTY(int cashedin READ cashedin NOTIFY cashedinChanged)
    Q_PROPERTY(int uid READ uid NOTIFY uidChanged)
    Q_PROPERTY(int level READ level NOTIFY levelChanged)
    Q_PROPERTY(double bank READ bank NOTIFY bankChanged)
    Q_PROPERTY(int merchant READ merchant NOTIFY merchantChanged)
    Q_PROPERTY(int cashier READ cashier NOTIFY cashierChanged)
    Q_PROPERTY(int staff_bank READ staff_bank NOTIFY staff_bankChanged)
    Q_PROPERTY(int active_sales READ active_sales NOTIFY active_salesChanged)
    Q_PROPERTY(double total_sales READ total_sales NOTIFY total_salesChanged)
    Q_PROPERTY(double pending_sales READ pending_sales NOTIFY pending_salesChanged)
    Q_PROPERTY(double cash_tendered READ cash_tendered NOTIFY cash_tenderedChanged)
    Q_PROPERTY(double cash_received READ cash_received NOTIFY cash_receivedChanged)
    Q_PROPERTY(double credit_received READ credit_received NOTIFY credit_receivedChanged)

    DataHandler&    data;
    User*           user = nullptr;

    public:
        explicit UserInfo(QObject* parent = nullptr);
        virtual ~UserInfo(){}

    public:
        Q_INVOKABLE int     login(const QString&);
        Q_INVOKABLE void    logout();
        Q_INVOKABLE void    clockin();
        Q_INVOKABLE int     clockout();

        Q_INVOKABLE void    cashin();
        Q_INVOKABLE double  cashout(double cash_count);

        Q_INVOKABLE void    start_staff_bank();
        Q_INVOKABLE void    make_cashier(double start_amount);
        Q_INVOKABLE int     seconds_on_clock_today();

        Q_INVOKABLE void    refresh();
    public:
        inline QString  firstName() { return (user) ? QString(user->first_name.c_str()) : QString(""); }
        inline QString  lastName()  { return (user) ? QString(user->last_name.c_str()) : QString(""); }
        inline QString  title()     { return (user) ? QString(user->title.c_str()) : QString(""); }
        inline int      uid()       { return (user) ? user->id : -1; }
        inline int      level()     { return (user) ? user->level : -1; }
        inline int      clockedin() { return (user) ? user->clockedin : -1; }
        inline int      cashedin()  { return (user) ? user->cashedin : -1; }
        inline double   bank()      { return (user) ? user->bank : -1; }
        inline int      merchant()  { return (user) ? user->merchant : -1; }
        inline int      cashier()   { return (user) ? user->cashier : -1; }
        inline int      staff_bank() { return (user) ? user->staff_bank : -1; }
        inline int      active_sales() { return (user) ? user->active_sales : 0; }
        inline double   total_sales() { return (user) ? user->total_sales : -1.0f; }
        inline double   pending_sales()
        {
            if( !user ) return 0.0f;
            if( user->pending_sales_total < 0.01f ) return 0.0f;
            return user->pending_sales_total;
        }
        inline double   cash_received() { return (user) ? (user->total_sales - user->credit_received) : 0.0f; }
        inline double   credit_received() { return (user) ? user->credit_received : -1.0f; }
        inline double   cash_tendered() { return (user) ? (user->cash_received) : 0.0f; }

private:
        void emit_sale_signals();

    signals:
        void firstNameChanged();
        void lastNameChanged();
        void titleChanged();
        void uidChanged();
        void levelChanged();
        void clockedinChanged();
        void cashedinChanged();
        void bankChanged();
        void merchantChanged();
        void cashierChanged();
        void staff_bankChanged();
        void active_salesChanged();
        void total_salesChanged();
        void pending_salesChanged();
        void cash_receivedChanged();
        void credit_receivedChanged();
        void cash_tenderedChanged();
};

#endif //#define USERINFO_HEADER
