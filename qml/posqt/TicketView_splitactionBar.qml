import QtQuick 1.1
import "colors.js" as Colors;
import "app.js" as App;

Slide {
    id: root;

    Row {
        anchors.verticalCenter: parent.verticalCenter;
        anchors.right: parent.right; anchors.rightMargin: parent.width * 0.1;
        spacing: 10;

        Button {
            width: root.height * 0.98;
            height: width;
            icon: "even_ico.svg";
            font: "Chunkfive";
            label: "Evenly";
            pointSize: 20;
            radius: 10;
            color: Colors.pink2;
            onButtonClick : {
            }
        }
        Button {
            width: root.height * 0.98;
            height: width;
            icon: "barcode_ico.svg";
            font: "Chunkfive";
            label: "By Item";
            pointSize: 20;
            radius: 10;
            color: Colors.blue5;
            onButtonClick : {
                App.showSplitTicketScreen();
            }
        }
        Button {
            width: root.height * 0.98;
            height: width;
            icon: "plus_ico.svg";
            font: "Chunkfive";
            label: "Combine";
            pointSize: 20;
            radius: 10;
            color: Colors.green2;
            onButtonClick : {
                //App.load_subview("CombineView.qml", combine_unload_check);
            }
        }
    }
}
