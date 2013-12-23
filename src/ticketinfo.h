#ifndef TICKETINFO_H
#define TICKETINFO_H

#include <QObject>

#include "data.h"
#include "print.h"

class ListModel;

class TicketInfo : public QObject {
    DataHandler&        data;
    PrinterHandler&     printer;
    Sale*               sale    =   nullptr;
    ListModel*          model   =   nullptr;

    Q_OBJECT
    Q_PROPERTY(double subTotal READ subTotal NOTIFY subTotalChanged)
    Q_PROPERTY(double cash_paid READ cash_paid NOTIFY cash_paidChanged)
    Q_PROPERTY(double credit_paid READ credit_paid NOTIFY credit_paidChanged)
    Q_PROPERTY(double Total READ Total NOTIFY TotalChanged)
    Q_PROPERTY(double taxTotal READ taxTotal NOTIFY taxTotalChanged)
    Q_PROPERTY(double owed READ owed NOTIFY owedChanged)
    Q_PROPERTY(int tableID READ tableID NOTIFY tableIDChanged)
    Q_PROPERTY(int saveCount READ saveCount NOTIFY saveCountChanged)
    Q_PROPERTY(int saleId   READ saleId NOTIFY  saleIdChanged)
    Q_PROPERTY(ListModel* ticketListModel READ ticketListModel NOTIFY ticketListModelChanged)

    public:
        TicketInfo(QObject* parent = nullptr);
        virtual ~TicketInfo();

    public:
        inline double subTotal()    { return (sale) ? sale->total - sale->tax: 0.0f; }
        inline double Total()       { return (sale) ? sale->total : -1.0f; }
        inline double taxTotal()    { return (sale) ? sale->tax : -1.0f; }
        inline int tableID()        { return (sale) ? sale->table_number : -1.0f; }
        inline int saveCount()      { return (sale) ? sale->items_before_save : 0; }
        inline int saleId()         { return (sale) ? sale->id : -1; }
        inline double cash_paid()   { return (sale) ? sale->tendered : 0; }
        inline double credit_paid() { return (sale) ? sale->cc_paid : 0; }
        inline double owed()        { return (sale) ? sale->owed : 0; }

        void updateListModel();
        void refresh        ();
        void activate_order (const int& item_id);

        Q_INVOKABLE void Refresh(void)                  { refresh(); }
        Q_INVOKABLE void void_item(const int& item_id, const int& user_id);
        Q_INVOKABLE void delete_item(const int& item_id);
        Q_INVOKABLE void set_sale(const int&);
        Q_INVOKABLE void SetActive(const int& item_id)  { activate_order(item_id); }
        Q_INVOKABLE bool Valid()                        { return (sale) ? true : false; }
        Q_INVOKABLE int  split();

        //! @brief  Un-split a ticket, adding its items back to its parent ticket
        Q_INVOKABLE void unsplit(const int&);
        Q_INVOKABLE void combine(const int&, const int&);

        //! @brief  Transfer orderedItem form one sale to another
        Q_INVOKABLE void transfer(const int&, const int&, const int&);

        //! @brief  Add a price modifier to an item in a sale
        //!         based on the amount entered.
        Q_INVOKABLE void modify_price(const double&);

        //! @brief  Close a ticket/sale
        Q_INVOKABLE void close();

        Q_INVOKABLE void finalize_changes();
        Q_INVOKABLE void discard_changes();
        Q_INVOKABLE int  removed_count()                { return (sale) ? sale->void_count : 0 ; }
        Q_INVOKABLE bool pay_cash(const double& amount);
        Q_INVOKABLE bool pay_credit(const double& amount);
        Q_INVOKABLE void updateListAdd();
        Q_INVOKABLE void print(const int&);

    public:
        ListModel* ticketListModel() { return model; }

    signals:
        void TotalChanged();
        void subTotalChanged();
        void taxTotalChanged();
        void tableIDChanged();
        void ticketListModelChanged();
        void saveCountChanged();
        void saleIdChanged();
        void cash_paidChanged();
        void credit_paidChanged();
        void owedChanged();
};

#endif // TICKETINFO_H
