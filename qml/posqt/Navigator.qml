import QtQuick 1.1

Item {
    anchors.fill: parent;

    signal navClick();

    Button {
        anchors.right: parent.right;
        anchors.bottom: parent.bottom;
        icon: "logout_ico.svg";
        width: 128;
        height: 128;
        label: "Back/Log Out";
        onButtonClick : {
            parent.navClick();
        }
    }
}
