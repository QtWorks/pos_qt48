import QtQuick 1.1
import "colors.js" as Colors;
import "user.js" as User;

FadeView {
    id: root;
    property alias total : cash_count_input.total;

    color: Colors.make( Colors.violet3, "aa" );
    dim: true;
    dimColor: Colors.make( "#000000", "cc" );

    onHalfShown : {
        cash_count_input.clear();
        cash_count_slide.show();
        if( User.api.cashedin === 1) {
            //display_text.text = (User.api.bank + User.api.cash_received).toFixed(2);
            total_text.text = "Expected Cash:\n$ " + (User.api.bank + User.api.cash_received).toFixed(2);
            expected_slide.show();
        } else {
            total_text.text = "Enter Starting Amount: ";
            expected_slide.show(600);
        }
        cash_count_input.show();
    }

    onHiding : {
        cash_count_input.hide();
        cash_count_slide.hide();
        expected_slide.hide(600);
    }

    onClicked : {
        hide(1000);
    }
    Text {
        id: total_text;
        anchors.left: parent.left; anchors.leftMargin: parent.width * 0.025;
        anchors.top: parent.top; anchors.topMargin: parent.height * 0.1;
        font.family: "Chunkfive";
        font.pixelSize: root.height * 0.08;
        color: "white";
        text: "Expected Cash"
    }


    Slide {
        id: cash_count_slide;
        property: "x";
        from: -width;
        to: 0;
        x: -width;
        width: parent.width;
        height: parent.height * 0.2;

        anchors.bottom: parent.bottom; anchors.bottomMargin: parent.height * 0.30;

        Row {
            anchors.verticalCenter: parent.verticalCenter;
            anchors.right: parent.right; anchors.rightMargin: parent.width * 0.4;

            spacing: 5;
            Button {
                id: cash_count_cancel_btn;
                pointSize: 20;
                color: Colors.red2;
                label: "Cancel";
                icon: "x_ico.svg";
                height: cash_count_slide.height * 0.9;
                width: height;
                radius: 5;
                onButtonClick : {
                    root.hide( 1000 );
                }

            }
            Button {
                id: cash_count_done_btn;
                pointSize: 20;
                color: Colors.green3;
                icon: "check_white_ico.svg";
                label: "Finish";
                width: cash_count_slide.height * 0.9;
                height: width;
                radius: 5;
                onButtonClick : {
                    if( !User.api.cashedin ) {
                        User.make_cashier( cash_count_input.total );
                    }
                    else {
                        cashout_result_view.total = User.api.total_sales.toFixed(2);
                        cashout_result_view.cash = User.api.cash_received;
                        cashout_result_view.tendered = User.api.cash_tendered;
                        cashout_result_view.credit = User.api.credit_received;
                        cashout_result_view.short_cash = User.cashout( cash_count_input.total );
                        cashout_result_view.show();
                    }
                    root.hide(1000);
                }
            }
         }
     }

    CashInput {
        id: cash_count_input;
        show_input: false;
        spacing: 5;
    }

    Slide {
        id: expected_slide;
        anchors.bottom: parent.bottom; anchors.bottomMargin: parent.height * 0.05;

        x: width;
        property: "x";
        from: width;
        to: 0;
        color: "black";
        height: parent.height * 0.25;
        width: parent.width;

        onClicked : { root.hide(1000); }

        Image {
            id: total_ico;
            source: "tag_ico.svg"
            sourceSize.height: root.height * 0.2;
            sourceSize.width: root.height * 0.2;
            anchors.left: parent.left; anchors.leftMargin: parent.width * 0.02;
            anchors.verticalCenter: parent.verticalCenter;
        }


        Text {
            id: display_text;
            anchors.centerIn: parent;
            font.family: "Molot";
            font.pixelSize: root.height * 0.1;
            color: "white";
            text: cash_count_input._raw;
            //text: (User.api.bank + User.api.cash_received).toFixed(2);
        }
    }


}
