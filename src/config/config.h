#ifndef POS_CLIENT_CONFIG_HEADER
#define POS_CLIENT_CONFIG_HEADER

#include "../defs.h"
#include"menu.h"
#include"../table.h"
#include <list>

// Table layout
typedef struct epos_floor_struct {
        std::vector<Table*> table;
        int             count;
        void*           data;
} Floor;

// Internal config data
typedef struct pos_client_config_structure {
        //MenuHandler*   menu;
        Floor*  floor;

    std::list<void*>    tdata;
} Client_Config;

extern Client_Config* client_config_load(void);
extern void client_config_free(Client_Config* config);

extern Floor* load_floor_file(const char* doc);
extern void floor_free(Floor* floor);

#endif //#define POS_CLIENT_CONFIG_HEADER

