import QtQuick 1.1
import "colors.js" as Colors;

FadeView {
    id: root;
    dim: true;
    duration: 300;

    property alias accept_text : accept_btn.label;
    property alias reject_text : reject_btn.label;
    property alias total : cash_input.total;
    default property alias children : content.children;

    signal accept();
    signal reject();

    onHalfShown : {
        pay_action_slide.show();
        cash_input.clear();
        cash_input.show();
        input.show();
    }


    onHiding : {
        cash_input.hide();
        pay_action_slide.hide();
        input.hide();
    }

    Item { anchors.fill: parent; id: content }

    Slide {
        id: pay_action_slide;
        property: "x";
        from: -width;
        to: 0;
        color: "#aa000000";
        x: -width;
        width: parent.width;
        height: parent.height * 0.2;

        anchors.bottom: parent.bottom; anchors.bottomMargin: parent.height * 0.3;

        Row {
            anchors.verticalCenter: parent.verticalCenter;
            anchors.right: parent.right; anchors.rightMargin: parent.width * 0.4;

            spacing: 10;
            Button {
                id: reject_btn;
                pointSize: 20;
                color: Colors.red2;
                label: "Cancel";
                icon: "x_ico.svg";
                height: pay_action_slide.height * 0.9;
                width: height;
                radius: 10;
                onButtonClick : reject();

            }
            Button {
                id: accept_btn;
                pointSize: 20;
                color: Colors.green3;
                icon: "check_white_ico.svg";
                label: "Pay";
                width: pay_action_slide.height * 0.9;
                height: width;
                radius: 10;
                onButtonClick : accept();
            }
        }
    }

    CashInput {
        id: cash_input;
        show_input: false;

        spacing: 10;
    }

    Slide {
        id: input;
        color: "#99000000";
        anchors.bottom: parent.bottom; anchors.bottomMargin: parent.height * 0.04;
        height: parent.height * 0.25; width: parent.width; x: width;
        property: "x"; to: 0; from: width;

        Image {
            id: total_ico;
            source: "tag_ico.svg"
            sourceSize.height: root.height * 0.2;
            sourceSize.width: root.height * 0.2;
            anchors.left: parent.left; anchors.leftMargin: parent.width * 0.02;
            anchors.verticalCenter: parent.verticalCenter;
        }

        Text {
            anchors.left: display_text.left;
            anchors.top: display_text.top;
            anchors.bottom: display_text.bottom;
            font.family: "Molot";
            font.pixelSize: root.height * 0.1;
            color: "#22595959";
            text: cash_input.total.toFixed(2);
        }

        Text {
            id: display_text;
            anchors.centerIn: parent;
            font.family: "Molot";
            font.pixelSize: root.height * 0.1;
            color: "white";
            text: cash_input._raw;
        }
    }

}
