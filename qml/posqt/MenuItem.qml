import QtQuick 1.1
import "colors.js" as Colors;

Button {
    width: 200;
    height: 200;
    pointSize: 16;
    label: "";//item_name;

    signal itemClicked(variant _name, variant _id, variant _issub);

    onCancelClick : {
        itemClicked(item_name, item_id, item_issub);
    }

    onButtonClick : {
        itemClicked(item_name, item_id, item_issub);
    }

    Text {
        id: nametext
        anchors.bottom: parent.bottom; anchors.bottomMargin: parent.height * 0.04
        anchors.left: parent.left; anchors.leftMargin: parent.width * 0.25;
        anchors.right: parent.right; anchors.rightMargin: parent.width * 0.04;
        color: "white";//Colors.tran_toolbar;

        text: item_name;
        font.family: "Helvetica";
        font.pixelSize: (item_name.length > 50) ? parent.height * 0.09 : parent.height * 0.11;
        font.capitalization: Font.AllUppercase;
        wrapMode: Text.Wrap;
        horizontalAlignment: Text.AlignRight;
        elide: Text.ElideRight;
        maximumLineCount: 5;
    }

    Text {
        id: pricetext;
        anchors.top: parent.top; anchors.topMargin: parent.height * -0.02;
        anchors.left: parent.left; anchors.leftMargin: parent.width * 0.02;
        text: (item_price > 0.009) ? "$" + (item_price - item_price % 1) : "";
        color: "white";
        font.family: "Molot";
        font.pixelSize: parent.height * 0.23;
    }

    Text {
        id: floattext;
        visible: (item_price % 1);
        text: String((item_price % 1).toFixed(2)).slice(1);
        anchors.left: pricetext.left;
        anchors.top: pricetext.bottom;
        color: "white";
        font.family: "Molot";
        font.pixelSize: parent.height * 0.11;
    }

    Component.onCompleted : {
        if( item_name === "__ADD_ITEM__" ) {
            nametext.visible = false;
            icon = "plus_ico.svg";
            color = Colors.green3;
        }
        else if( item_name === "__BACK_ITEM__" ) {
            nametext.text = "back";
            icon = "refresh_ico.svg";
            color = Colors.red2;
        }
    }
}
