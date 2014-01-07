#include "data.h"
#include "server/server.h"
#include "connect.h"

std::unique_ptr<Result> DataHandler::get_sale_report()
{
    return dserver->command(new ReadSalesSummaryCmd());
}

std::unique_ptr<Result> DataHandler::get_sale_report(const int& yr)
{
    return dserver->command(new ReadSalesSummaryCmd(yr));
}

std::unique_ptr<Result> DataHandler::get_sale_report(const int& yr, const int& mo)
{
    return dserver->command( new ReadSalesSummaryCmd(yr, mo) );
}

//Specific date summary(all sales for that date) is slightly different from other getSaleReport overloaded functions
std::unique_ptr<Result> DataHandler::get_sale_report(const int& yr, const int& mo, const int& dy)
{
    return dserver->command( new ReadSaleSummaryDateCmd(yr, mo, dy) );
}

std::unique_ptr<Result> DataHandler::getSaleOrderReport(const int& saleid)
{
    return dserver->command(new ReadSaleDataCmd(saleid, 0) );
}

std::unique_ptr<Result> DataHandler::get_salestat_user()
{
    return dserver->command(new ReadSalesStatsUserCmd());
}

std::unique_ptr<Result> DataHandler::get_complete_stats(int year, int month, int day)
{
    return dserver->command(new CompleteReportDateCmd(year, month, day) );
}

std::unique_ptr<Result> DataHandler::get_datereport(int type, int year, int month, int day)
{
    switch( type ) {
    case 0: //User
        return dserver->command(new DateReportUserCmd(year, month, day));
    case 1: //Item
        return dserver->command(new DateReportItemCmd(year, month, day));
    case 2: //Hour
        return dserver->command(new DateReportHourCmd(year, month, day));
    };

    return std::unique_ptr<Result>(nullptr);
}

std::unique_ptr<Result> DataHandler::get_sale_info(const int& saleid)
{
    return dserver->command(new ReadSaleCmd(saleid) );
}


