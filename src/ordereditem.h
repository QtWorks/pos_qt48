#ifndef ORDEREDITEM_HEADER
#define ORDEREDITEM_HEADER

#include "item.h"
#include <vector>
#include <memory>

class MenuItem;

class OrderedItem
{
    public:
        const MenuItem*          definition;
        std::unique_ptr<Item>    data;

        int             id;
        double          price       = 0.0f;
        double          total       = 0.0f;
        bool            subitem     = false;
        bool            unique      = false;
        bool            final       = false;
        bool            added       = false;
        bool            comment     = false;
        int             void_flag   = 0;
        std::string     name;
        int             sub_count   = 0;
        int             menu_id     = -1;
        int             sub_id      = -1;
        int             display_id  = -1;
        int             unique_id   = -1;
        int             sale_id     = -1;
        int             refill      = 0;
        int             quantity    = 1;
        int             hold_flag   = 0;
        OrderedItem* parent = nullptr;

        std::vector<OrderedItem*>   subitems;

    public:
        OrderedItem(const MenuItem*,std::unique_ptr<Item>&&);
        OrderedItem(const std::string&);
        OrderedItem();
        ~OrderedItem();

    public:
        int  addItem(OrderedItem* item);
        void delItem(const int& item_id);
        void clear(void);
        int hasItem(const int& item_id);

        //! @brief  Returns wheter the item is abstract. Has no menu definition
        //!         aka is a comment or modifier
        bool abstract() const { return (definition == nullptr); }


        //! @brief  Return an Item reperesentation of self.
        Item*   to_item()   const;
};

#endif //ORDEREDITEM_HEADER
