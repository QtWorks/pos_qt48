import QtQuick 1.1
import "ticket.js" as TicketLib;
import "app.js" as App;
import "colors.js" as Colors;

Item {
    id: root;
    anchors.centerIn: parent;
    width: App.screen_w;
    height: App.screen_h;
    property int splitCount : 0;
    property int stid: 0;
    property int siid: 0;
    property bool selection: false;
    property variant from_t : null;
    property variant to_t : null;
    property variant selected_ticket : null;

    function backButtonPressed() {
        TicketLib.split_mode = false;
        App.setBackCB( null );
    }

    function removeClick( vt ) {
        var c = vt.count;
        TicketLib.combine( rootTicket.sale_id, vt.sale_id );
        vt.destroy();
        if( c ){
            TicketLib.refresh();
            rootTicket.ticketSubTotal = TicketLib.SubTotal;
        }
    }

    function view_split_select_item( sticket, sale_id, item_id ) {
        from_t = sticket;
        dimmer.show();
        removeBtn.visible = false;
    }

    function split_ticket_select( sticket ) {
        removeBtn.visible = !(rootTicket.sale_id == sticket.sale_id);
        if( sticket.sale_id !== rootTicket.sale_id ) {
            indicator.x = sticket.x + rootTicket.width;
        }
        else {
            indicator.x = rootTicket.x;
        }

        selected_ticket = sticket;
    }

    function split_select_destination( sticket, sale_id ) {
        if( from_t == sticket ) {
            from_t.selected_id = -1;
            from_t = null;
            dimmer.hide();
            return;
        }
        if( from_t && from_t !== sticket && from_t.selected_id > -1) {

            var item = from_t.orderListModel.get( from_t.selected_id );
            if( item.item_is_sub ) return;

            from_t.ticketSubTotal -= item.item_total;

            sticket.add( item );

            for( var k = from_t.selected_id + 1; k < from_t.count; k++ ) {
                var sub = from_t.orderListModel.get( k );
                if( sub.item_is_sub ) {
                    sticket.add( sub );
                } else {
                    break;
                }
            }

            TicketLib.transfer( from_t.selected_id, sale_id, from_t.sale_id );
            TicketLib.refresh();

            if( from_t.virtual )
                from_t.remove( item );


            dimmer.hide();
            removeBtn.visible = false;
            from_t = null;
        }
    }

    function hasItem() { return (from_t && from_t.selected_id > -1); }

    Component {
        id: list_comp;
        ListModel { }
    }

    FadeView {
        id: bg;
        dim: true;
        duration: 1000;
        dimColor: Colors.make( "#000000", "cc" );
        color: Colors.make ( Colors.violet3, "99" );
        visible: true;
        opacity: 0.2;
    }

    Item {
        id: virtual_area;
        width: rootTicket.width * 2 + root.width * 0.003;//1220;
        height: rootTicket.height;
        anchors.left: rootTicket.right; anchors.leftMargin: 10;
        anchors.top: rootTicket.top;

    }

    Flickable {
        id: tickFlicker;
        anchors.fill: virtual_area;
        contentHeight: rootTicket.height;
        contentWidth: rootTicket.width * splitCount + splitCount * (parent.width * 0.001);
        boundsBehavior: Flickable.StopAtBounds;
        clip: true;
        Row {
            //anchors.fill: parent;
            id: split_area;
            spacing: 10;
            layoutDirection: Qt.RightToLeft;
            add : Transition {
                NumberAnimation { property: "y"; from: -rootTicket.height; duration: 200; easing.type: Easing.InOutCirc; }
            }
            move : Transition {
                PropertyAnimation { property: "x"; duration: 200; easing.type: Easing.InOutCirc; }
            }
        }
    }

    Rectangle {
        anchors.top: rootTicket.bottom;
        anchors.left: parent.left;
        anchors.right: parent.right;
        anchors.bottom: parent.bottom; anchors.bottomMargin: 10;

        color: "black";
        Button {
            anchors.left: parent.left; anchors.leftMargin: parent.width * 0.001;
            anchors.verticalCenter: parent.verticalCenter;
            //anchors.top: rootTicket.bottom; anchors.topMargin: 10;
            id: splitBtn;
            height: parent.height * 0.90;
            width: rootTicket.width;
            radius: 5;
            color: Colors.green2;
            pixelSize: height * 0.4;
            font: "Chunkfive";
            label: "Add Ticket";

            onButtonClick : {
                splitCount++;
                var vt = rootTicket.split( split_area, list_comp.createObject(parent) );
                vt.visible = true;
            }
        }

        Button {
            anchors.left: splitBtn.right; anchors.leftMargin: root.width * 0.001;
            anchors.verticalCenter: parent.verticalCenter;
            //anchors.top: splitBtn.top;
            width: rootTicket.width;
            height: splitBtn.height;
            radius: 5;
            color: Colors.red2;
            id: removeBtn;
            visible: false;
            pixelSize: height * 0.4;
            font: "Chunkfive";
            label: "Remove Ticket";
            onButtonClick : {
                visible = false;
                removeClick( selected_ticket );
                indicator.x = -indicator.width;
            }
        }

        Image {
            source: "barcode_ico.svg";
            sourceSize.width: parent.height * 0.7;
            sourceSize.height: parent.height * 0.7;

            anchors.right: parent.right; anchors.rightMargin: sourceSize.width * 0.3;
            anchors.verticalCenter: parent.verticalCenter;
        }
    }

    FadeView {
        id: dimmer;
        duration: 300;
        dim: true;
        dimColor: Colors.make( "#000000", "cc" );
        color: Colors.make( Colors.blue2, "99" );
        onClicked :  {
            hide();
        }
    }

    Ticket {
        id: rootTicket;
        anchors.left: parent.left; anchors.leftMargin: parent.width * 0.001;
        anchors.top: parent.top; anchors.topMargin: 10;
        width: (parent.width - (parent.width * 0.003)) / 3;
        ticketSubTotal: TicketLib.SubTotal;
    }


    Rectangle {
        id: indicator;
        color: Colors.make( Colors.blue4, "99" );
        width: rootTicket.width;
        height: rootTicket.height;
        x: -width;
        y: rootTicket.y;

        Behavior on x { PropertyAnimation { duration: 300; easing.type: Easing.InOutCirc; } }
    }

    Component.onCompleted : {
        TicketLib.refresh();
        rootTicket.ticketSubTotal = TicketLib.API.subTotal.toFixed(2);
        rootTicket.sale_id = TicketLib.API.saleId;
        bg.show();
        App.setBackCB( backButtonPressed );
    }
}
