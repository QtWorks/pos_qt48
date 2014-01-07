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
    property alias snap_to_item : ticket.snap_to_item;

    signal doneClick();
    signal payClick();
    signal cashOutClick();
    signal closed();
    signal transfer();

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

    function print_choice( print ) {
        if( print ) {
            TicketLib.API.print( ticket.sale_id );
            App.hide_choice();
        }
    }

    function hide_level_one() {
        main_actions.hide();
    }

    function hide_level_two() {
        extra_actions.hide();
        split_actions.hide();
        pay_actions.hide();
    }

    function combine_unload_check() {
        if( TicketLib.API.saleId === -1 ) {
            root.hide();
        }
        else {
            root.color =  Colors.index( TicketLib.API.tableID + 1 );
        }
    }

    function set_tip()
    {
        extra_actions.hide();
        App.show_number_input("Enter tip percent: ", set_tip_callback);
    }

    function set_tip_callback( tip )
    {
        TicketLib.API.set_sale_tip( ticket.sale_id, tip );
    }

    Button {
        id: doneb;
        anchors.bottom: ticket.bottom; anchors.bottomMargin: root.height * -0.05;
        anchors.right: parent.right; anchors.rightMargin: root.width * 0.01;
        height: root.height * 0.22;
        width: height;
        radius: 5;
        color: Colors.orange3;
        icon: "x_ico.svg";
        label: "Done";
        pixelSize: height * 0.2;
        font: "Chunkfive";
        onButtonClick : root.doneClick();
    }

    TicketView_payactionBar {
        id: pay_actions;
        anchors.top: parent.top; anchors.topMargin: height * 1.8;
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
        anchors.top: parent.top; anchors.topMargin: height * 1.8;
        color: "black";
        width: parent.width;
        height: parent.height/4;
        x: -width;
        to: 0;
        from: -width;
        property: "x";
    }
    TicketView_extraBar {
        id: extra_actions;
        anchors.top: parent.top; anchors.topMargin: height * 1.8;
        color: Colors.make( Colors.blue2, "99" );//"#88ffffff";
        width: parent.width;
        height: parent.height/4;
        x: -width;
        to: 0;
        from: -width;
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
            extra_actions.hide();
            split_actions.hide();
            pay_actions.toggle();
        }

        onDoneClick : {
            //root.doneClick();
        }
    }


    Ticket {
        id: ticket;
        y: -height;
        anchors.left: parent.left; anchors.leftMargin: parent.width * 0.015;
        allowOk: true;
        property bool active : false;
        ticketColor: Colors.gray_two;

        onPrintClick : {
            App.choice( "Print Ticket?", print_choice );
        }

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
            App.choice( "Print Receipt?", print_choice );
        }

        function print_choice( choice ) {
            hide(1000);
            var change  = total - TicketLib.API.owed;
            var paid = (cashmode) ? TicketLib.pay_cash( total, choice ) : TicketLib.pay_credit( total, choice );
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
