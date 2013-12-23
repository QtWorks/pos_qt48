.pragma library

/* NOTE:There are no acutal tickets inside the main C++ code
        Ticket is more of an alias for Sale class inside the core.
        The ticket representation is build on-top-of the Sale data.
        The QML file "Ticket.qml" is the only True representation of a ticket.
*/

var _init   = false;
var split_mode = false;
var split_to = null;
var split_from = null;
var callback;
var saleid = null;
var tableid = null;
var active_ticket;
var _active = null;
var SubTotal = null;
var TaxPercent = 0.06;

//C++ API
var API;

//Function... 'pointers'
var refresh     = null;
var activate    = null;
var close       = null;
var updateListAdd = null;
var pay_cash    = null;
var pay_credit  = null;
var unsplit     = null;
var combine     = null;
var transfer    = null;
var set_sale    = null;
var finalize_changes = null;
var discard_changes = null;
var void_item   = null;
var delete_item = null;

function init(ticketAPI) {
    API = ticketAPI;
    _init = true;

    //asign functions
    refresh     = API.Refresh;
    activate    = API.set_sale;
    close       = API.close;
    updateListAdd = API.updateListAdd;
    pay_cash    = API.pay_cash;
    pay_credit  = API.pay_credit;
    unsplit     = API.unsplit;
    combine     = API.combine;
    transfer    = API.transfer;
    set_sale    = API.set_sale;
    finalize_changes  = API.finalize_changes;
    discard_changes = API.discard_changes;
    void_item   = API.void_item;
    delete_item = API.delete_item;

    SubTotal = API.subTotal;
    saleid = function (api){ return api.saleId }(API);
}

function isValid() {
    return ( _init && API.Valid() );
}

function get_list() {
    return API.ticketListModel;
}

function get_selected_item() {
    return itemselect;
}

//Refresh ticket data
/*function refresh() {
    API.Refresh();
    SubTotal = API.subTotal;
}*/

//Select Existing order inside the Ticket(sale)
function select(item) {
    if ( !isValid() ) return;
    if( split_mode ) {
        //split_select = item; //item
    }
    API.SetActive(item); //item id
}

function selectTicket( ticket ) {
    if( !isValid() ) return;
    _active = ticket;
}

function voidItem(item_id) {
    if ( !isValid() ) return;

    select(item_id);
    API.VoidItem(item_id);
    updateListAdd();
}

function split( ticket ) {
    if ( !isValid() ) return;
    split_mode = true;
    split_from = null;
    split_to = null;
    return API.split( );
}

/*function close( ) {
   API.close();
}

function updateListAdd() {
    API.updateListAdd();
}


function cashOut( amount ) {
    return API.cashOut( amount );
}
*/

function removeSplit( ticket ) {
    API.removeSplit( ticket.sale_id );
}

function unsaved()
{
    return API.saveCount;
}

function activateItem(item_id) {

}

function selectLastItem() {
    if( !isValid() ) return;

    _active.selected_id = _active.orderListModel.count - 1;
}

function transfer(item_id, to, from) {
    if ( !isValid() ) return;
    //console.log( "Tranfer id: " + item_id );
    API.transfer( item_id, to, from );
}

function hasSelect( ticket ) {
    return ( ticket && ticket.selected_id > -1 );
}

function hasItems( ticket ) {
    return ( ticket && ticket.count );
}


function split_click( ticket ) {
    //console.log( "Split click " + ticket.sale_id + " , " + ticket.selected_id);
    //console.log( "Item count : " + ticket.count );
    if ( !split_from && hasItems(ticket) ) {
        //console.log( "set split from");
        split_from = ticket;
        if( callback )
            callback.text =  "Choose Ticket";
        return;
    }

    //console.log( "Split to");
    if ( hasSelect( split_from ) && ticket !== split_from) {
        //console.log( "set" );
        split_to = ticket;
    }

    if( split_to && hasSelect(split_from) ) {
        split_transfer();
    }
}

//Clear split flags & selections
function split_reset()
{
    split_to.selected_id = -1;
    split_from.selected_id = -1;
    if( callback )
        callback.text = "Choose Item";
    split_to = null;
    split_from = null;
}

//Complete split-item-transfer based on set selections
function split_transfer()
{
    //console.log( "transfer" );
    var item = split_from.orderListModel.get( split_from.selected_id );
    if ( item.item_is_sub ) //don't transfer single modifiers explicitly
        return;

    //update sub totals manually
    split_from.ticketSubTotal -= item.item_total;
    //transfer the [GUI] item to the destination ticket
    split_to.add( item );
    //Search for sub items in the list and transfer the attached subs as well
    for( var i = split_from.selected_id + 1; i < split_from.count; i++) {
        var sub = split_from.orderListModel.get( i );
        if( sub.item_is_sub ) {
            split_to.add( sub );
        } else {
            break;
        }
    }
    //Complete by using the C++ transfer API
    API.transfer( item.item_id, split_to.sale_id, split_from.sale_id );

    refresh();

    //Virutal tickets are not updated automatically.
    //Remove [GUI] item form original if ticket is virtual
    if( split_from.virtual ) {
        split_from.remove( item );
    }

    //Reset
    split_reset();
}
