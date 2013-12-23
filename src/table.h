#ifndef EPOS_TABLE_HEADER
#define EPOS_TABLE_HEADER

#include <memory>
#include <vector>
#include <string>

class Item;
class User;
class Sale;

class Table {
     std::unique_ptr<Item>  payload;
     std::vector<Sale*>     sales;

     public:
        int&        id;
        int&        seats;
        int         sale_id = -1;
        int&        user_id;
        int&        x;
        int&        y;
        int&        w;
        int&        h;

        Sale*       sale = nullptr;
        User*       user = nullptr;

        int         sale_count = 0;

        std::string name;
        std::string reserve;
     public:
        Table(std::unique_ptr<Item>&& data);

        inline bool isOpen(void) const { return (user_id > -1); }

        void attach_sale(Sale*);
        void detach_sale(Sale*);

        inline const std::vector<Sale*>& get_sales() const { return sales; }
};

#endif //EPOS_TABLE_HEADER

