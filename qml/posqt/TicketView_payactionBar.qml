import QtQuick 1.1
import "colors.js" as Colors;

Slide {
    id: root;

    Row {
        anchors.verticalCenter: parent.verticalCenter;
        anchors.right: parent.right; anchors.rightMargin: parent.width * 0.2;
        spacing: 10;

        Button {
            id: cash;
            width: root.height * 0.9;
            height: width;
            icon: "cash_ico.svg";
            label: "Cash";
            color: Colors.blue6;
            pointSize: 20;
            radius: 10;
            onButtonClick : {
                cashView.color = Colors.make( color, "99" );
                cashView.hint = 0;
                cashView.show();
                cashmode = true;
            }
        }

        Button {
            id: credit;
            width: root.height * 0.9;
            height: width;
            icon: "credit_ico.svg";
            label: "Credit";
            color: Colors.red2;
            pointSize: 20;
            radius: 10;
            onButtonClick : {
                cashView.color = Colors.make( color, "99" );
                cashmode = false;
                cashView.hint = ticket.ticketTotal;
                cashView.show();
            }
        }
    }
}
