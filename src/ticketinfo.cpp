#include "ticketinfo.h"
#include "ticketlistmodel.h"
#include "salelistitem.h"

TicketInfo::TicketInfo(QObject* parent) :
    QObject(parent),
    data(Data::Instance()),
    printer(Printer::Instance())
{
    model = new ListModel(new TicketListItem());
    all_sales = std::unique_ptr<ListModel>(new ListModel(new SaleListItem()));
}

TicketInfo::~TicketInfo()
{
    delete model;
    model = nullptr;
}

double TicketInfo::subTotal()    { return (sale) ? sale->total - sale->tax: 0.0f; }
double TicketInfo::Total()       { return (sale) ? sale->total : -1.0f; }
double TicketInfo::taxTotal()    { return (sale) ? sale->tax : -1.0f; }
int TicketInfo::tableID()        { return (sale) ? sale->table_number : -1; }
int TicketInfo::saveCount()      { return (sale) ? sale->items_before_save : 0; }
int TicketInfo::saleId()         { return (sale) ? sale->id : -1; }
double TicketInfo::cash_paid()   { return (sale) ? sale->tendered : 0; }
double TicketInfo::credit_paid() { return (sale) ? sale->cc_paid : 0; }
double TicketInfo::owed()
{
    if( sale ) {
        return (sale->owed);
    }
    return 0;
}
int  TicketInfo::guests()        { return (sale) ? sale->guests : -1; }
double TicketInfo::tip_percent() { return (sale) ? sale->tip_percent : 0.0f; }

void TicketInfo::activate_order(const int& item_id)
{
    sale->selectItem(item_id);
}

void TicketInfo::updateListModel()
{
    model->clear();

    if( !sale ) {
        return;
    }
    for(const auto& order : sale->get_items()) {
        order->added = true;
        model->appendRow(new TicketListItem(order.get()));
    }
}

void TicketInfo::updateListAdd()
{

    for(const auto& order : sale->get_items() ){
        if( !order->added ) {
            order->added = true;
            if( order->subitem ) {
                for( int i = 0; i < model->count(); i++) {
                    TicketListItem* ptr = (TicketListItem*)model->m_list.at(i);
                    if( ptr->order->id == order->parent->id ) {
                        model->insertRow(i+order->parent->sub_count, new TicketListItem(order.get()));
                        break;
                    }
                }
            }
            else {
                model->appendRow(new TicketListItem(order.get()));
            }
        }
    }

    emit model->dataChanged(model->index(0),model->index(model->count()-1));
    emit ticketListModelChanged();
    emit subTotalChanged();
    emit taxTotalChanged();
    emit TotalChanged();
}

void TicketInfo::refresh(void)
{
    //updateListAdd();
    if( sale ) {
        updateListModel();
    } else {
        model->clear();
    }
    emit ticketListModelChanged();
    emit subTotalChanged();
    emit taxTotalChanged();
    emit TotalChanged();
    emit saleIdChanged();
    emit credit_paidChanged();
    emit cash_paidChanged();
    emit owedChanged();
    emit guestsChanged();
    emit allSaleModelChanged();
    emit tip_percentChanged();
}

void TicketInfo::void_item(const int& item_id, const int& user_id)
{
    if ( sale ) {
        sale->void_item(item_id, user_id);
        emit owedChanged();
        emit subTotalChanged();
        emit TotalChanged();
    }
}

void TicketInfo::delete_item(const int& item_id)
{
    if( sale ) {
        sale->delItem( item_id );
        updateListModel();
        //refresh();
        /*for( const auto& rid : sale->get_removed_items() ) {
            for(int i = 0; i < model->count(); i++){
                TicketListItem* ptr = (TicketListItem*)model->m_list.at(i);
                if( ptr->unique_id == rid ) {
                    model->removeRow( i );
                    break;
                }
            }
        }*/
        /*if( sale->remove_count ) {
            for( const auto& removed : sale->get_removed_items() ) {
                for(int i = 0; i < model->count(); i++) {
                    //Walk model, remove voided rows
                    TicketListItem* ptr = (TicketListItem*)model->m_list.at(i);
                    if( ptr->unique_id == removed ) {
                        model->removeRow( i );
                        break;
                    }
                }
            }

            sale->reset_removed();
        }*/

        //emit model->dataChanged(model->index(0),model->index(model->count()-1));
        emit ticketListModelChanged();
        emit owedChanged();
        emit subTotalChanged();
        emit TotalChanged();
    }
}

