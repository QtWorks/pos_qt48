import QtQuick 1.1

Rectangle{
    id: tab;
    width: parent.width/3;
    height: parent.height/10;
    anchors.bottom: parent.bottom;
    anchors.horizontalCenter: parent.horizontalCenter;
    anchors.bottomMargin: parent.height * 0.015;
    color: "#00000000";
    property int buttonHeight: height;

    signal itemClick( variant item );

    Row{
        id: buttonRow;
        anchors.verticalCenter: parent.verticalCenter;
        anchors.left: parent.left;
        anchors.right: parent.right;

        spacing: 5;

        Button {
            id: launchButton;

            height: tab.buttonHeight;
            width: parent.width/5 - 5;
            label: "Launch";
            icon: "launch_ico.svg";
            onButtonClick: tab.itemClick( -1 );
        }
        Button {
            id: mainButton;

            width: parent.width/5 - 5;
            height: tab.buttonHeight;
            label: "Main";
            icon: "home_ico.svg";
            onButtonClick: tab.itemClick( 0 );
        }
        Button {
            id: configButton;

            width: parent.width/5 - 5;
            height: tab.buttonHeight;
            label: "Configure";
            icon: "settings_ico2.svg";
            onButtonClick: tab.itemClick( 1 );
        }
        Button {
            id: reportsButton;

            height: tab.buttonHeight;
            width: parent.width/5 - 5;
            label: "Reports";
            icon: "reports_ico3.svg";
            onButtonClick: tab.itemClick( 2 );
        }
        Button {
            id: exitButton;

            height: tab.buttonHeight;
            width: parent.width/5 - 5;
            label: "Exit";
            icon: "red_exit_ico.svg";
            onButtonClick: tab.itemClick( 3 );
        }
    }
}
