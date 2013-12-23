import QtQuick 1.1
import "colors.js" as Colors
import "app.js" as App;

FadeView {
    id: root;
    width: App.screen_w;
    height: App.screen_h;
    //anchors.fill: parent;

    property bool show_expected : true;
    property double total : 0.0001;
    property double cash : 0.0;
    property double tendered : 0.0;
    property double credit : 0.0;
    property double short_cash : 0.0;

    onHalfShown : {
        //prevent divide by zero
        total = ( total ) ? total : 0.001;

        total_slide.show();
        tendered_slide.show(500);
        cash_slide.show(900);

        if( show_expected && tendered - cash > 0 )
            expected_slide.show(800);

        credit_slide.show(700);
        if( short_cash > 0.009 )
            App.warning("Cash SHORT : $" + short_cash.toFixed(2) + " !");
    }

    onHiding : {
        total_slide.hide(1000);
        tendered_slide.hide(900);
        credit_slide.hide(600);
        expected_slide.hide(500)
        cash_slide.hide(400);
    }

    onClicked : {
        hide(1000);
    }

    Slide {
        id: total_slide;
        anchors.top: parent.top; anchors.topMargin: parent.height * 0.02;

        x: -width;
        property: "x";
        from: -width;
        to: 0;
        color: "#cc000000";
        height: parent.height * 0.9;
        width: parent.width;

        onClicked : { root.hide(1000); }

        Image {
            id: total_ico;
            source: "tag_ico.svg"
            sourceSize.height: root.height * 0.15;
            sourceSize.width: root.height * 0.15;
            anchors.left: parent.left; anchors.leftMargin: parent.width * 0.02;
            anchors.verticalCenter: total_text.verticalCenter;
        }

        Text {
            id: total_text;
            anchors.left: parent.left; anchors.leftMargin: root.width * 0.1;
            anchors.top: parent.top; anchors.topMargin: root.height * 0.005;
            font.family: "Molot";
            font.pixelSize: root.height * 0.2;
            color: "white";
            text: "Total"
        }

        Text {
            anchors.right: parent.right; anchors.rightMargin: parent.width * 0.1;
            anchors.verticalCenter: total_text.verticalCenter;
            font.family: "Molot";
            font.pixelSize: root.height * 0.1;
            color: "white";
            text: total.toFixed(2);
        }
    }
    Slide {
        id: tendered_slide;
        anchors.top: total_slide.top; anchors.topMargin: parent.height * 0.25;

        x: -width;
        property: "x";
        from: -width;
        to: 0;
        color: Colors.make( Colors.grey3, "aa");
        height: (tendered > total) ? parent.height * 0.45 : parent.height * 0.2 + parent.height * 0.25 * (tendered / total);
        width: parent.width;

        onClicked : { root.hide(1000); }

        Image {
            source: "cash_ico.svg"
            sourceSize.height: root.height * 0.1;
            sourceSize.width: root.height * 0.1;
            anchors.left: parent.left; anchors.leftMargin: parent.width * 0.02;
            anchors.verticalCenter: tendered_text.verticalCenter;
        }

        Text {
            id: tendered_text;
            anchors.left: parent.left; anchors.leftMargin: root.width * 0.1;
            anchors.top: parent.top; anchors.topMargin: root.height * 0.005;
            font.family: "Molot";
            font.pixelSize: root.height * 0.1;
            color: "white";
            text: "Tendered"
        }

        Text {
            anchors.right: parent.right; anchors.rightMargin: parent.width * 0.1;
            anchors.verticalCenter: tendered_text.verticalCenter;
            font.family: "Molot";
            font.pixelSize: root.height * 0.1;
            color: "white";
            text: "$" + tendered.toFixed(2);
        }
    }
    Slide {
        id: cash_slide;
        anchors.bottom: tendered_slide.bottom;

        x: -width;
        property: "x";
        from: -width;
        to: 0;
        color: Colors.make( Colors.green2, "cc");
        height: parent.height * 0.2 + parent.height * 0.25 * (cash / total);
        width: parent.width;

        onClicked : { root.hide(1000); }

        Image {
            source: "cash_ico.svg"
            sourceSize.height: root.height * 0.1;
            sourceSize.width: root.height * 0.1;
            anchors.left: parent.left; anchors.leftMargin: root.width * 0.02;
            anchors.verticalCenter: cash_text.verticalCenter;
        }

        Text {
            id: cash_text;
            anchors.left: parent.left; anchors.leftMargin: root.width * 0.1;
            anchors.top: parent.top; anchors.topMargin: root.height * 0.1;
            font.family: "Molot";
            font.pixelSize: root.height * 0.1;
            color: "white";
            text: "Cash"
        }

        Text {
            anchors.right: parent.right; anchors.rightMargin: root.width * 0.1;
            anchors.verticalCenter: cash_text.verticalCenter;
            font.family: "Molot";
            font.pixelSize: root.height * 0.1;
            color: "white";
            text: "$" + cash.toFixed(2);
        }
    }
    Slide {
        id: expected_slide;
        anchors.bottom: tendered_slide.bottom;

        x: -width;
        property: "x";
        from: -width;
        to: 0;
        color: Colors.yellow3;
        height: parent.height * 0.1;
        width: parent.width;

        onClicked : { root.hide(1000); }

        Image {
            source: "cash_ico.svg"
            sourceSize.height: root.height * 0.1;
            sourceSize.width: root.height * 0.1;
            anchors.left: parent.left; anchors.leftMargin: root.width * 0.02;
            anchors.verticalCenter: expected_text.verticalCenter;
        }

        Text {
            id: expected_text;
            anchors.left: parent.left; anchors.leftMargin: root.width * 0.1;
            anchors.verticalCenter: parent.verticalCenter;
            font.family: "Molot";
            font.pixelSize: root.height * 0.1;
            color: "white";
            text: "Expected"
        }

        Text {
            anchors.right: parent.right; anchors.rightMargin: root.width * 0.1;
            anchors.verticalCenter: expected_text.verticalCenter;
            font.family: "Molot";
            font.pixelSize: root.height * 0.1;
            color: "white";
            text: "$" + (total - credit).toFixed(2);
        }
    }

    Slide {
        id: credit_slide;
        anchors.top: cash_slide.bottom; anchors.topMargin: root.height * 0.01;

        x: -width;
        property: "x";
        from: -width;
        to: 0;
        color: Colors.blue3;//Colors.make( Colors.blue3, "cc");
        height: parent.height * 0.15 + parent.height * 0.25 * (credit / total);
        width: parent.width;

        onClicked : { root.hide(1000); }

        Image {
            source: "credit_ico.svg"
            sourceSize.height: root.height * 0.1;
            sourceSize.width: root.height * 0.1;
            anchors.left: parent.left; anchors.leftMargin: root.width * 0.02;
            anchors.verticalCenter: credit_text.verticalCenter;
        }

        Text {
            id: credit_text;
            anchors.left: parent.left; anchors.leftMargin: parent.width * 0.1;
            anchors.top: parent.top; anchors.topMargin: root.height * 0.01;
            font.family: "Molot";
            font.pixelSize: root.height * 0.1;
            color: "white";
            text: "Credit/Debit"
        }

        Text {
            anchors.right: parent.right; anchors.rightMargin: root.width * 0.1;
            anchors.verticalCenter: credit_text.verticalCenter;
            font.family: "Molot";
            font.pixelSize: root.height * 0.1;
            color: "white";
            text: "$" + credit.toFixed(2);
        }
    }

    MouseArea {
        anchors.fill: parent;
        onClicked: root.hide(1000);
    }
}
