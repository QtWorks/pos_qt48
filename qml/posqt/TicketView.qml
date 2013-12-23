import QtQuick 1.1
import "colors.js" as Colors;
import "app.js" as App;
import "ticket.js" as TicketLib;
import "user.js" as User;

FadeView {
    id: root;
    duration : 300;
    dim: true;
    dimColor: Colors.make( "#000000", "cc" );

    property bool cashmode : true;
    property bool ticket_editable : false;
    property alias ticket_color : ticket.ticketColor;
    property alias ticket_sub_total : ticket.ticketSubTotal;
    property alias ticket_id : ticket.sale_id;
    property alias ticket_user : ticket.userName;

    signal doneClick();
    signal payClick();
    signal cashOutClick();
    signal closed();

    onHalfShown : {
        cashmode = true;
        ticket.show();
    }

    onClicked : {
        hide();
    }

    onHiding : {
        hide_level_one();
        hide_level_two();
    }

    function hide_level_one() {
        main_actions.hide();
    }

    function hide_level_two() {
        split_actions.hide();
        pay_actions.hide();
    }

    TicketView_payactionBar {
        id: pay_actions;
        anchors.top: parent.top; anchors.topMargin: height * 2;
        x: -width;
        to: 0;
        from: -width;
        property: "x";
        color: "black";
        width: parent.width;
        height: parent.height/4;
    }

    TicketView_splitactionBar {
        id: split_actions;
        anchors.top: parent.top; anchors.topMargin: height * 2;
        color: "black";
        width: parent.width;
        height: parent.height/4;
        x: -width;
        to: 0;
        from: -width;
        property: "x";
    }

    TicketView_actionBar {
        id: main_actions;
        y: root.height * 0.1;
        //anchors.top: ticket.top; //anchors.topMargin: parent.height * 0.05;
        height: parent.height / 3;
        width: parent.width - ticket.width;
        property: "x";
        x: parent.width;
        to: ticket.x + ticket.width + parent.width * 0.01;
        from: parent.width;

        onHid : {
            ticket.hide();
        }

        onPayClick : {
            split_actions.hide();
            pay_actions.toggle();
        }

        onDoneClick : {
            root.doneClick();
        }
    }

    Ticket {
        id: ticket;
        y: -height;
        anchors.left: parent.left; anchors.leftMargin: parent.width * 0.015;
        allowOk: true;
        property bool active : false;
        ticketColor: Colors.gray_two;

        onYChanged : {
            if( active && y >= (parent.height/2 - height/2)/2 ) {
                main_actions.show();
                active = false;
            }
        }

        Behavior on y
        {
            NumberAnimation {
                duration: 300;
                easing.type: Easing.InOutCirc;
            }
        }

        function show() {
            active = true;
            y = root.height/2 - height/2;
        }

        function hide() {
            active = false;
            y = -height;
        }
    }

    CashCountView {
        id: cashView;
        onReject : {
            hide( 1000 );
            pay_actions.hide();
        }
        onAccept : {
            hide(1000);
            var change  = total - TicketLib.API.owed;
            var paid = (cashmode) ? TicketLib.pay_cash( total ) : TicketLib.pay_credit( total );

            App.refresh();
            User.api.refresh();
            TicketLib.refresh();

            if( paid ) {
                root.hide();

                if( cashmode ) {
                    if( change > 0) {
                        App.warning( "Change :  $" + change.toFixed(2) );
                    }
                }
                closed();
            }
        }
    }

}
