#ifndef POS_EMPLOYEE_HEADER
#define POS_EMPLOYEE_HEADER

#include <list>

typedef struct pos_employee_struct {
        int     id;
        int     in;
        int     pass;
        int     lvl;
        int     tbc; //table count
        std::list<Table*>  tables;
/*        Evas_Object*    order;
        Evas_Object*    clock;
        Eina_Strbuf*    first;
        Eina_Strbuf*    last;
        Eina_Strbuf*    title;*/
} Employee;

#endif //#define POS_EMPLOYEE_HEADER
