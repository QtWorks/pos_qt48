import QtQuick 1.1
import "app.js" as App

Item {
    id: root;

    anchors.right: parent.right;
    anchors.left: parent.left;
    height: App.screen_h * 0.025;

    Text {
        anchors.left: parent.left;
        anchors.verticalCenter: parent.verticalCenter;
        font.family: "Helvetica";
        font.pointSize: 15;
        color: "white";
        text: user_firstname + " " + user_lastname;
    }
    Text {
        anchors.left: parent.left; anchors.leftMargin: parent.width * 0.55;
        anchors.verticalCenter: parent.verticalCenter;
        font.family: "Helvetica";
        font.pointSize: 15;
        color: "white";
        text: user_sale_count;
    }
    Text {
        anchors.right: parent.right;
        anchors.verticalCenter: parent.verticalCenter;
        font.family: "Helvetica";
        font.pointSize: 15;
        color: "white";
        text: "$" + user_sale_total.toFixed(2);
    }
}

