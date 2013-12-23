#include "item.h"

static inline int UNIQUEITEMID() { static int _unique_ids = 0; return (++_unique_ids); }

Item::Item() : unique_id( UNIQUEITEMID() )
{
}

Item::Item(Item* parent_ptr) : parent( parent_ptr) , unique_id( UNIQUEITEMID() )
{
}

Item::Item(std::map<std::string, Trait>&& props) : unique_id( UNIQUEITEMID() )
{
    properties = std::move(props);
    props.clear();
}

Item::~Item()
{
    properties.clear();
}

