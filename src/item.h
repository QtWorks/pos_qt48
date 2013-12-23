#ifndef ITEM_HEADER
#define ITEM_HEADER

#include <vector>
#include <algorithm>
#include <iostream>
#include <memory>
#include <utility>
#include <map>
#include <new>

#include "itemtraits.h"

class Item;
typedef std::unique_ptr<Item>   Item_uptr;
typedef std::shared_ptr<Item>   Item_sptr;

class Item {
    public:
        Item();
        Item(Item* parent_ptr);
        Item(std::map<std::string, Trait>&& props);

        virtual ~Item();

    public: //copy ctors
        Item(const Item& copyit) : properties(copyit.properties)
            {}

        Item(Item&& moveit) : unique_id(std::move(moveit.unique_id)), properties(std::move(moveit.properties))
            {
                moveit.properties.clear();
            }

    public:
        Item*   parent = nullptr;
        int     unique_id = -1;

    public:
        //Item Trait hooks
        std::map<std::string, Trait> properties;
        Trait NullTrait = Trait();

        void copy_property(const std::pair<std::string, Trait>& p) {
            Trait copy(p.second);
            properties[p.first] =  copy;
        }

        //Append a new property with checks
        template<typename T> void push_property(const std::string& pToken, const T& pValue)
        {
            if(properties.find(pToken) == properties.end())
                properties[pToken] = pValue;
        }

        //Return reference to existing property value
        template<class E> E& property(const std::string& pToken)
        {
            return properties[pToken].Ref<E>();
        }

        Trait& operator[](const std::string& pToken)
        {
            if(properties.find(pToken) != properties.end()){
                return properties[pToken];
            }
            else {
                return NullTrait;
            }
        }
};

#endif //ITEM_HEADER
