#ifndef EPOS_MENU_HEADER_FILE
#define EPOS_MENU_HEADER_FILE

#include <vector>
#include <list>
#include <string>
#include <algorithm>
#include "generics.h"
#include <QtDebug>

#include "menuitem.h"

class Item;
class MenuItem;
typedef std::vector<std::unique_ptr<MenuItem> > umivec;

class MenuHandler {
        std::vector<std::unique_ptr<MenuItem>>  items;
        std::vector<std::unique_ptr<MenuItem>>  deleted_items;
        std::unique_ptr<MenuItem>               blank_item;

    public:
        MenuHandler();
        ~MenuHandler(){}

        //Methods
        inline const umivec::const_iterator noItem() const { return items.cend(); }
        void add_item(std::unique_ptr<MenuItem>&&);
        void delete_item(const int&);

        //! Find item by using its id. Returns MenuItem pointer. Checks deleted items as well
        MenuItem* find_item(const int& id);
        //! Find item by using its id. Returns MenuItem pointer. Only checks items in the main menu
        MenuItem* find_existing_item(const int&);

        inline const std::vector<std::unique_ptr<MenuItem>>&    getData(void) const { return items; }
        inline const std::unique_ptr<MenuItem>&                 get_blank() const   { return blank_item; }
        inline size_t                                           count() const       { return items.size(); }
        inline void                                             clear()             { items.clear(); }
};

#endif //EPOS_MENU_HEADER_FILE

