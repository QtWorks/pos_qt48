#ifndef SALE_CMD_H
#define SALE_CMD_H

#include "result.h"
#include "command.h"

class ReadSaleCmd :             public Command { using Command::Command; };
class ReadSalePaymentCmd :      public Command { using Command::Command; };
class ReadOpenSalesAllCmd :     public Command { using Command::Command; };
class DeleteOrderCmd :          public Command { using Command::Command; };
class DeleteAllSaleOrdersCmd :  public Command { using Command::Command; };
class DeleteSaleCmd:            public Command { using Command::Command; };

class ReadOpenSalesEmpCmd { public: int mEmpId; ReadOpenSalesEmpCmd(int pEmp_Id) : mEmpId(pEmp_Id) {} };
class ReadSalesSummaryCmd
{
    public:
        int year = 0;
        int month = 0;
        int day = 0;
        std::string dist;

        ReadSalesSummaryCmd()   : dist("year") {}
        ReadSalesSummaryCmd(int yr) : year(yr), dist("month") {}
        ReadSalesSummaryCmd(int yr, int mo) : year(yr), month(mo), dist("day") {}
        ReadSalesSummaryCmd(int yr, int mo, int dy) : year(yr), month(mo), day(dy), dist("hour") {}
        ~ReadSalesSummaryCmd() { using std::string; dist.~string(); }
};

class ReadSaleSummaryDateCmd {
public:
    int year = 0;
    int month = 0;
    int day = 0;
    ReadSaleSummaryDateCmd(const int& yr,const int& mo, const int& dy) : year(yr), month(mo), day(dy) {}
};

class ReadSalesStatsUserCmd {
    public:
    int user = -1;
    ReadSalesStatsUserCmd(int eid = -1) : user(eid) {}
};

class ReadSalesDetailsDateCmd {
public:
    int y = -1;
    int m = -1;
    int d = -1;
    int val = -1;
    std::string selection;

    ReadSalesDetailsDateCmd(int year = -1, int month = -1, int day = -1) :
        y(year), m(month), d(day)
    {
        if( y > 0 ) {
            selection = "year";
            val = y;
        }
        if( m > 0 ) {
            selection = "month";
            val = m;
        }
        if( d > 0 ) {
            selection = "day";
            val = d;
        }
    }
};

class ReadSaleDataCmd
{
public:
    ReadSaleDataCmd(unsigned pSaleId, unsigned pItemCount)
    {
        mItemCount = pItemCount;
        mSaleId = pSaleId;
    }

public:
    unsigned    mItemCount;
    unsigned    mSaleId;
};

class WriteSaleCmd
{
public:
    std::unique_ptr<Item>   info;
    bool                    isnew = false;

    WriteSaleCmd(std::unique_ptr<Item>&& info_ptr, const bool& _isnew )
        :   info(std::move(info_ptr)),
            isnew( _isnew )
    { }
};

class WriteOrderCmd
{
public:
    double  amount = 0.0f;

    int         sale_id = -1;
    int         menu_id = -1;
    int         order_id = -1;
    int         modifies = -1;
    int         quantity = 1;
    std::string start_t;
    std::string name;

    WriteOrderCmd(const std::string& _start_t, const std::unique_ptr<Item>& def)
        :   amount(def->property<double>("price")),     //It's amount but there is a name mismatch(price)
            sale_id(def->property<int>("sale_id")),
            menu_id(def->property<int>("menu_id")),
            order_id(def->property<int>("order_id")),
            modifies(def->property<int>("modifies")),
            quantity(def->property<int>("quantity")),
            start_t(_start_t),
            name(def->property<std::string>("name"))
    {
    }
    WriteOrderCmd() {}
};

class MoveOrderCmd :     public MoveCommand { public: using MoveCommand::MoveCommand; };
class WriteOrderModCmd : public MoveCommand { public: using MoveCommand::MoveCommand; };

class InsertOrderCmd
{
public:
    std::string name;
    double  amount = 0.0f;
    int     sale_id = -1;
    int     menu_id = -1;
    int     order_id = -1;
    int     modifies = -1;
    double  multiply = 0.0f;
};

class AddPaymentCmd
{
public:
    int         sale_id;
    double      amount;
    std::string type;
    AddPaymentCmd(int sid, double amt, const std::string& t)
    :  sale_id(sid), amount(amt), type(t) {}
};

#endif // SALE_CMD_H
