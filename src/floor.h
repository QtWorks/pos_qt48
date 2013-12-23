#ifndef FLOOR_H
#define FLOOR_H

#include <vector>

// Table layout
struct Floor {
        std::vector<NetDataTableInfo*> table;
        int             count;
        void*           data;
};

#endif // FLOOR_H
