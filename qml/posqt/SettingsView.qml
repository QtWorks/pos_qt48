import QtQuick 1.1;
import "colors.js" as Colors;
import "app.js" as App;

Item {
    id: root;
    anchors.fill: parent;

    Rectangle {
        width: blackbar.width;
        height: blackbar.height * 0.8;
        anchors.left: blackbar.left;
        anchors.top: blackbar.bottom;
        color: "black";
        Image {
            //anchors.left: parent.left;
            anchors.centerIn: parent;
            //anchors.verticalCenter: parent.verticalCenter;
            //anchors.h
            source: "settings_ico2.svg";
            sourceSize.width: parent.height * 0.8;
            sourceSize.height: parent.height * 0.8;
        }
    }

    Rectangle {
        id: blackbar;

        anchors.left: parent.left; anchors.leftMargin: 1;
        anchors.right: parent.right; anchors.rightMargin: 1;
        anchors.top: parent.top; anchors.topMargin: parent.height * 0.2;

        //anchors.verticalCenter: parent.verticalCenter;

        height: parent.height * 0.4;
        color: "black";
        /*Image {
            anchors.left: parent.left;
            anchors.verticalCenter: parent.verticalCenter;
            source: "settings_ico2.svg";
            sourceSize.width: parent.height * 0.8;
            sourceSize.height: parent.height * 0.8;
        }*/

        Row {
            spacing: 10;
            anchors.centerIn: parent;
           // anchors.horizontalCenter: parent.horizontalCenter;
            //anchors.top: parent.top;

            Button {
                width: blackbar.height * 0.95;
                height: blackbar.height * 0.95;
                color: Colors.red2;
                radius: 5;
                label: "Menu";
                font: "Chunkfive";
                icon: "menu_ico.svg";
                pixelSize: height * 0.15;
                onButtonClick: load( "SettingsMenuView.qml" );
            }
            Button {
                width: blackbar.height * 0.95;
                height: blackbar.height * 0.95;
                color: Colors.orange;
                radius: 5;
                label: "Employees";
                icon: "hardhat_ico.svg";
                font: "Chunkfive";
                pixelSize: height * 0.15;
                onButtonClick: load( "SettingsUserView.qml" );
            }
            Button {
                width: blackbar.height * 0.95;
                height: blackbar.height * 0.95;
                color: Colors.blue5;
                radius: 5;
                icon: "table_no_legs_ico.svg";
                font: "Chunkfive";
                label: "Layout";
                pixelSize: height * 0.15;
                onButtonClick: load( "SettingsLayoutView.qml" );
            }
            Button {
                width: blackbar.height * 0.95;
                height: blackbar.height * 0.95;
                color: Colors.yellow3;
                radius: 5;
                icon: "print_ico.svg";
                font: "Chunkfive";
                label: "Printing";
                pixelSize: height * 0.15;
                onButtonClick: load( "SettingsPrintView.qml" );
            }
        }
    }

    Loader {
        id: loader;
        focus: true;
        onStatusChanged : {
            if( status == Loader.Ready ) {
                item.show(200);
            }
        }
    }

    function load( component ) {
        //loader.source = "";
        //loader.source = component;
        //loader.setSource( component, { "parent" : root } );
        App.load_subview( component );
    }
}
