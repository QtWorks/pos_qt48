#include "ordereditem.h"
#include "menu.h"
#include <ctime>

static int _UNIQUEORDERID_ = 0;

OrderedItem::OrderedItem(const MenuItem* menu_item, std::unique_ptr<Item>&& data_ptr)
    :   definition(menu_item),
        data(std::move(data_ptr))
{
    unique_id = --_UNIQUEORDERID_;

    if( definition ) {
        id      =   definition->id;
        price   =   definition->price;
        total   =   price;
        subitem =   definition->subitem;
        name    =   definition->name;
        menu_id =   definition->id;
        refill  =   definition->refill;
        unique  =   definition->addon;
    }
    if(data) {
        subitem =   ( data->property<int>("modifies") > -1 );
        comment =   ( data->property<int>("menu_id") < 0 );
        name    =   data->property<std::string>("name");
        price   =   data->property<double>("amount");
        total   =   price;
        unique_id = data->property<int>("uid");
    }
}

OrderedItem::OrderedItem(const std::string& text)
    :   definition(nullptr),
        data(new Item)
{
    unique_id = ++_UNIQUEORDERID_;

    data->push_property("name", text.c_str());

    name = text;
}

OrderedItem::OrderedItem()
    :   definition(nullptr),
        data(nullptr)
{

}

OrderedItem::~OrderedItem()
{
    subitems.clear();
}

Item* OrderedItem::to_item() const
{
    Item* item = new Item;

    item->push_property( "name",        name );
    item->push_property( "order_id",    id );
    item->push_property( "price",       price );
    item->push_property( "total",       total );
    item->push_property( "menu_id",     menu_id );
    item->push_property( "sale_id",     sale_id );
    if( parent )
        item->push_property( "modifies",parent->unique_id );
    else
        item->push_property( "modifies",-1 );

    return item;
}

int OrderedItem::addItem(OrderedItem* item)
{
    total += item->total;
    item->parent = this;
    item->sub_id = sub_count++;
    subitems.push_back(item);

    return sub_count;
}

//Clear all subitems. This function does not acess the subitem pointers
void OrderedItem::clear(void)
{
    subitems.clear();
    sub_count = 0;
    total = price;
}

void OrderedItem::delItem(const int& item_id)
{
    try {
        auto item = subitems.at(item_id);

        total -= item->total;
        item->parent = nullptr;
        subitems.erase(subitems.begin() + item_id);
        sub_count--;
        int i = 0;
        for( auto sub : subitems ) { sub->sub_id = i++; }

    }
    catch(...) {
        std::cout << "FATAL ERROR: removing  non-existent item id: " << item_id << "\n";
        exit(-1);
    }

}

int OrderedItem::hasItem(const int& menu_id)
{
    for ( auto sub : subitems ) {
        if( sub->menu_id == menu_id )
            return sub->sub_id;
    }

    return -1;
}