void TicketInfo::set_sale(const int& sale_id)
{
    sale = data.set_active_sale(sale_id);
    if( sale ) {
        refresh();
    } else {
        model->clear();
    }
}

void TicketInfo::modify_price(const double& amount)
{
    if( sale ) sale->add_price_modifier( amount );
}

// Returns the sale index;
int TicketInfo::split( )
{
    //int id = data.new_sale(sale->table_number);
    return data.new_sale(sale->table_number);
}

void TicketInfo::unsplit(const int& splitid)
{
    Sale* removed_sale = data.find_sale( splitid );
    int i = removed_sale->item_count;

    while( i ) {
        const auto& sale_data = removed_sale->get_items();

        if( !sale_data[i - 1]->subitem ) {
            transfer( i - 1, sale->id, removed_sale->id );
        }
        i = removed_sale->item_count;
    }

    data.pay_sale(removed_sale, 0.0f, false);
}

void TicketInfo::combine( const int& to_id, const int& from_id )
{
    Sale* to = data.find_sale( to_id );
    Sale* from = data.find_sale( from_id );

    if( !to || !from ) return;

    int i = from->item_count;

    while( i ) {
        const auto& sale_data = from->get_items();
        if( !sale_data[i - 1]->subitem ) {
            transfer( i - 1, to_id, from_id );
        }
        i--;// = from->item_count;
    }

    //The from Ticket is erased so drop its pointer if
    //still holding it.
    if( sale && sale->id == from_id ) {
        sale = nullptr;
    }
    data.pay_sale(from, 0, false);
    data.erase_sale( from_id );
}

void TicketInfo::transfer(const int& item_idx, const int& to_id, const int& from_id)
{
   Sale* to = data.find_sale( to_id );
   if( !to ) return;
   Sale* from = data.find_sale( from_id );
   if( !from ) return;

   data.sale_transfer_item( to, from, from->get_items()[item_idx].get() );
}

void TicketInfo::finalize_changes()
{
    if( sale ) {
        data.save_sale( sale->id );
    }
}

void TicketInfo::discard_changes()
{
    if(sale) {
        sale->discard_changes();
    }
}

void TicketInfo::close()
{
    data.close_sale( sale->id );
    sale = nullptr;
}

bool TicketInfo::pay_cash(const double& amount, bool print)
{
    bool result = data.pay_sale( sale, amount, print );
    if( result )
        sale = nullptr;
    refresh();
    return result;
}

bool TicketInfo::pay_credit(const double& amount, bool print)
{
    bool result = data.pay_sale_credit( sale, amount, print );
    if( result )
        sale = nullptr;
    refresh();

    return result;
}

void TicketInfo::print(const int& sale_id)
{
    auto ps = data.find_sale( sale_id );
    printer.print( ps );
}

void TicketInfo::transfer_ownership(const int& userid)
{
    if( !sale && userid > -1) return;
    //sale->user_id = userid;
}

ListModel* TicketInfo::allSaleModel()
{
    all_sales->clear();
    auto& sales = data.getSaleData();
    for( const auto& s : sales ) {
        if( sale && sale->id != s->id )
            all_sales->appendRow(new SaleListItem(s.get()));
    }

    return all_sales.get();
}

double TicketInfo::get_sale_subtotal(const int& saleid)
{
    auto s = data.find_sale( saleid );
    if( s )
        return s->sub_total;
    return -1.0f;
}

double TicketInfo::get_sale_owed(const int& saleid)
{
    auto s = data.find_sale( saleid );
    if( s ) return s->owed;
    return -1.0f;
}

void TicketInfo::set_sale_guests(const int& saleid, const int& guests)
{
    auto s = data.find_sale( saleid );
    if( s ) {
        s->guests = guests;
        data.save_sale( saleid );

        if( sale && sale->id == saleid )
            emit guestsChanged();
    }
}

void TicketInfo::set_sale_tip(const int& saleid, const double& tip)
{
    auto s = data.find_sale( saleid );
    if( s ) {
        if( tip > -0.99 && tip < 100.000001f )
        s->tip_percent = tip;
        data.save_sale( saleid );

        if( sale && sale->id == saleid )
            emit tip_percentChanged();
    }
}
