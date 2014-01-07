import QtQuick 1.1
import "app.js" as App;
import "ticket.js" as TicketLib
import "colors.js" as Colors;
import "ticket_functions.js" as Methods

Item {
    id: ticketBox;
    width: App.screen_w * 0.34;
    height: App.screen_h * 0.77;//800;
    property double ticketSubTotal  : TicketLib.API.subTotal;
    property double ticketTaxTotal   : 0.0;
    property double ticketTaxPercent : 0.06;
    property double ticketTotal     : TicketLib.API.Total;
    property double ticketOwed      : TicketLib.API.owed;
    property double ticketTip       : TicketLib.API.tip_percent;
    property alias  userName        : user.text;
    property bool   split_mode : TicketLib.split_mode;
    property bool   virtual : false;
    property int    sale_id : -1;//TicketLib.get_sale_id();
    property bool allowOk : false;
    property bool editable : true;
    property bool showSplitButton: editable;
    property bool showEditButton: (virtual) ? false : editable;
    property bool showOrderButton: false;
    property bool showPayButton: (virtual) ? false : editable;
    property int  selected_id : -1;
    property int  menu_id : -1;
    property color ticketColor: "#2c2a29";
    property variant item : undefined;
    property variant orderListModel : TicketLib.get_list();
    property variant lastActions: null;
    property int count : ticketView.count;
    property int itemWidth : (virtual) ? 400 : 500;
    property alias bgColor : ticketBG.color;
    property alias selected : indicator.visible;
    property bool snap_to_item : true;

    //Method bindings
    function split( area, orderList ) { return Methods.split( area, orderList); }
    function reset() { Methods.reset(); }
    function remove( item ) { Methods.remove( item ); }
    function add( item ) { Methods.add( item ); }
    function transfer( item_id, to ) { Methods.transfer(item_id,  to); }
    function showActions( show, index, h) { Methods.showActions(show, index, h); }
    function vkill() { Methods.vkill(); }
    function seek(item_id) { Methods.seek(item_id); }
    function seek_end() { Methods.seek_end(); }

    signal ticketClick();
    signal okClick();
    signal itemClick(variant _item_id, variant _cat_id);
    signal finalItemClick(variant _item_id, variant _cat_id);
    signal subItemClick(variant _item_id, variant _item_refill);
    signal longClick(variant _item_id, variant _cat_id);
    signal editClick();
    signal voidClick();
    signal splitClick();
    signal saveClick();
    signal cashClick(variant _payed);
    signal removeMeClick();
    signal printClick();

    onTicketSubTotalChanged : {
        ticketTaxTotal = ticketSubTotal * ticketTaxPercent;
        ticketTotal = ticketSubTotal + ticketTaxTotal;
        ticketTotal = ticketTotal + (ticketTotal * ticketTip/100);
    }

    onTicketTipChanged : {
        ticketTaxTotal = ticketSubTotal * ticketTaxPercent;
        ticketTotal = ticketSubTotal + ticketTaxTotal;
        ticketTotal = ticketTotal + (ticketTotal * ticketTip/100)
    }

    function splitSelect( item_id ) {
        view_split_select_item( ticketBox, sale_id, item_id );
    }

    Component {
        id: virtual_list;
        ListModel {
        }
    }

    Component { //Ticket Item delegate
        id: ticketItemDelegate;
        TicketItem {
            onClick : itemClick(_iid, _cid);
            onItemLongClick : longClick( _iid, _cid );
            onSubClick : subItemClick( _iid, _refill );
            onFinalClick : finalItemClick( _iid, _cid );
        }
    }
    Button {
        anchors.right: parent.right;
        anchors.bottom: parent.top; anchors.bottomMargin: App.screen_h * 0.01;
        width: parent.height * 0.2;
        height: parent.height * 0.2;
        icon: "print_ico.svg";
        font: "Chunkfive";
        label: "Print";
        pixelSize: height * 0.15;
        onButtonClick : printClick();
    }

    Text {
        id: user;
        anchors.left: parent.left; anchors.leftMargin: parent.width * 0.1;
        anchors.bottom: parent.top;
        anchors.bottomMargin: App.screen_h * 0.01;
        font.family: "Chunkfive";
        font.pixelSize: parent.height * 0.05;
        color: "white";
    }

    Rectangle { //Main Ticket Body
        id: ticketBod;
        color:  ticketColor;
        radius: 10;
        anchors.top: parent.top;

        width: parent.width;
        height: parent.height * 0.15;

        Behavior on scale { PropertyAnimation { duration: 200; } }

        Rectangle {
            id: indicator;
            radius: 10;
            anchors.fill: parent;
            color: Colors.make( Colors.blue3, "99" );
            visible: false;
        }

        Ticket_amount {
        }
        MouseArea {
            id: ticketMouseArea;
            visible: (split_mode || virtual);
            anchors.fill: parent;
            preventStealing: true;
            //propagateComposedEvents: true;
            onPressed : {
                if( TicketLib.split_mode ) {
                    if( hasItem() )
                        split_select_destination( ticketBox, sale_id );
                    else
                        split_ticket_select( ticketBox );
                } else {
                    mouse.accepted = false;
                }
            }
        }

    }

    Rectangle {
        id: ticketBG;
        color: "white";
        anchors.top: ticketBod.bottom; anchors.topMargin: parent.height * 0.01;

        width: parent.width;// * 0.8;
        height: parent.height * 0.85;

        ListView {
            id: ticketView;
            anchors.fill: parent; anchors.topMargin: parent.height * 0.01;
            anchors.bottomMargin: 50;
            clip: true;
            model: orderListModel;//TicketInfo.ticketListModel;
            spacing: 5;
            delegate: ticketItemDelegate;
            boundsBehavior: Flickable.StopAtBounds;
            pressDelay: 0;

            transitions : [
                Transition {
                    NumberAnimation { property: "y"; from: 0; duration: 300; easing.type: Easing.InOutCirc; }
                }
            ]
            /*remove : Transition {
                NumberAnimation { property: "y"; to: 0; duration: 300; easing.type: Easing.InOutCirc; }
                NumberAnimation { property: "opacity"; from: 1; to: 0; duration: 400; easing.type:Easing.InOutCirc;}
            }*/
         }

        Rectangle {
            color: Colors.blue3;
            anchors.bottom: parent.bottom;
            anchors.right: parent.right;
            anchors.left: parent.left;
            height: 5;
        }
    }




    CashInput {
        id: cashOut;
        anchors.centerIn: parent;
        visible: false;
        onPayClicked : {
            cashOut.visible = false;
            cashClick( TicketLib.cashOut( _total ) );
            App.refresh();
        }
    }
}
