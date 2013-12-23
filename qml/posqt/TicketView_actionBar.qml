import QtQuick 1.1
import "colors.js" as Colors;
import "ticket.js" as TicketLib;
import "app.js" as App;
import "user.js" as User;

Slide {
    id: root;
    property alias font : edit.font;
    property alias pixelSize: edit.pixelSize;

    signal payClick();
    signal doneClick();

    Row {
        id: actionGrid;
        anchors.right: parent.right;
        anchors.left: parent.left;
        anchors.verticalCenter: parent.verticalCenter;
        spacing: 10;

        Button {
            id: edit;
            anchors.verticalCenter: parent.verticalCenter;
            height: root.height * 0.8;
            width: root.height * 0.8;
            color: Colors.main_blue;
            font: "Chunkfive";
            icon: "edit_ico.svg";
            radius: 10;
            label: "Edit";
            visible: ticket_editable;
            pixelSize: height * 0.2;
            onButtonClick : {
                TicketLib.refresh();
                App.refresh();
                App.editTicket();
            }
        }
        Button {
            id: split;
            anchors.verticalCenter: parent.verticalCenter;
            height: root.height * 0.8;
            width: root.height * 0.8;
            radius: 10;
            color: Colors.violet3;
            icon: "settings_ico2.svg";
            label: "Split";
            font: root.font;
            pixelSize: root.pixelSize;
            visible: ticket_editable;
            onButtonClick : {
                pay_actions.hide();
                split_actions.toggle();
            }
        }
        Button {
            id: pay;
            anchors.verticalCenter: parent.verticalCenter;
            height: root.height * 0.8;
            width: root.height * 0.8;
            radius: 10;
            color: Colors.green4;
            icon: "tag_ico.svg";
            label: "Pay";
            font: root.font;
            pixelSize: root.pixelSize;
            visible: ticket_editable;
            onButtonClick : {
                if( ticket.ticketSubTotal > 0 ) {
                    payClick();
                } else {
                    TicketLib.close();
                    App.refresh();
                    User.api.refresh();
                    TicketLib.refresh();

                    closed();
                }
            }
        }
        Button {
            id: done;
            anchors.verticalCenter: parent.verticalCenter;
            height: root.height * 0.8;
            width: root.height * 0.8;
            radius: 10;
            color: Colors.orange3;
            icon: "x_ico.svg";
            label: "Done";
            pixelSize: root.pixelSize;
            font: root.font;
            onButtonClick : doneClick();
        }
    }

}
