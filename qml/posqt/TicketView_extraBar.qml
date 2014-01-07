import QtQuick 1.1
import "colors.js" as Colors;
import "ticket.js" as TicketLib;
import "app.js" as App;

HorizontalSlide {
    id: extra;
    to: 0;
    from: -width;

    color: "#aaffffff";

    Row {
        id: actions;
        anchors.right: parent.right; anchors.rightMargin: parent.width * 0.1;
        anchors.verticalCenter: parent.verticalCenter;

        spacing: 10;

        Button {
            id: tipb;
            anchors.verticalCenter: parent.verticalCenter;
            height: extra.height * 0.98;
            width: height;
            radius: 5;
            color: Colors.violet2;
            icon: "tag_ico.svg";
            label: "Tip";
            pixelSize: height * 0.15;
            font: "Chunkfive";
            onButtonClick : root.set_tip();
        }

        Button {
            id: transfer_owner;
            anchors.verticalCenter: parent.verticalCenter;
            height: extra.height * 0.98;
            width: height;
            radius: 5;
            color: Colors.blue2;
            icon: "hardhat_ico.svg";
            label: "Transfer";
            pixelSize: height * 0.15;
            font: "Chunkfive";
            onButtonClick : root.transfer();
        }
    }
}
