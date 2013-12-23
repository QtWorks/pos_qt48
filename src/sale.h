#ifndef SALE_HEADER_FILE
#define SALE_HEADER_FILE

#include <memory>
#include <vector>

class OrderedItem;
class MenuItem;
class Item;
class Time;
class Table;
class User;

//Sale class
class Sale {
        //Private data
        std::unique_ptr<Item> payload; //Sale item owns its own data;
        std::vector<std::unique_ptr<OrderedItem> > items;
        std::vector<OrderedItem*> voids;   //!< Voids are: Items inside DB marked for removal
        std::vector<int> removed;
        std::vector<OrderedItem*> unsaved;

        //Selected item TODO: make public?
        OrderedItem*          active_order  = nullptr;

        int         add_abstract(const std::unique_ptr<OrderedItem>&);
        int         add_concrete(const std::unique_ptr<OrderedItem>&);
        void        delete_order(const int&);
        //! Internal bookkeeping
        void        refresh_ids(void);

    public:
        //General Information
        //! NOTE: references are bindings to payload data. Initialized in ctor
        int         id = -1;
        double&     total;
        double&     tendered;
        double&     change;
        double&     tax;
        double&     cc_paid;
        double      tax_percent = 0.06f;
        double      sub_total   = 0.0f;
        double      owed = 0.0f;
        int&        open;
        int&        item_count;
        int&        table_number;
        int&        user_id;
        int&        session;

        std::string start_t; //!<   moment the sale is created. Returned by the DB

        bool        editable = false;
        int         items_before_save = 0;
        int         remove_count  = 0;
        int         void_count = 0;

    public:

        Table*       table      = nullptr; //!< Convenience pointer. non-owned, read-only
        User*        user       = nullptr; //!< Convenience pointer, non-owned, read-only

    public:
        //! Can only be created with previously existend data
        //! The functions that pull data from DB have no access Sale class
        Sale(std::unique_ptr<Item>&& data);
        ~Sale();

        //! Setting iscounted = true will add OrderedItem price information to the sale
        //! Add a previously constructed item
        OrderedItem* addItem(std::unique_ptr<OrderedItem>&& item, bool iscounted = false);

        //! Add from a menu definition. Creates a new OrderItem
        OrderedItem* addItem(const MenuItem*, bool iscounted = false);

        //! Add a comment to the active item
        void add_comment(const std::string& comment, const std::unique_ptr<MenuItem>& blank);
        //! @brief  Add a comment to the active item
        void add_comment(std::string comment);

        //! @brief  Add a price modifier to active item
        void add_price_modifier(const double&);

        //! Get an existing item from within a sale
        const std::unique_ptr<OrderedItem>& getItem(const int& item_id);

        //! Get an existing item pointer from within a sale
        OrderedItem* get_order_ptr(const int&) const;

        //! Delete item from within a sale
        void delItem(const int&);

        //! @brief  Flag an item for voiding
        void void_item(const int&, const int&);

        //! Empty out the ticket
        void empty(void);

        //! Clear list of removed items
        void reset_removed();

        //! Basically current item being viewed/editted
        void selectItem(const int&);

        //! Items getting added, DO NOT remain in the sale class unless saved
        //! think 'edit mode' where you can void/add on demand
        void discard_unsaved();
        void discard_voids();
        void discard_changes();

        //! Items that are saved(or finalized) remain in the sale class, thus the system.
        //! This should ALWAYS be called when the sale is written to DB. Always.
        void finalize_unsaved();
        void finalize_voids();
        void finalize_changes();

        //! Payment functions. Simple total updates etc.,
        void pay_cash(const double&);
        void pay_credit(const double&);

        /// @brief Check if the sale holds a menuitem.
        /// @param id MenuItem id to be checked.
        /// @return true if item is inside sale. false otherwise.
        bool hasitem(const int& id) const;

    public:
        //Item data whitin the sale.
        inline std::vector<std::unique_ptr<OrderedItem> >& get_items(void) { return items; }
        inline std::vector<OrderedItem*>& get_unsaved_items(void) { return unsaved; }
        inline std::vector<int>& get_removed_items(void) { return removed; }
        std::vector< OrderedItem* >& get_voids(void) { return voids; }

        //Returns a unique item that describes the sale. Return item is a copy
        std::unique_ptr<Item> flatten(void) const;
};

typedef std::shared_ptr<Sale> sale_sptr;
typedef std::unique_ptr<Sale> sale_uptr;

#endif //#define SALE_HEADER_FILE
