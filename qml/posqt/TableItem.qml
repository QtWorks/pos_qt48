import QtQuick 1.1

Button {
    sensitive: true;
    Image {
        id: icon;
        anchors.bottom: sid.top; anchors.bottomMargin: parent.height * 0.01;
        anchors.right: parent.right; anchors.rightMargin: parent.width * 0.01;
        source: "table_no_legs_ico.svg";
        sourceSize.width: parent.width * 0.2;
        sourceSize.height: parent.width * 0.2;
    }

    Text {
        id: sid;
        anchors.bottom: parent.bottom; anchors.bottomMargin: parent.height * 0.01;
        anchors.right: parent.right; anchors.rightMargin: parent.width * 0.01;
        text: "SID#: " + sale_id;
        color: "white";
        font.family: "Molot";
        font.pixelSize: parent.height * 0.15;
    }

    Text {
        //anchors.horizontalCenter: parent.horizontalCenter;
        anchors.verticalCenter: icon.verticalCenter;
        anchors.right: icon.left; anchors.leftMargin: parent.width * 0.01;
        //anchors.right: icon.right; anchors.rightMargin: parent.width * 0.01;
        color: "white";
        font.family: "Molot";
        font.pixelSize: parent.height * 0.2;
        text: table_id + 1;
    }
    Text {
        id: pricetext;
        anchors.top: parent.top; anchors.topMargin: parent.height * -0.02;
        anchors.left: parent.left; anchors.leftMargin: parent.width * 0.02;
        text: (sale_amount > 0.009) ? qsTr("$") + (sale_amount - sale_amount % 1) : "$0";
        color: "white";
        font.family: "Molot";
        font.pixelSize: parent.height * 0.23;
    }
    Text {
        id: floattext;
        //visible: (sale_amount % 1);
        text: String((sale_amount % 1).toFixed(2)).slice(1);
        anchors.left: pricetext.left;
        anchors.top: pricetext.bottom;
        color: "white";
        font.family: "Molot";
        font.pixelSize: parent.height * 0.11;
   }
}
