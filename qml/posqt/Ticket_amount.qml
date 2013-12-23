import QtQuick 1.1;
import "colors.js" as Colors;
import "ticket.js" as TicketLib;

Item {
    id: root;

    anchors.fill: parent;
    Column {
        spacing: 10;
        anchors.verticalCenter: parent.verticalCenter;
        anchors.left: parent.left; anchors.leftMargin:  parent.width * 0.1;

        Text {
            font.pointSize: 18;
            font.family: "Chunkfive";
            color: Colors.make( Colors.green3, "88" );
            text: "Paid: $" + (ticketTotal - ticketOwed).toFixed(2);
        }

        Text {
            font.pointSize: 20;
            font.family: "Chunkfive";
            color: "white";
            text: "Owed: $" + ticketOwed.toFixed(2);
        }
    }
    Column {
        spacing: 1;
        anchors.verticalCenter: parent.verticalCenter;
        anchors.right: parent.right; anchors.rightMargin: total_text.width * 1.4;//root.width * 0.20;
        Text {
            id: sub_total_text;
            text: "Sub Total";
            color: "white";
            font.family: "Helvetica";
            font.pointSize: 18;
            Text {
                id: subtotal_text;
                anchors.bottom: parent.bottom; //anchors.bottomMargin: root.height * 0.2;
                anchors.left: parent.right; anchors.leftMargin: 10;
                font.pointSize: 15;
                font.family: "Helvetica";
                color: "white";
                text: ticketSubTotal.toFixed(2);
            }
        }
        Text {
            text: "Taxes";
            color: "white";
            font.family: "Helvetica";
            font.pointSize: 18;
            anchors.right: sub_total_text.right;
            Text {
                id: tax_text;
                anchors.left: parent.right; anchors.leftMargin: 10;
                anchors.bottom: parent.bottom; //anchors.bottomMargin: root.height * 0.2;
                font.pointSize: 15;
                font.family: "Helvetica";
                color: "white";
                text: "+" + (ticketSubTotal * 0.06).toFixed(2);
            }
        }
        Text {
                text: "Total";
                color: "white";
                font.family: "Helvetica";
                font.pointSize: 18;
                anchors.right: sub_total_text.right;
                Text {
                    id: total_text;
                    anchors.left: parent.right; anchors.leftMargin: 10;
                    anchors.bottom: parent.bottom; //anchors.bottomMargin: root.height * 0.2;
                    font.pointSize: 20;
                    font.family: "Helvetica";
                    color: Colors.main_green;
                    text: "$" + (Number(ticketSubTotal.toFixed(2)) + Number((ticketSubTotal * 0.06).toFixed(2))).toFixed(2);
                }
        }
    }
}
