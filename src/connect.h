#ifndef EPOS_CONNECT_HEADER
#define EPOS_CONNECT_HEADER

#include "result.h"
#include "empl_cmd.h"
#include "sale_cmd.h"

enum LogType {
    Void = 0,
    NewSale,
    PaySale,
    NewOrder,
    TransferOrder,
    LogTypeCount
};

class ReadConfigCmd {};

class WriteLogCmd {
public:
    int             type = -1;
    int             user = -1;
    std::string     tag;
    WriteLogCmd(int _type, int _user, const std::string& _tag)
        :   type(_type),
            user(_user),
            tag(_tag) {}
};

class ReportDateCommand {
public:
    int y = -1;
    int m = -1;
    int d = -1;
    ReportDateCommand(int year = -1, int month = -1, int day = -1)
        :   y(year), m(month), d(day) {}
};

class CompleteReportDateCmd :   public ReportDateCommand { using ReportDateCommand::ReportDateCommand; };
class DateReportUserCmd :       public ReportDateCommand { using ReportDateCommand::ReportDateCommand; };
class DateReportItemCmd :       public ReportDateCommand { using ReportDateCommand::ReportDateCommand; };
class DateReportHourCmd :       public ReportDateCommand { using ReportDateCommand::ReportDateCommand; };
/*
class CompleteReportDateCmd {
    public:
    int y = -1;
    int m = -1;
    int d = -1;
    CompleteReportDateCmd(int year = -1, int month = -1, int day = -1):y(year),m(month),d(day){}
};
class DateReportUserCmd {
    public:
    int y = -1;
    int m = -1;
    int d = -1;
    DateReportUserCmd(int year = -1, int month = -1, int day = -1) : y(year), m(month), d(day) {}
};
class DateReportItemCmd {
    public:
    int y = -1;
    int m = -1;
    int d = -1;
    DateReportItemCmd(int year = -1, int month = -1, int day = -1) : y(year), m(month), d(day) {}
};
class DateReportHourCmd {
    public:
    int y = -1;
    int m = -1;
    int d = -1;
    DateReportHourCmd(int year = -1, int month = -1, int day = -1) : y(year), m(month), d(day) {}
};
*/

class WriteEmployeeLoginCmd
{
    public:
        int         mEmpId = -1;
        bool        mLogin = false;

        WriteEmployeeLoginCmd(unsigned pEmpId, bool pLogin) :
             mEmpId(pEmpId), mLogin(pLogin) { }
};

class ReadTableAllCmd {};
class ReadTableCmd
{
    public:
        unsigned mTableId = 0;
        ReadTableCmd(unsigned pTableId) : mTableId(pTableId) { }
};
class WriteTableCmd
{
public:
    int id = -1;
    std::string name;
    WriteTableCmd(const int& _id, const std::string& _name )
        :   id(_id),
            name(_name)
    {}
};

class ReserveTableCmd {
public:
    int         id = -1;
    std::string reserve;
    ReserveTableCmd(const int& _id, const std::string& _rstring)
        :   id(_id),
            reserve(_rstring)
    {}
};

class NewTableCmd
{
public:
    int x = 0;
    int y = 0;
    int w = 0;
    int h = 0;
    int seats = 0;
    std::string name;
};

class ReadMenuCmd {
    public:
    int     item_id = -1;
    ReadMenuCmd(int id = -1)
        :   item_id( id )
    {
    }
};

class WriteNewMenuItemCmd {
    public:
    std::string name;
    int         parent_id = -1;
    WriteNewMenuItemCmd(const int& pid,const std::string& n)
        :   name(n),
            parent_id(pid)
    {
    }
};

class WriteMenuItemCmd
{
    public:
    std::unique_ptr<Item>   data;
    WriteMenuItemCmd(std::unique_ptr<Item>&& data_ptr)
        :   data( std::move(data_ptr) )
    {
    }
};

class MarkMenuItemDeletedCmd
{
public:
    int id       = -1;
    bool deleted = true;
    MarkMenuItemDeletedCmd(const int& _id, const bool del = true)
        : id(_id),
          deleted(del)
    {

    }
};

#endif //EPOS_CONNECT_HEADER
