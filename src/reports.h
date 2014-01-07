#ifndef REPORTS_H
#define REPORTS_H

#include <QObject>
#include "data.h"
#include "listmodel.h"

class Reports;

class EmpSaleReportItem : public ListItem
{
    friend class Reports;
    QHash<int, QByteArray>  roles;
    User*                   user;
    std::unique_ptr<Item>   item;

    void set_roles();

    public:
    EmpSaleReportItem(std::unique_ptr<Item>&&, User*);
    QHash<int, QByteArray> roleNames() const { return roles; }
    QString id() const { return QString("Undefined"); }

    enum Roles {
        IdRole = Qt::UserRole + 1,
        FirstNameRole,
        LastNameRole,
        TotalRole,
        SaleCountRole,
        RoleCount
    };

    QVariant data(int) const;
};

class ItemSaleReportItem : public ListItem
{
    QHash<int, QByteArray>  roles;
    MenuItem*               menu_item;
    std::unique_ptr<Item>   item;

    void set_roles();

    public:
    ItemSaleReportItem(std::unique_ptr<Item>&&, MenuItem*);
    QHash<int, QByteArray> roleNames() const { return roles; }
    QString id() const { return QString("Undefined"); }

    enum Roles {
        IdRole = Qt::UserRole + 1,
        NameRole,
        TotalRole,
        CountRole,
        RoleCount
    };
    QVariant data(int) const;
};

class HourSaleReportItem : public ListItem
{
    QHash<int, QByteArray>  roles;
    std::unique_ptr<Item>   item;

    void set_roles();

    public:
    HourSaleReportItem(std::unique_ptr<Item>&&);
    QHash<int, QByteArray> roleNames() const { return roles; }
    QString id() const { return QString("undefined"); }

    enum Roles {
        NameRole = Qt::UserRole + 1,
        TotalRole,
        CountRole,
        RoleCount
    };
    QVariant data(int) const;
};

class SaleReportItem : public ListItem
{
    friend class Reports;
    std::unique_ptr<Item>   item;
    QHash<int, QByteArray>  mRoles;
    std::string             label;
    double                  amt = 0.0f;
    double                  tax = 0.0f;
    int                     year = 0;
    int                     month = 0;
    int                     day = 0;
    int                     item_count = 0;
    int                     sid = -1;
    int                     emp = 0;
    int                     table_num = 0;
    int                     sale_count = 0;

    public:
        SaleReportItem(std::unique_ptr<Item>&& data_ptr);
        ~SaleReportItem();

    public:
        QHash<int, QByteArray> roleNames() const
        {
            return mRoles;
        }

        QString id() const {
            return QString("Undefined");
        }

    public:

        enum Roles {
            IdRole = Qt::UserRole + 1,
            LabelRole,
            TotalRole,
            AmountRole,
            YearRole,
            MonthRole,
            HourRole,
            MinuteRole,
            DayRole,
            ItemCountRole,
            EmpRole,
            TableRole,
            SaleCount,
            RoleCount
        };

    public:

        QVariant data(int) const;

        private:
            void setRoles();
};

class ETimeCardItem : public ListItem {
public:
    QHash<int, QByteArray> mRoles;
    int         start_epoch;
    int         end_epoch;
    std::string start_time;
    std::string end_time;
    int         seconds;

    ETimeCardItem(Item*);

    QHash<int, QByteArray> roleNames() const { return mRoles; }
    QString id() const { return QString("undefined"); }

    enum Roles {
        StartEpochRole = Qt::UserRole + 1,
        EndEpochRole,
        StartTimeRole,
        EndTimeRole,
        SecondsRole,
        RoleCount
    };

    QVariant data(int) const;

private:
    void setRoles();
};

//!Class responsible for fetching sale data and structuring report datag
class Reports : public QObject {
    DataHandler&    data;
    ListModel*      sale_report;
    ListModel*      orders;
    ListModel*      employee_sales;
    ListModel*      item_details;
    ListModel*      hour_details;
    ListModel*      employee_timecards;

