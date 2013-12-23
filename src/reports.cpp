#include "reports.h"
#include "ticketlistmodel.h"
#include "print.h"

Reports::Reports(QObject* parent) : QObject(parent),
    data(Data::Instance())
{
    sale_report = new ListModel(new SaleReportItem(nullptr));
    orders = new ListModel(new TicketListItem());
    employee_sales = new ListModel(new EmpSaleReportItem(nullptr, nullptr));
    item_details = new ListModel(new ItemSaleReportItem(nullptr, nullptr));
    hour_details = new ListModel(new HourSaleReportItem(nullptr));
    dummySale = Factory::EmptySale();
}

Reports::~Reports()
{
    sale_report->clear();
    orders->clear();
    delete orders;
    delete sale_report;
}

void Reports::get_allyears()
{
    sale_report->clear();// = new ListModel(new SaleReportItem(nullptr));

    auto result = data.get_sale_report();

    for( auto& report : result->items ) {
        sale_report->appendRow( new SaleReportItem(std::move(report)) );
    }

    //emit saleReportModelChanged();
}

//Get all the months in the specified year
void Reports::get_year(const int& year)
{
    sale_report->clear();

    auto result = data.get_sale_report(year);

    auto back = new SaleReportItem(nullptr);
    back->label = "Back to All";
    sale_report->appendRow(back);

    for(auto& report : result->items ) {
        sale_report->appendRow( new SaleReportItem(std::move(report)) );
    }
}

void Reports::get_month(const int& year, const int& month)
{
    sale_report->clear();
    auto result = data.get_sale_report(year, month);

    auto back = new SaleReportItem(nullptr);
    back->label = "Back to Year";
    back->year = year;
    sale_report->appendRow(back);

    for( auto& report : result->items ) {
        sale_report->appendRow( new SaleReportItem( std::move(report) ) );
    }
}

void Reports::get_day(const int& year, const int& month, const int& day)
{
    sale_report->clear();
    auto result = data.get_sale_report(year, month, day);

    auto back = new SaleReportItem(nullptr);
    back->label = "Back to Month";
    back->year = year;
    back->month = month;
    sale_report->appendRow(back);

    for( auto& report : result->items ) {
        sale_report->appendRow( new SaleReportItem(std::move(report)) );
    }

}

void Reports::get_sale(const int& saleid)
{
    auto saledata = data.get_sale_info( saleid );
    auto result = data.getSaleOrderReport(saleid);
    orders->clear();
    emit saleOrderModelChanged();
    dummySale->empty();
    dummySale->user_id = saledata->items[0]->property<int>("emp");
    dummySale->user = data.find_user(dummySale->user_id);
    dummySale->id = saledata->items[0]->property<int>("id");

    for( auto& order : result->items ) {
        auto menu_it = data.find_menu_item( order->property<int>("menu_id") );

        if( menu_it ) {
            std::unique_ptr<OrderedItem> oi(new OrderedItem(menu_it, std::move(order)));

            dummySale->addItem(std::move(oi), true);
        }
    }
    dummySale->finalize_unsaved();

    for( const auto& order : dummySale->get_items() ) {
        orders->appendRow(new TicketListItem(order.get()));
        emit saleOrderModelChanged();
    }

}

void Reports::get_today() {
    sale_report->clear();

    struct tm* now = data.now();

    auto result = data.get_sale_report(now->tm_year + 1900, now->tm_mon + 1, now->tm_mday);
    for( auto& report : result->items ) {
        std::cout << report->property<std::string>("label") << std::endl;
        sale_report->appendRow( new SaleReportItem(std::move(report)) );
    }
}

void Reports::report_date_stats(int y = -1, int m = -1, int d = -1)
{
    auto result = data.get_complete_stats(y, m, d);
    if( result->size() ) {
        date_stats = std::move( result->items[0] );
    } else {
        date_stats = nullptr;
    }

    emit dateTotalChanged();
    emit dateCreditTotalChanged();
    emit dateTenderedChanged();
    emit dateCashTotalChanged();
    emit dateSaleCountChanged();

    employee_sales->clear();

    result = data.get_datereport(0, y, m, d);
    if( result->size() )
    {
        for( auto& userstat : result->items ) {
            if( data.find_user(userstat->property<int>("id")) ) {
                employee_sales->appendRow( new EmpSaleReportItem(std::move(userstat), data.find_user(userstat->property<int>("id"))) );
            }
        }
    }

    item_details->clear();
    result = data.get_datereport(1, y, m, d);
    if( result->size() ) {
        for( auto& mitemstat : result->items ) {
            if( data.find_menu_item( mitemstat->property<int>("menu_id") ) ) {
                item_details->appendRow( new ItemSaleReportItem(std::move(mitemstat), data.find_menu_item( mitemstat->property<int>("menu_id"))) );
            }
        }
    }

    hour_details->clear();
    result = data.get_datereport(2, y, m, d);
    if( result->size() ) {
        for( auto& hourstat : result->items ) {
            hour_details->appendRow( new HourSaleReportItem(std::move(hourstat)) );
        }
    }
}

void Reports::print_sale()
{
    Printer::Instance().print( dummySale.get() );
}
