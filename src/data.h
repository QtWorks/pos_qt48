#ifndef POS_DATA_HEADER
#define POS_DATA_HEADER

#include "connect.h"
#include "generics.h"
#include <vector>
#include <string>
#include <ctime>
#include "server/server.h"
#include "activetablelist.h"
#include "itemfactory.h"
#include <QtDebug>

#include "menu.h"
#include "table.h"
#include "user.h"
#include "sale.h"
#include "ordereditem.h"
#include "print.h"

#include "item.h"

#define Undefined   -1;

class DataHandler;
typedef GenericSingleton<DataHandler>   Data;
typedef std::vector<std::unique_ptr<Item> > uItemVector;
typedef std::vector<std::pair<std::vector<User>::const_iterator, std::unique_ptr<Sale> > > UserSale_pairvec;
typedef std::vector<std::unique_ptr<Table> > uTableVector;
typedef std::vector< std::pair<Table*,Sale*> > tspair_vector;

class DataHandler {

        std::vector<std::unique_ptr<Sale>>  Sales;
        uTableVector                        Tables;
        std::vector<std::unique_ptr<User>>  Users;
        tspair_vector                       table_sale_pairs;
        UserSale_pairvec::const_iterator    active_usesale;
        std::vector<std::unique_ptr<Item> > sale_report;
        std::unique_ptr<Result>            report_result;

        int max_saleid      = -1;
        int max_solditem_id = -1;
        time_t       now_t  = time(0);

    public:
        DataHandler();
        ~DataHandler()
        {
        }

    public:     //Predefined Constant

        const int& GetScreenHeight() const;
        const int& GetScreenWidth() const;
        void SetScreenWidth(int pWidth);
        void SetScreenHeight(int pHeight);
        struct tm* now() const;

    public:     //Data
        inline const std::vector<std::unique_ptr<Sale>>&  getSaleData() const     { return Sales; }
        inline const uTableVector&       getTableData() const    { return Tables; }
        inline const std::vector< std::unique_ptr<User> >& getUserData() const     { return Users; }
        inline const umivec&   getMenuData() const     { return mmenu.getData(); }

        inline uTableVector::const_iterator noTable(void) const { return Tables.end(); }
        inline umivec::const_iterator noMenuItem(void) const { return mmenu.noItem(); }

public: //Logs
        void log_order_new(OrderedItem*);
        void log_order_void(OrderedItem*);
        void log_order_transfer(OrderedItem*, const int&, const int&, const int&);

    public:     //User
        /// @brief  Create a new user
        User*   create_new_user();
        //! @brief  find existing user based on id
        User*   find_user(const int&);
        //! @brief  find existing user based on password
        User*   find_user(const std::string&);
        void    set_active_user(const int&);
        void    set_active_user(User*);
        int     login_user(int);
        void    logout_user(void);
        User*   add_user(std::unique_ptr<Item>&&);
        void    save_user_data(User*);
        void    save_user_data(const int&);
        void    clockin_user(User*);
        int     clockout_user(User*); //returns # of seconds from generated timecard
        void    fill_user_hours(User*);
        void    update_user_sale_totals(User*);
        void    delete_user(const int&);

    public:
        //Tables
        inline Table*       getActiveTable(void) { return active_table; }
        inline int  getActiveTableId(void) const { return (active_table) ? active_table->id : Undefined; }

        inline Table* find_table(const int& tableid) {
            for( const auto& table : Tables ) {
                if( table->id == tableid ) {
                    return table.get();
                }
            }

            return nullptr;
        }

        inline void set_active_table(Table* activate)     { active_table = activate; }
        inline void set_active_table(const int& tableid)  { active_table = find_table( tableid ); }
        void        save_table(const int&);
        void        reserve_table(const int&, const std::string&);

    public:     //Menu
        inline  MenuItem* find_menu_item    (const int& id) { return mmenu.find_item( id ); }
        inline  MenuItem* find_existing_menu_item(const int& id) { return mmenu.find_existing_item(id); }
        int     create_new_menuitem         (const int&,const std::string&);
        void    save_menuitem               (const int&);
        void    delete_menuitem             (const int&);
        void    reset_menu                  (void);
        inline const std::unique_ptr<MenuItem>& blankMenuItem(void) const
        {
            return mmenu.get_blank();
        }
        /// @brief Checks if a menuitem is currently used inside the system.
        /// @param menuid Id of the item to be checked.
        /// @return true if item is inside the system. false otherwise.
        bool    menuitem_isused             (const int& menuid);

    public:     //Sales
        //! Get currently viewed sale id
        int     get_active_sale_id(void) const;
        //! Get currently viewed sale
        Sale*   get_active_sale() const;
        //! Find a sale at a table_id
        Sale*   find_sale_at_table(const int& table_id) const;
        //! Set the currently viewed sale
        Sale*   set_active_sale(const int& sale_id);
        //! Find a sale based on its id
        Sale*   find_sale(int);

        //New sales
        //void    write_sale(Sale*, std::vector<std::unique_ptr<OrderedItem>>&, bool); //Write sale
        //void    write_sale(Sale*, std::vector<OrderedItem*>&,bool); //Overwrite existing sale
        void    write_sale(Sale*, bool);
        void    save_sale(const int& sale_id, bool isnew = false); //Save sale data
        void    write_new_sale(Sale*);

        void    attach_sale(Sale*);
        void    detach_sale(Sale*);

        int     new_sale(int table_id);
        int     add_sale(std::unique_ptr<Sale>&&);
        void    remove_sale(Sale*);
        void    erase_sale(int);
        void    finalize_sale(const int& sale_id);
        void    close_sale(const int&);
        bool    pay_sale(const int&,const double&);
        bool    pay_sale(Sale*,     const double&);
        bool    pay_sale_credit(Sale*,const double&);
        void    void_sale(const int& sale_id, const int& emp_id);

        void    sale_transfer_item(Sale*,Sale*,OrderedItem*);
        void    sale_combine(Sale*, Sale*);

        //Sale orderitems
        void    save_ordereditem(OrderedItem* order);
        void    void_ordereditem(OrderedItem* order);
        void    void_ordereditem(Sale*, const int&);

        std::unique_ptr<Result> get_sale_info(const int&);

    public:
        //Reports
        std::unique_ptr<Result>  getSaleOrderReport(const int&);
        //const std::vector<std::unique_ptr<Item> >& getSaleReport();
        std::unique_ptr<Result> get_sale_report();
        std::unique_ptr<Result> get_sale_report(const int&);
        std::unique_ptr<Result> get_sale_report(const int&,const int&);
        std::unique_ptr<Result> get_sale_report(const int&,const int&, const int&);

        std::unique_ptr<Result> get_salestat_user();
        std::unique_ptr<Result> get_complete_stats(int year = -1, int month = -1, int day = -1);
        std::unique_ptr<Result> get_datereport(int type, int year = -1, int month = -1, int day = -1);

        const std::vector<std::unique_ptr<Item> >& getSaleReport(const int&);
        const std::vector<std::unique_ptr<Item> >& getSaleReport(const int&,const  int&);
        const std::vector<std::unique_ptr<Item> >& getSaleReport(const int&,const  int&,const  int&);

    public:
        Table*          active_table    = nullptr;
        User*           active_user     = nullptr;
        Sale*           active_sale     = nullptr;
        int             mScreenHeight;
        int             mScreenWidth;

    private:
        DataServer*     dserver;
        MenuHandler     mmenu;
        PrinterHandler  &printer;
};

#endif

