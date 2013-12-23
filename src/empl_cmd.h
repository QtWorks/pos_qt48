#ifndef EMPL_CMD_H
#define EMPL_CMD_H

#include "command.h"
#include "result.h"

class CreateEmployeeCmd : public Command { using Command::Command; };
class ReadEmployeeCmd
{
    public:
        int mEmpId = -1;

        ReadEmployeeCmd(int pEmpId = -1) : mEmpId(pEmpId){}
};

class AddEmployeeCmd
{
    public:

    std::string pass;
    int         level = -1;
    std::string name;

    AddEmployeeCmd(const std::string& empname, const std::string& emppass, const int& emplevel) :
        pass( emppass ), level( emplevel ) , name( empname )
    {
    }
};
class RemoveEmployeeCmd
{
    public:

    int id = -1;

    RemoveEmployeeCmd( const int& empid ) : id( empid ) {}
};
class WriteEmployeeCmd
{
    public:
        std::unique_ptr<Item> data;

        WriteEmployeeCmd(std::unique_ptr<Item>&& ed) : data( std::move(ed) ) {}
};
class ClockinEmployeeCmd { public: int id = -1; ClockinEmployeeCmd(int emp_id) : id(emp_id) {} };
class ClockoutEmployeeCmd { public: int id = -1; ClockoutEmployeeCmd(int emp_id) : id(emp_id) {} };
class ReadEmployeeHours {
    public:
    int    uid = -1;
    time_t from;
    time_t to;
    bool range = false;
    int  limit = 0;

    ReadEmployeeHours(int id, time_t& from_clock, time_t to_clock) : uid(id), from(from_clock) , to(to_clock), range(true) {}
    ReadEmployeeHours(int id) : uid(id) {} //all
    ReadEmployeeHours(int id, time_t date)  : uid(id), from(date) {}
    ReadEmployeeHours(int id, int set_limit) : uid(id), limit( set_limit ) {}
};

class ReadEmployeeSaleTotals { public: int emp = -1; int session = -1; ReadEmployeeSaleTotals(int emp_id, int s) : emp(emp_id), session(s) {} };
class ReadEmployeeCurrentSaleTotals { public: int id = -1; int cutoff = 0; ReadEmployeeCurrentSaleTotals(int emp_id, int cut = 0) : id(emp_id), cutoff(cut) {} };


#endif // EMPL_CMD_H
