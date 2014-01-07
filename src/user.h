#ifndef USER_HEADER_FILE
#define USER_HEADER_FILE

#include <string>
#include <iostream>
#include <memory>
#include <vector>

#include "timehelp.h"

class Item;
class Sale;

class Session {
    public:

    std::string type;
    std::string name;
    int         user_id;
    int         id;
    double     start_amount     = 0.0f;
    double     total_sales      = 0.0f;
    double     tendered_cash    = 0.0f;
    double     short_total     = 0.0f;
    double     credit_total     = 0.0f;
    double     cash_count       = 0.0f;

    time_t      start_time;
    time_t      end_time;
};

class User {
    std::unique_ptr<Item>   payload;
    std::vector< std::unique_ptr<Item> > hv;
    std::vector< Sale* >      sales; //Sales owned by user

    public:
        std::string&    first_name;
        std::string&    last_name;
        std::string&    title;
        int             id;
        int&            level;
        int&            clockedin;
        int&            session;
        int&            cashedin;   //holds the id of the first sale created for this session
        int&            staff_bank;
        std::string&    pass;
        double&         bank;
        int&            merchant;
        int&            cashier;
        double&         cash_received;
        double&         credit_received;
        double&         total_sales;
        double&         total_change;

        double          pending_sales_total = 0.0f;  //Amount to be collected by/from user
        double          expected_cash = 0.0f; //Amount of cash expected from user
        double          credit_tip_total = 0.0f; //Credit tip total

        int             active_sales = 0;

        Time            today;

        int             clocked_seconds_today = 0;
        int             clocked_seconds_total = 0;

        Session         session_info;

        struct Timecard {
           std::string  start;
           std::string  end;
           std::string  label;
           int          end_e;
           int          start_e;
           int          tdsec;    //seconds today
           int          tlsec;    //secons total
        };
        Timecard        timecard;

        User(std::unique_ptr<Item>&& data);
        User(User&& moved_user) : User(std::move(moved_user.payload)) { }
        ~User();

        void attach_sale(Sale* sale_ptr);
        void detach_sale(Sale* sale_ptr);

        void add_timecard(std::unique_ptr<Item>&& timecard);
        void clear_all_timecard();
        int  seconds_timed_total();

        inline bool checkPassword(const std::string& password) const
        {
            return  (password == pass);
        }

        std::unique_ptr<Item>   get_data(void);
        std::vector<Item*>      get_timecards();
};

#endif //#define USER_HEADER_FILE
