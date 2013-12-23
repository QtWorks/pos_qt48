import QtQuick 1.1
import "ticket.js" as TicketLib;
import "app.js" as App;
import "colors.js" as Colors;

Item {
    anchors.centerIn: parent;
    width: App.screen_w;
    height: App.screen_h;
    property variant vm : null;
    property variant vt : null;
    property int splitCount : 0;

    Component {
        id: list_comp;
        ListModel { }
    }

    Item {
        width: 1220;
        height: 900;
        anchors.left: ticket_area.right;
        anchors.top: ticket_area.top;
        anchors.leftMargin: 10;
        Flickable {
            id: tickFlicker;
            anchors.fill: parent;
            contentHeight: 800;
            contentWidth: 600 * splitCount + splitCount * 10;
            boundsBehavior: Flickable.StopAtBounds;
            clip: true;
            Row {
                //anchors.fill: parent;
                id: split_area;
                spacing: 10;// * (tickFlicker.contentWidth / 600);
            }
        }
    }
    Item {
        id: ticket_area;
        anchors.left: parent.left;
        anchors.verticalCenter: parent.verticalCenter;
        width: 600;
        height: 800;

        Ticket {
            id: rootTicket;
            anchors.fill: parent;
            ticketSubTotal: TicketLib.SubTotal;
            sale_id : 368;
            onSplitClick : {
                splitCount++;
                vt = rootTicket.split( split_area, list_comp.createObject(parent) );
            }
        }
    }

    Component.onCompleted : {
        TicketLib.activate( 368 );
        rootTicket.ticketSubTotal = TicketLib.SubTotal;
    }



    Button {
        label: "Exit";
        anchors.right: parent.right;
        anchors.bottom: parent.bottom;
        onButtonClick: {
            App.exit();
        }
    }
}


