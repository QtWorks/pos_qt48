import QtQuick 1.1
import "app.js" as App;
import "reports.js" as Reports;

Item {
    id: itemlist_item;
    anchors.fill: parent;

    Component { id: item_detail_delegate; ItemDetailCmp {} }

    Item
    {
        anchors.right: parent.right;
        anchors.left: parent.left;
        anchors.bottom: item_detail_list.top;
        height: App.screen_h * 0.05;

        Text {
            anchors.left: parent.left;
            anchors.top: parent.top;
            font.family: "Helvetica";
            font.pointSize: 15;
            color: "white";
            text: "name";
        }
        Text {
            anchors.left: parent.left; anchors.leftMargin: parent.width * 0.5;
            anchors.top: parent.top;
            font.family: "Helvetica";
            font.pointSize: 15;
            color: "white";
            text: "items sold";
        }
        Text {
            anchors.right: parent.right;
            anchors.top: parent.top;
            font.family: "Helvetica";
            font.pointSize: 15;
            color: "white";
            text: "total";
        }
    }
    ListView {
        id: item_detail_list;
        anchors.fill: parent;
        spacing: App.screen_h * 0.01;
        delegate: item_detail_delegate;
        clip: true;
        model: Reports.itemDetailSaleModel;

        /*add : Transition {
            NumberAnimation { property: "y"; from: 0; duration: 200; easing.type: Easing.InOutCirc; }
        }*/
    }
}
