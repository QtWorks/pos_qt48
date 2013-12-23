#ifndef RESULT_H
#define RESULT_H

#include "defs.h"
#include "item.h"
#include <memory>
#include <ctime>

class Result {
    public:
        std::vector<std::unique_ptr<Item> >  items;

        inline size_t size() { return items.size(); }
        inline void clear() { items.clear(); }

        inline void push_back(std::unique_ptr<Item>&& item) {
            items.push_back( std::move(item) );
        }

        inline void push_back(Item* item_ptr) {
            items.emplace_back( item_ptr );
        }

        inline const std::unique_ptr<Item>& item(const std::size_t& id) const {
            return items.at( id );
        }
};

#endif // RESULT_H
