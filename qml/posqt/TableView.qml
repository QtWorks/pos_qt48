import QtQuick 1.1
import "app.js" as App;
import "user.js" as User;
import "ticket.js" as TicketLib;
import "colors.js" as Colors;

Item {
    property variant table_model : App.get_tableModel();
    property int current_table : 0;
    width: App.screen_w;
    height: App.screen_h;

    Image {
        anchors.fill: parent;
        source: App.bg_filepath;
        fillMode: Image.PreserveAspectCrop;
    }

    Rectangle {
        anchors.fill: parent;
        color: "#99ffffff";
    }

    function showQuickview( condition ) {
        if( condition ) {
            quickview.show();
        }
        else {
            quickview.hide();
        }
    }

    function select_table( tbl ) {
        tbl.width = 100;
        tbl.height = 100;
    }

    function reservation_action_cb( choice )
    {
        if( choice ){
            App.unreserve_table( App.tableid );
            App.hide_choice();
            App.setBackCB( null );
            App.refresh();
            quickview.show();
        }
        else {
            App.setBackCB( null );
        }
    }

    function new_sale_with_guests( guest_num )
    {
        var sid = App.addNewSale( current_table );
        TicketLib.API.set_sale_guests( sid, guest_num );
        App.refresh();
    }

    Component {
        id: tableDelegate;
        Button{
            id: table;
            width: grid.width * 0.245;
            height: grid.height/3 * 0.99;
            radius: 5;

            color: Colors.index( table_id );

            textColor: "white";
            font: "Chunkfive";
            pixelSize: height * 0.1;
            label: table_name;
            Image {
                id: table_state_ico;
                anchors.left: parent.right;
                anchors.top: parent.top;
                anchors.leftMargin: -35;
                anchors.topMargin: -10;
                sourceSize.width: 44;
                sourceSize.height: 44;
            }
            onButtonClick : {
                if( !table_salecount || table_user == User.id ) {
                    ticket.ticket_editable = true;
                    add_sale_btn.visible = true;
                } else {
                    ticket.ticket_editable = false;
                    add_sale_btn.visible = false;
                }

                App.activateTable(table_id);
                if( table_reserved ) {
                    App.choice( "Clear Reservation:\n" + table_reserved, reservation_action_cb );
                    return;
                }

                current_table = table_id;
                quickview.color = Colors.make( color, "99" );
                quickview.show();
                App.refreshTables();
            }

            Component.onCompleted : {
                if( table_reserved ) {
                    icon = "banner_ico.svg";

                    if( table_user !== User.id ) {
                        table.color = Colors.grey2;
                    }

                    return;
                }

                if( table_salecount ) {
                    if( table_user !== User.id ) {
                        icon = "table_x_ico.svg";
                    }
                    else if( table_user === User.id ) {
                        icon = "table_active_ico.svg";
                    }
                }
                else {
                    icon = "check_white_ico.svg";
                }
            }
        }
    }

    Rectangle {
        anchors.fill: parent;
        color: Colors.transparent_white;

        GridView {
            id: grid;
            cellHeight: height / 3;
            cellWidth: width * 0.25;
            delegate: tableDelegate;
            anchors.centerIn: parent;
            width: parent.width * 0.6;
            height: parent.height * 0.8;
            model: table_model;
        }
    }
    FadeView {
        id: quickview;
        color: Colors.tran_toolbar;
        duration: 300;
        dim: true;
        dimColor: Colors.make( "#000000", "cc" );

        onHalfShown : {
            list_slider.show();
            actions_slider.show();
        }

        onClicked : hide();

        onHiding : {
            ticket.hide();
            hide();
            list_slider.hide();
            actions_slider.hide();
        }

        Slide {
            id: list_slider;
            anchors.top: parent.top; anchors.topMargin: height * 0.25;
            color: "black";
            height: App.screen_h * 0.37;
            property: "x";
            to: 0;
            from: -width;
            x: -width;

            Item { //Sales present at table(containter)
                anchors.verticalCenter: parent.verticalCenter;
                height: parent.height * 0.9;//App.screen_h * 0.37;
                anchors.left: parent.left; anchors.leftMargin: 10;
                anchors.right: add_sale_btn.left; anchors.rightMargin: parent.width * 0.01;

                ListView {
                    id: tableSalesList;
                    anchors.fill: parent;
                    model: App._API.tableSaleModel;
                    spacing: 10;
                    delegate: tableSaleDelegate;
                    orientation: ListView.Horizontal;
                    clip: true;

                    /*add : Transition {
                        NumberAnimation { property: "x"; from: list_slider.width; duration: 300; easing.type: Easing.InOutCirc; }
                    }*/

                    Component {
                        id: tableSaleDelegate;
                        TableItem {
                            id: saleButton;
                            color: Colors.index( table_id );
                            width: tableSalesList.height;
                            height: width;//list_slider.height;
                            radius: 5;
                            pointSize: 20;
                            onButtonClick: {
                                ticket.ticket_id = sale_id;
                                TicketLib.activate(sale_id);
                                TicketLib.refresh();
                                ticket.ticket_sub_total = TicketLib.API.subTotal.toFixed(2);
                                ticket.color = Colors.make( color, "99" );
                                current_table = table_id;
                                ticket.show();
                            }
                            Component.onCompleted : {
                                if( !ticket.ticket_editable ) {
                                    color = Colors.make( Colors.gray, Colors.opc_half );
                                }
                            }
                        }
                    }
                }
            }

            Button {
                id: add_sale_btn;
                height: parent.height * 0.9;
                width: parent.height * 0.9;
                color: Colors.green3;
                icon: "plus_ico.svg";
                radius: 5;

                anchors.right: parent.right; anchors.rightMargin: parent.width * 0.01;
                anchors.verticalCenter: parent.verticalCenter;

                onButtonClick : {
                    //var sid = App.addNewSale( App.tableid );
                    App.show_number_input("Number of Guests", new_sale_with_guests );
                }
            }
        }

        Slide {
            id: actions_slider;
            anchors.top: list_slider.bottom; anchors.topMargin: parent.height * 0.02;
            height: parent.height * 0.3;
            width: parent.width;
            property: "x";
            x: width;
            to: 0;
            from: width;

            Button {
                id: reserve_btn;
                anchors.right: parent.right; anchors.rightMargin: parent.width * 0.02 + parent.height * 0.9;
                anchors.verticalCenter: parent.verticalCenter;
                font: "Chunkfive";
                label: "Reserve";
                icon: "banner_ico.svg";
                radius: 5;
                pixelSize: height * 0.15;
                height: parent.height * 0.9;
                color: Colors.blue3;
                width: parent.height * 0.9;
                onButtonClick: {
                    App.reserve_table( App.tableid, "reserve" );
                    quickview.hide();
                }
            }

            Button {
                anchors.right: parent.right; anchors.rightMargin: parent.width * 0.01;
                anchors.verticalCenter: parent.verticalCenter;
                font: "Chunkfive";
                label: "Done";
                icon: "x_ico.svg";
                radius: 5;
                pixelSize: height * 0.15;
                height: parent.height * 0.9;
                width: parent.height * 0.9;
                color: Colors.red2;
                onButtonClick : quickview.hide();
            }
        }

        TicketView {
            id: ticket;
            ticket_user : User.title + ": " + User.first_name;

            onClosed : {
                App.activateTable( App.tableid );
                hide();
            }

            onDoneClick : {
                hide();
            }
        }
    }
}
