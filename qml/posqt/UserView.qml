import QtQuick 1.1
import "app.js" as App;
import "ticket.js" as TicketLib
import "colors.js" as Colors;
import "user.js" as User;

Item {
    id: rootScreen;
    width: App.screen_w;
    height: App.screen_h;

    //properties
    property string uName: "";
    property string uTitle: "";
    property int uId: -1;
    property variant activeModel : App.get_saleModel();
    property int table_button_height : tableBackdrop.height * 0.95;
    property variant last_btn : undefined;
    //clip: true;

    //Backdrop{}
    Rectangle {
        anchors.fill: parent;
        color: "#aaffffff";
    }

    Component.onCompleted : {
        User.api.refresh();
        uName = User.api.firstName;
        uTitle = User.api.title;
        //Show timeclock slide w/o animation
        timeclock_slide.y = rootScreen.height * 0.01;// - timeclock_slide.height * 1.3;
        //Hide sale info slide w/o animation
        actions_slide.show( 0 );
        if( User.api.clockedin ) {
            timeclock_slide.y = rootScreen.height - timeclock_slide.height * 1.3;
            //actions_slide.show( 0 );//.x = 0;
            if( User.api.cashedin ) {
                //Hide timeclock slide w/o animation
                timeclock_slide.y = rootScreen.height;
            }

        }
    }

    function ticketCashedOut() {
        App.refreshActiveSales();
    }

    function selectSale(tableid, saleid) {
        App.activateTable(tableid);
        App.refresh();
        TicketLib.activate( saleid );
        TicketLib.refresh();

        ticket_view.ticket_editable = true;
        ticket_view.ticket_id = TicketLib.API.saleId;//saleid;
        ticket_view.show();
    }

    function activateSale(saleid) {
        TicketLib.activate(saleid);
        TicketLib.refresh();
        App.refresh();
    }

    Rectangle {
        id: tableBackdrop;
        color: "black";//"#a02c2a29";
        anchors.right : parent.right; //anchors.rightMargin: 1;
        anchors.left: parent.left; //anchors.leftMargin: 1;
        anchors.bottom: actions_slide.top; anchors.bottomMargin: parent.height * 0.15;

        height: App.screen_h * 0.35;

        Item {
            anchors.verticalCenter: parent.verticalCenter;
            anchors.right: add_btn.left; anchors.rightMargin: 10;
            height: table_button_height;
            width: height * 3 + (3 * 10);

            ListView {
                id: currentTableList;
                anchors.fill: parent;
                model: activeModel;
                spacing: 10;
                clip: true;
                delegate : tableDelegate;
                orientation: ListView.Horizontal;
                Component {
                    id: tableDelegate;
                    TableItem {
                        width: table_button_height;
                        height: table_button_height;
                        radius: 5;
                        color: Colors.index( table_id + 1 );
                        sensitive: false;
                        onButtonClick : {
                            ticket_view.color = Colors.make( color, "dd" );
                            TicketLib.tableid = table_id;
                            selectSale(table_id, sale_id);
                            currentTableList.positionViewAtIndex( 0, ListView.Beginning );
                        }
                    }
                }
            }
        }

        Image {
            source: "table_active_ico.svg";
            sourceSize.height: table_button_height * 0.5;
            sourceSize.width: table_button_height * 0.5;

            anchors.left: parent.left; anchors.leftMargin: sourceSize.height * 0.35;
            anchors.verticalCenter: parent.verticalCenter;
        }

        Button {
            id: add_btn;
            anchors.right: parent.right; anchors.rightMargin: 10;//width * 0.15;
            anchors.verticalCenter: parent.verticalCenter;

            height: table_button_height;
            width: height;
            radius: 10;

            icon: "plus_ico.svg";
            color: Colors.main_green//2;
            textColor: "black";
            pointSize: 35;
            label: "Add";


            onButtonClick: {
                App.showTableScreen();
            }
        }
    }

    Slide {
        id: actions_slide;
        height: parent.height * 0.35;
        width: parent.width;
        anchors.bottom: timeclock_slide.top; anchors.bottomMargin: parent.height * 0.1;
        property: "x";
        x: -width;
        from: -width;
        to: 0;
        color: "black";
        visible: User.api.merchant;
        clip: true;

        Image {
            id: actions_ico;
            source: "tag_ico.svg";
            sourceSize.height: parent.height * 0.9;
            sourceSize.width: parent.height * 0.9;
            anchors.right: parent.right; anchors.rightMargin: parent.width * 0.01;
            anchors.top: drawer_select.bottom; anchors.topMargin: (parent.height - sourceSize.height)/2;
            //anchors.verticalCenter: parent.verticalCenter;
        }

        Button {
            id: staff_bank_btn;
            width: parent.height * 0.95;
            height: width;
            pixelSize: height * 0.2;
            anchors.left: drawer_select.left;
            anchors.top: drawer_select.bottom; anchors.topMargin: parent.height * 0.025;
            icon: "register_ico.svg";
            font: "Molot";

            label: "Cashier";

            visible: User.api.clockedin && User.api.cashier && !User.api.cashedin;

            onButtonClick : {
                drawer_select.show();
            }
        }

        Button {
            id: cashier_btn;
            width: parent.height * 0.95;
            height: width;
            pixelSize: height * 0.1;
            anchors.left: staff_bank_btn.right; anchors.leftMargin: parent.width * 0.01;
            anchors.top: staff_bank_btn.top;
            label: "Staff Bank";
            font: "Molot"
            icon: "cash_ico.svg";

            visible : !User.api.cashedin && User.api.merchant;

            onButtonClick : {
                User.start_staff_bank();
                User.api.refresh();
                timeclock_slide.hide(1000);
            }
        }

        Button {
            id: cashout;
            width: parent.height * 0.95;
            height: width;
            pixelSize: height * 0.2;
            textColor: Colors.yellow3;
            anchors.left: parent.left; anchors.leftMargin: parent.width * 0.01;
            anchors.verticalCenter: parent.verticalCenter;
            font: "Molot";
            label: "Cashout";

            visible: User.api.cashedin && !User.api.active_sales;

            onButtonClick : {
                cash_count_view.show();
            }
        }

        Text {
            id: sales_info_text;
            font.pixelSize: parent.height * 0.3;
            font.family: "Molot";
            anchors.right: actions_ico.left; anchors.rightMargin: parent.width * 0.01;
            anchors.bottom: parent.bottom;
            color: "white";
            visible: User.api.cashedin;
            text: "Pending: $" + (User.api.pending_sales - User.api.credit_received).toFixed(2);
        }

        Text {
            id: total_text;
            anchors.top: parent.top;
            anchors.right: actions_ico.left; anchors.rightMargin: parent.width * 0.01;
            font.pixelSize: parent.height * 0.3;
            font.family: "Molot";
            color: "white";
            visible: User.api.cashedin;
            text: "Total: $" + User.api.total_sales.toFixed(2);
        }
        Slide {
            id: drawer_select;
            y: -height;
            from: -height;
            to: 0;
            catch_input: true;
            anchors.right: parent.right;
            height: parent.height;
            width: parent.width;

            onClicked: {
                hide();
            }

            Image {
                id: register_icon;
                source: "register_ico.svg";
                sourceSize.height: parent.height * 0.9;
                sourceSize.width: parent.height * 0.9;
                anchors.right: parent.right; anchors.rightMargin: parent.width * 0.01;
                anchors.verticalCenter: parent.verticalCenter;
            }
            Text {
                font.family: "Chunkfive";
                wrapMode: Text.Wrap;
                elide: Text.ElideRight;
                color: "white";
                text: "Select Drawer";
                font.pixelSize: parent.height * 0.20;
                anchors.right: register_icon.left; anchors.rightMargin: parent.width * 0.01;
                anchors.verticalCenter: parent.verticalCenter;
                verticalAlignment: Text.AlignVCenter;
            }

            Row {
                id: drawer_row;

                anchors.verticalCenter: parent.verticalCenter;
                anchors.left: parent.left; anchors.leftMargin: parent.width * 0.01;
                spacing: 10;

                Button {
                    width: drawer_select.height * 0.95;
                    height: width;
                    label: "Drawer One";
                    color: Colors.green5;
                    font: "Chunkfive";
                    pixelSize: height * 0.2;
                    radius: 10;
                    onButtonClick : {
                        drawer_select.hide( 100 );
                        cash_count_view.show();
                    }
                }
            }
       }
    }

    Slide {
        id: timeclock_slide;
        width: parent.width;
        height: parent.height * 0.35;//28;
        to: parent.height - height * 1.3;
        from: parent.height;
        y: parent.height;
        catch_input: true;
        color: (User.api.clockedin) ? Colors.red2 : Colors.green2;

        Image {
            anchors.right: parent.right; anchors.rightMargin: width * 0.2;
            anchors.verticalCenter: parent.verticalCenter;
            source: "clock_ico.svg";
            sourceSize.height: parent.height * 0.9;
            sourceSize.width: parent.height * 0.9;
        }

        Text {
            anchors.centerIn: parent;
            font.family: "Molot";
            font.pixelSize: parent.height * 0.5;
            color: "white";
            text: (User.api.clockedin) ? "Clock Out" : "Clock In";
        }

        Image {
            anchors.left: parent.left; anchors.leftMargin: width * 0.2;
            source: (User.api.clockedin) ? "x_ico.svg" : "check_white_ico.svg";
            sourceSize.height: parent.height * 0.9;
            sourceSize.width: parent.height * 0.9;
            anchors.verticalCenter: parent.verticalCenter;
        }

        onClicked : {
            if( running ) return;

            if( !User.api.clockedin ) {
                User.clockin();
                timeclock_slide.to = App.screen_h - timeclock_slide.height * 1.3;
                timeclock_slide.from = App.screen_h;
                timeclock_slide.show(500);
                return;
            }

            timeclock_slide.to = App.screen_h - timeclock_slide.height * 1.3;
            timeclock_slide.from = App.screen_h * 0.01;
            timeclock_slide.hide(500);
            var seconds = User.clockout();
            var s, m, h;
            if( seconds > 59 && seconds < 3600 ) {
                s = seconds % 60;
                m = (seconds / 60).toFixed(0);
                App.warning( "You were clocked in for: " + m + "m " + s + "s");
            }
            else if( seconds > 3599 ) {
                h = (seconds / 3600).toFixed(0);
                m = ((seconds - (h * 3600) ) / 60).toFixed(0);
                m = (m < 0) ? 0 : m;
                s = seconds - (h * 3600 + m * 60);
                s = ( s < 0 ) ? 0 : s;
                App.warning( "You were clocked in for: " + h + "h " + m + "m " + s + "s");
            }
            else {
                App.warning( "You were clocked in for: " + seconds + "s" );
            }
        }
    }



    DrawerCashCounter {
        id: cash_count_view;

        onHalfHidden : {
            if ( User.api.cashedin )
                timeclock_slide.hide(500);
        }
    }

    TicketView {
        id: ticket_view;
        width: App.screen_w;
        height: App.screen_h
        ticket_user: User.title + ": " + User.first_name;
        snap_to_item: false;

        onDoneClick : {
            hide();
        }

        onTransfer : {

        }

        onClosed : {
            hide();
        }
    }

    CashOutView {
        id: cashout_result_view;
        dim: true;
        color: Colors.make( Colors.violet3, "cc" );
        dimColor: Colors.make( "000000", "99" );

        onHidden : {
            if( !User.api.cashedin )
                timeclock_slide.show(500);
        }
    }

    TimeBar {}

    UserIcon {
        userName: uName;
        userTitle: uTitle;
    }

    function transfer_check( password ) {
        if( App._API.check_user_level( _password ) === 2 ) {
            return 1;
        }
    }
}