    //Used to display picked sale
    std::unique_ptr<Sale> dummySale;

    //Current selected dates stats
    std::unique_ptr<Item>   date_stats;

    Q_OBJECT
    //Report lists
    Q_PROPERTY(ListModel* saleReportModel READ saleReportModel NOTIFY saleReportModelChanged)
    Q_PROPERTY(ListModel* saleOrderModel READ saleOrderModel NOTIFY saleOrderModelChanged)
    Q_PROPERTY(ListModel* employeeSaleModel READ employeeSaleModel NOTIFY employeeSaleModelChanged)
    Q_PROPERTY(ListModel* itemDetailSaleModel READ itemDetailSaleModel NOTIFY itemDetailSaleModelChanged)
    Q_PROPERTY(ListModel* hourDetailSaleModel READ hourDetailSaleModel NOTIFY hourDetailSaleModelChanged)
    Q_PROPERTY(ListModel* empTimecardModel  READ empTimecardModel NOTIFY empTimecardModelChanged)

    //Selected date totals
    Q_PROPERTY(double dateTotal READ dateTotal NOTIFY dateTotalChanged)
    Q_PROPERTY(double dateCreditTotal READ dateCreditTotal NOTIFY dateCreditTotalChanged)
    Q_PROPERTY(double dateTendered READ dateTendered NOTIFY dateTenderedChanged)
    Q_PROPERTY(double dateCashTotal READ dateCashTotal NOTIFY dateCashTotalChanged)
    Q_PROPERTY(int dateSaleCount READ dateSaleCount NOTIFY dateSaleCountChanged)
    Q_PROPERTY(QString sale_username READ sale_username NOTIFY sale_usernameChanged)

    public:
        Reports(QObject* parent = nullptr);
        virtual ~Reports();

    public:
        //These functions update the ListModels shown in sale reports
        Q_INVOKABLE void get_allyears();
        Q_INVOKABLE void get_year(const int&);
        Q_INVOKABLE void get_month(const int&, const int&);
        Q_INVOKABLE void get_day(const int&, const int&, const int&);
        Q_INVOKABLE void get_sale(const int&);
        Q_INVOKABLE void get_emp_hours(const int&);
        Q_INVOKABLE void get_today();
        Q_INVOKABLE void report_date_stats(int,int,int);
        Q_INVOKABLE void print_sale();

    public:
        inline ListModel* saleReportModel() { return sale_report; }
        inline ListModel* saleOrderModel() { return orders; }
        inline ListModel* employeeSaleModel() { return employee_sales; }
        inline ListModel* itemDetailSaleModel() { return item_details; }
        inline ListModel* hourDetailSaleModel() { return hour_details; }
        inline ListModel* empTimecardModel() { return employee_timecards; }

        inline double   dateTotal()         { return (date_stats) ? date_stats->property<double>("total_sales") : 0; }
        inline double   dateCreditTotal()   { return (date_stats) ? date_stats->property<double>("credit_charges") : 0; }
        inline double   dateTendered()      { return (date_stats) ? date_stats->property<double>("cash_tendered") : 0; }
        inline double   dateCashTotal()     { return (date_stats) ? date_stats->property<double>("cash_total") : 0; }
        inline int      dateSaleCount()     { return (date_stats) ? date_stats->property<int>("sale_count") : 0; }
        QString         sale_username()     { return (dummySale->user) ? QString(dummySale->user->first_name.c_str()) : QString("undefined"); }

    signals:
        void saleReportModelChanged();
        void saleOrderModelChanged();
        void employeeSaleModelChanged();
        void itemDetailSaleModelChanged();
        void hourDetailSaleModelChanged();
        void empTimecardModelChanged();

        void dateTotalChanged();
        void dateCreditTotalChanged();
        void dateTenderedChanged();
        void dateSaleCountChanged();
        void dateCashTotalChanged();
        void sale_usernameChanged();
};

#endif // REPORTS_H
