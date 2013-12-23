import QtQuick 1.1

Image {
    id: userIcon;

    property string userName: "User";
    property string userTitle: "Title";
    anchors.bottom : parent.bottom;
    anchors.left: parent.left;

    source: "user_ico.svg";
    sourceSize: "64x64";

    Text {
        id: usernametext;
        anchors.bottomMargin: 10;
        anchors.bottom: parent.bottom;
        anchors.left: parent.right;
        font.family: "Helvetica";
        font.pointSize: 15;
        text: userName + " (" + userTitle + ")";
        color: "white";
    }
}
