#ifndef ITEMFACTORY_H
#define ITEMFACTORY_H

#include<memory>
#include "item.h"
#include "menu.h"
#include "sale.h"

template<typename T>
class TemplateFactory
{
    public:
    static std::shared_ptr<T>   Spawn(void) { return std::make_shared<T>(); }
    static std::unique_ptr<T>   Unique(void) { return std::unique_ptr<T>(new T()); }
    static std::unique_ptr<Item> DefineItem(void) { return std::unique_ptr<Item>(new Item()); }
    static T&&  Convert(Item&& pItemDef){ return T(pItemDef); }
};

class Factory
{
    public:
    static std::unique_ptr<Sale> EmptySale() {
        std::unique_ptr<Item> sale(new Item);
        sale->push_property("year",    0);
        sale->push_property("month",   0);
        sale->push_property("day",     0);
        sale->push_property("hour",    0);
        sale->push_property("minute",  0);
        sale->push_property("amt",     double(0.0f));
        sale->push_property("emp",     0);
        sale->push_property("tax",     double(0.0f));
        sale->push_property("table",   0);
        sale->push_property("tender",  double(0.0f));
        sale->push_property("open", 0);
        sale->push_property("id",      -1);
        sale->push_property("item_count", 0);
        sale->push_property("cc_paid", double(0.0f));
        sale->push_property("change", double(0.0f));
        sale->push_property("guests", 0);
        sale->push_property("tip", 0);
        sale->push_property("start_t", "now()");

        return std::unique_ptr<Sale>(new Sale(std::move(sale)));
    }

    //Empty sale with identical bookeeping information to the original
    // IDs are not copied here this is a distinct sale with id of zero
    static std::unique_ptr<Sale>&& shallow_sale_copy(const std::unique_ptr<Sale>& def) {
        sale_uptr sale = Factory::EmptySale();
        sale->user_id = def->user_id;
        sale->table_number = def->table_number;

        return std::move( sale );
    }

    static std::unique_ptr<Sale> shallow_sale_copy(Sale* def) {
        sale_uptr sale = std::move(Factory::EmptySale());
        sale->user_id = def->user_id;
        sale->table_number = def->table_number;

        return sale;
    }
};

#endif // ITEMFACTORY_H
