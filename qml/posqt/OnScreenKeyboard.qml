import QtQuick 1.1
import "app.js" as App;
import "colors.js" as Colors;

Item {
    id: root;
    width: App.screen_w;
    height: App.screen_h;

    property bool shift : false;
    property bool caps : false;

    signal keyClick( string _key );

    function show() {
        spacerow.show(800);
        row3.show(800);
        row2.show(800);
        row0.show(600);
        row1.show(600);
    }

    function hide() {
        spacerow.hide();
        row3.hide();
        row2.hide();
        row1.hide();
        row0.hide();
    }

    Slide {
        id: spacerow;
        anchors.bottom: parent.bottom; anchors.bottomMargin: root.height * 0.01;

        property: "x";
        to: 0;
        from: -width;
        width: root.width;
        height: root.height * 0.07;
        x: -width;

        OSK_Button {
            label: "space";
            anchors.centerIn: parent;
            width: parent.width * 0.4;
            height: parent.height * 0.9;
        }
    }

    Slide {
        id: row0;
        anchors.bottom: spacerow.top;

        property: "x";
        to: 0;
        from: width;
        width: root.width;
        height: root.height * 0.07;
        x: width;

        Row {
            spacing: 5;

            anchors.centerIn: parent;
            OSK_Button { label: "shift"; width: root.height * 0.12; onButtonClick: { root.shift = !root.shift; } }
            OSK_Button { label: "z"; }
            OSK_Button { label: "x"; }
            OSK_Button { label: "c"; }
            OSK_Button { label: "v"; }
            OSK_Button { label: "b"; }
            OSK_Button { label: "n"; }
            OSK_Button { label: "m"; }
            OSK_Button { label: "backspace"; width: root.height * 0.12; }
        }
    }

    Slide {
        id: row1;
        anchors.bottom: row0.top;

        property: "x";
        to: 0;
        from: -width;
        width: root.width;
        height: root.height * 0.07;
        x: -width;

        Row {
            spacing: 5;

            anchors.centerIn: parent;

            OSK_Button { label: "caps"; width: root.height * 0.12; onButtonClick: { root.caps = !root.caps; } }
            OSK_Button { label: "a"; }
            OSK_Button { label: "s"; }
            OSK_Button { label: "d"; }
            OSK_Button { label: "f"; }
            OSK_Button { label: "g"; }
            OSK_Button { label: "h"; }
            OSK_Button { label: "j"; }
            OSK_Button { label: "k"; }
            OSK_Button { label: "l"; }
        }
    }
    Slide {
        id: row2;
        anchors.bottom: row1.top;

        property: "x";
        to: 0;
        from: width;
        width: root.width;
        height: root.height * 0.07;
        x: width;

        Row {
            spacing: 5;

            anchors.centerIn: parent;

            OSK_Button { label: "q"; }
            OSK_Button { label: "w"; }
            OSK_Button { label: "e"; }
            OSK_Button { label: "r"; }
            OSK_Button { label: "t"; }
            OSK_Button { label: "y"; }
            OSK_Button { label: "u"; }
            OSK_Button { label: "i"; }
            OSK_Button { label: "o"; }
            OSK_Button { label: "p"; }
        }
    }
    Slide {
        id: row3;
        anchors.bottom: row2.top;

        property: "x";
        to: 0;
        from: width;
        width: root.width;
        height: root.height * 0.07;
        x: width;

        Row {
            spacing: 5;

            anchors.centerIn: parent;

            OSK_Button { label: "1"; }
            OSK_Button { label: "2"; }
            OSK_Button { label: "3"; }
            OSK_Button { label: "4"; }
            OSK_Button { label: "5"; }
            OSK_Button { label: "6"; }
            OSK_Button { label: "7"; }
            OSK_Button { label: "8"; }
            OSK_Button { label: "9"; }
            OSK_Button { label: "0"; }
        }
    }
}
