import QtQuick 1.1
import "colors.js" as Colors;
import "app.js" as App;

FadeView {
    id: root;

    property string text : "";
    property int max : 128;

    dim: true;
    duration: 300;
    dimColor: "#99000000";
    color: Colors.make( Colors.blue2, "cc" );

    signal accept( string _text);
    signal reject();

    onHalfShown : {
        text_slide.show( 600 );
        App.show_osk( key_click );
    }

    onHiding : {
        text_slide.hide();
        App.hide_osk();
        text = "";
    }

    function key_click( key )
    {
        if( text.length === root.max ) {
            return;
        }

        if( key === "backspace" ) {
            if( text.length )
                text = text.slice(0, -1);
        }
        else {
            text += key;
        }
    }

    Slide {
        id: text_slide;

        width: root.width;
        height: root.height * 0.3;
        color: "black";
        to: root.height/2 - height;
        from: -height;
        y: -height;

        Button {
            id: rejectb;
            anchors.left: parent.left; anchors.leftMargin: parent.width * 0.01;
            anchors.verticalCenter: parent.verticalCenter;

            radius: 5;
            icon: "x_ico.svg";
            label: "Cancel";
            color: Colors.red2;
            pixelSize: height * 0.15;
            height: parent.height * 0.95;
            width: parent.height * 0.95;
            font: "Chunkfive";

            onButtonClick: reject();
        }

        Text {
            color: "white";
            text: Number(root.max - root.text.length);
            font.family: "Helvetica";
            font.pointSize: 40;
            anchors.bottom: parent.bottom; anchors.bottomMargin: parent.height * 0.01;
            anchors.horizontalCenter: parent.horizontalCenter;
        }

        Text {
            id: input;
            //anchors.centerIn: parent;
            anchors.left: rejectb.right;
            anchors.right: acceptb.left;
            anchors.verticalCenter: parent.verticalCenter;
            width: parent.width * 0.8;
            focus: true;
            color: "white";
            horizontalAlignment: Text.AlignHCenter;
            verticalAlignment: Text.AlignVCenter;
            font.family: "Chunkfive";
            font.pixelSize: (text.length > 32 ) ? parent.height * 0.25 * (32 / text.length) : parent.height * 0.25;
            wrapMode: Text.Wrap;
            text: root.text + "|"
        }

        Button {
            id: acceptb;
            anchors.right: parent.right; anchors.rightMargin: parent.width  * 0.01;
            anchors.verticalCenter: parent.verticalCenter;

            radius: 5;
            icon: "save_ico.svg";
            label: "Save";
            color: Colors.green3;
            pixelSize: height * 0.15;
            font: "Chunkfive";
            height: parent.height * 0.95;
            width: parent.height * 0.95;
            onButtonClick: {
                accept( root.text );
            }
        }
    }
}
