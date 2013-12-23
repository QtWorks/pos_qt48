import QtQuick 1.1;
import "colors.js" as Colors;
import "app.js" as App;

Item {
    width: App.screen_w;
    height: App.screen_h;


    Rectangle {
        id: blackBar;

        anchors.left: parent.left; anchors.leftMargin: 1;
        anchors.right: parent.right; anchors.rightMargin: 1;

        anchors.verticalCenter: parent.verticalCenter;

        height: parent.height * 0.4;
        color: "black";
        Image {
            anchors.left: parent.left; anchors.leftMargin: sourceSize.width * 0.3;
            anchors.verticalCenter: parent.verticalCenter;
            source: "home_ico.svg";
            sourceSize.width: parent.height * 0.5;
            sourceSize.height: parent.height * 0.5;
        }
        Row {
            id: mainRow;
            anchors.centerIn: parent;
            spacing: 10;

            Button {
                id: money_btn;
                width: height;
                height: blackBar.height * 0.95;
                color: Colors.green2;
                label: "Money";
                font: "Chunkfive";
                icon: "tag_ico.svg";
                pixelSize: height * 0.15;
                radius: 10;
                Behavior on width { PropertyAnimation { duration: 200; easing.type: Easing.InOutCirc; } }
                Behavior on height { PropertyAnimation { duration: 200; easing.type: Easing.InOutCirc; } }
                onHeightChanged : {
                    if( height == 410 ) {
                    }
                }
                onButtonClick : {
                    anchors.bottom = undefined;
                }
            }
            Button {
                id: inv_btn;
                width: height;
                height: blackBar.height * 0.95;
                color: Colors.orange;
                label: "Inventory";
                icon: "inventory_ico.svg";
                font: "Chunkfive";
                pixelSize: height * 0.15;
                radius: 10;
                Behavior on width { PropertyAnimation { duration: 200; easing.type: Easing.InOutCirc; } }
                Behavior on height { PropertyAnimation { duration: 200; easing.type: Easing.InOutCirc; } }
                onHeightChanged : {
                    if( height == 410 ) {
                    }
                }
                onButtonClick : {
                    anchors.bottom = undefined;
                }
            }
            Button {
                id: employee_btn;
                width: height;
                height: blackBar.height * 0.95;
                color: Colors.brown;
                label: "Employee";
                icon: "hardhat_ico.svg";
                font: "Chunkfive";
                pixelSize: height * 0.15;
                radius: 10;
                Behavior on width { PropertyAnimation { duration: 200; easing.type: Easing.InOutCirc; } }
                Behavior on height { PropertyAnimation { duration: 200; easing.type: Easing.InOutCirc; } }
                onHeightChanged : {
                    if( height == 410 ) {
                    }
                }
                onButtonClick : {
                    anchors.bottom = undefined;
                }
            }
        }//Row
    }
}
