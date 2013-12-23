#ifndef POS_GUI_TICKET_HEADER
#define POS_GUI_TICKET_HEADER

#if 0
Ticket ticket(sale);
ticket.Refresh();
ticket.total();
ticket.getItemList();
ticket.text();
ticket.addItem();
ticket.delItem();
ticket.assign(sale);
#endif

#include <vector>
#include <memory>

class Sale;
class OrderedItem;
class MenuItem;
class ListModel;

class Ticket {
    Sale* sale = nullptr;
    double total = 0.0f;
    ListModel*  list = nullptr;

    public:
        Ticket(Sale* sale = nullptr);
        ~Ticket() { }

    public:
        void refresh();
        void assign(Sale*);

    public:
        void addItem(OrderedItem*);
        void addItem(MenuItem*);
        void addItem(const int&);

        void delItem(const int&);

    public:
        inline const double& getTotal();

    public:
        ListModel* getList(void);
};

#endif //POS_GUI_TICKET_HEADER
