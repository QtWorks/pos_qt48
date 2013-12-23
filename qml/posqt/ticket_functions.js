function reset() {
    selected_id = -1;
    menu_id = -1;
}

//Recursive function to remove the item leftovers(sub items)
function sub_remove( sub_id )
{
    var item = orderListModel.get( sub_id );
    if( item && item.item_is_sub ) {
        orderListModel.remove( sub_id );
        sub_remove( sub_id );
    }
}

function remove( item )
{
    console.log( "*** Remove leftovers ***");
    console.log( "Remove requested at id: " + item.item_id );
    console.log( orderListModel.count );
    var i = item.item_id;
    orderListModel.remove( item.item_id );
    console.log( orderListModel.count );
    sub_remove( i );

    for( var i = 0; i < orderListModel.count ; i++) {
        console.log( orderListModel.get( i ).item_name );
        orderListModel.setProperty(i, "item_id", i);
        orderListModel.get( i ).item_displayid = i + 1;
    }
}

function add( item )
{
    console.log( "Add item: " + item.item_name );
    if( !item.item_is_sub )
        ticketSubTotal += item.item_total;
    if( virtual ) { //append to the virtual order list
        //fix the item id when added to virutal tickets
        orderListModel.append( item );
        for( var i = 0; i < orderListModel.count; i++) {
            orderListModel.setProperty(i, "item_id", i);
            orderListModel.get( i ).item_displayid = i + 1;
        }
    }
    else {
        //TicketLib handles the case for the rootTicket
    }
}

function transfer( item_id, to )
{
    TicketLib.transfer( item_id, to, sale_id );
    TicketLib.refresh();
}

function split( area, orderList ) {
    //Ticket item spawns a new Ticket, creating a split
    split_mode = true;
    var component = Qt.createComponent("Ticket.qml");
    var sid = TicketLib.split();
    var vt =  component.createObject( area, { "visible" : false, "orderListModel" : null, "virtual" : true, "sale_id" : sid, "width" : width } );
    vt.orderListModel = virtual_list.createObject(vt);
    return vt;
}

function showActions( show, index, h )
{
    if( show ) {
        orderActions.x = ticketBox.x;
        orderActions.y = mapToItem(ticketBox, 0, 0).y;
        menu_id = mid;
        orderActions.visible = true;
    }
    else {
        orderActions.visible = false;
    }
}

//Virtual ticket self destruct
function vkill()
{
}

function seek_end()
{
    ticketView.positionViewAtEnd();
}

function seek( item_id )
{
    ticketView.positionViewAtIndex( item_id - 1, ListView.Beginning );
}
