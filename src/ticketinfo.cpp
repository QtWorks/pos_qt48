#include "ticketinfo.h"
#include "ticketlistmodel.h"

TicketInfo::TicketInfo(QObject* parent) :
    QObject(parent),
    data(Data::Instance()),
    printer(Printer::Instance())
{
    model = new ListModel(new TicketListItem());
}

TicketInfo::~TicketInfo()
{
    delete model;
    model = nullptr;
}

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
    updateListModel();
    emit ticketListModelChanged();
    emit subTotalChanged();
    emit taxTotalChanged();
    emit TotalChanged();
    emit saleIdChanged();
    emit credit_paidChanged();
    emit cash_paidChanged();
    emit owedChanged();
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

    data.pay_sale(removed_sale, 0.0f);
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
            transfer( i - 1, sale->id, from->id );
        }
        i--;// = from->item_count;
    }

    data.pay_sale(from, 0);
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

bool TicketInfo::pay_cash(const double& amount)
{
    bool result = data.pay_sale( sale, amount );
    if( result )
        sale = nullptr;
    refresh();
    return result;
}

bool TicketInfo::pay_credit(const double& amount)
{
    bool result = data.pay_sale_credit( sale, amount );
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
