#ifndef MENU_ITEM_H
#define MENU_ITEM_H

#include <vector>
#include <memory>
#include <string>

class Item;
class MenuHandler;

class MenuItem {
    friend class MenuHandler;

    std::unique_ptr<Item>       payload;
    std::vector<MenuItem*>      subs;

    public:
        int&            id;
        std::string&    name;
        int&            cat;
        int&            menu_id;
        int&            subitem;
        int&            recurring;
        double&         price;
        int&            addon;
        int&            deleted;
        int&            refill;
        int             sub_count = 0;
        /* Reason for the incomplete flag:
            Some Orders(objects that go INSIDE a sale) are incomplete.
            In a sence that they need another order(a subitem) added to
            them to provide them with a price. Items without a price
            do not get added automatically to the sale.
        */
        bool            incomplete = false;

        //MenuItem guarantees that it does not take ownership of its(hierarchial)
        //parent.Also we do not have raw MenuItem pointers running around anywhere
        //to begin with
        MenuItem*       parent = nullptr;

    public:
        MenuItem(void);
        MenuItem(std::unique_ptr<Item>&&);
        MenuItem(MenuItem&&);

    public:
        void add_item(MenuItem*);
        void delete_item(const int&);

        inline const std::vector<MenuItem*>& subitems(void) const {
            return subs;
        }

    public:
        inline static bool isCategory(const std::unique_ptr<MenuItem>& item) {
            return (item->cat < 0);
        }
        inline static bool isCategory(MenuItem* item) {
            return (item->cat < 0);
        }

        inline static bool iscat(MenuItem* item) { return (item->cat < 0); }
        inline static bool iscat(std::unique_ptr<MenuItem>& item) { return (item->cat < 0); }

    public:
        std::unique_ptr<Item> get_data();
};

#endif // MENU_ITEM_H
