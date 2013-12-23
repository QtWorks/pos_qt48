import QtQuick 1.1
import "app.js" as App;
import "reports.js" as Reports;

Item {
    id: root;
    anchors.fill: parent;

    Component {
        id: delegate;
        Item {
            anchors.right: parent.right;
            anchors.left: parent.left;
            height: App.screen_h * 0.035;

            Text {
                anchors.left: parent.left;
                anchors.verticalCenter: parent.verticalCenter;
                font.family: "Helvetica";
                font.pointSize: 15;
                color: "white";
                text: hour_name;
            }
            Text {
                anchors.left: parent.left; anchors.leftMargin: parent.width * 0.55;
                anchors.verticalCenter: parent.verticalCenter;
                font.family: "Helvetica";
                font.pointSize: 15;
                color: "white";
                text: hour_sale_count;
            }
            Text {
                anchors.right: parent.right;
                anchors.verticalCenter: parent.verticalCenter;
                font.family: "Helvetica";
                font.pointSize: 15;
                color: "white";
                text: "$" + hour_sale_total.toFixed(2);
            }
        }
    }

    Item
    {
        anchors.right: parent.right;
        anchors.left: parent.left;
        anchors.bottom: hour_detail_list.top;
        height: App.screen_h * 0.05;

        Text {
            anchors.left: parent.left;
            anchors.top: parent.top;
            font.family: "Helvetica";
            font.pointSize: 15;
            color: "white";
            text: "hour #";
        }
        Text {
            anchors.left: parent.left; anchors.leftMargin: parent.width * 0.5;
            anchors.top: parent.top;
            font.family: "Helvetica";
            font.pointSize: 15;
            color: "white";
            text: "sale count";
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
        id: hour_detail_list;
        anchors.fill: parent;
        spacing: App.screen_h * 0.01;
        delegate: delegate;
        clip: true;
        model: Reports.hourDetailSaleModel;

        /*add : Transition {
            NumberAnimation { property: "y"; from: 0; duration: 200; easing.type: Easing.InOutCirc; }
        }*/
    }
}
