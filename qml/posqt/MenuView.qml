import QtQuick 1.1
import "app.js" as App;
import "ticket.js" as TicketLib;
import "menu.js" as MenuLib;
import "colors.js" as Colors;
import "menu_functions.js" as Methods

Item {
    id: root;
    width: App.screen_w;
    height: App.screen_h;
    property int catCelHeight : 200;
    property bool showCat : true;
    property string selected_category_name : "Category List";
    property int    current_cat : -1;
    property string selected_item_name  : "Item";
    property int    selected_item_id    : 0;
    property string subitem_name : "Sub-Item";
    property bool    showGrid : false;
    property variant catModel : MenuLib.get_catModel();
    property variant itemModel : MenuLib.get_itemModel();
    property int catcount : 0;
    //property variant lastClicked : null;

    //Method bindings
    /*property variant selectCategory : Methods.selectCa    CashInput {
        id: cash_count_input;
        show_input: false;
        spacing: 5;
    }tegory;
    property variant selectItem : Methods.selectItem;
    property variant navigateTo : Methods.navigateTo;
    property variant finalAction : Methods.finalAction;
    property variant backButtonPressed : Methods.backButtonPressed;
*/

    function modify_item(userid) {
        cash_input.show();
    }

    function void_item(userid) {
        TicketLib.void_item( ticket.selected_id , userid);
        TicketLib.refresh();
        ticket.ticketSubTotal= TicketLib.API.subTotal;
        ticket.ticketOwed = TicketLib.API.owed;
    }

    Component.onCompleted : {
        //qmlSet a callback when a user is leaving this screen
        App.setBackCB( Methods.backButtonPressed );
        ticket.sale_id = TicketLib.API.saleId;
        ticket.orderListModel = TicketLib.get_list();
        ticket.y = App.screen_h * 0.005;//38;
        //TicketLib.refresh();
        catlist.positionViewAtIndex( 0, ListView.Beginning );

        Methods.ticketlib   = TicketLib;
        Methods.app         = App;
        Methods.menulib     = MenuLib;

        //itemBox.show(1000);
        gridbox.show(700);
    }

    Rectangle {
        color: "#aaffffff";//Colors.make( Colors.blue4, "cc" );
        //height: parent.height * 0.35;
        //width: parent.width;
        anchors.fill: parent;
        x: 0;
        y: parent.height - height;
    }

    Component {
        id: menuCatDelegate;
        MenuCategory {
            id: cat;
            cat_name : item_name;
            cat_id : item_id;
            //tc: hgh.color;
            //default_height: height;//catlist.height/4;
            //default_width: width;//catlist.width;
            height: itemBox.height * 0.245;
            width: itemBox.width * 0.95;

            onCatClick : {
                select();
                var index = Methods.selectCategory(cat_name, cat_id);
                catlist.positionViewAtIndex(index, ListView.beginning );
                MenuLib.select( cat_id );
                //itemModel = MenuLib.get_itemModel();
                item_actions.hide();
                gridbox.show();
            }
            onItemClick : {
                selectItem(_name, _id, _issub);
                item_actions.hide();
            }

            Component.onCompleted : {
                //MenuLib.bind_category( cat, cat_id );
                var i = MenuLib.map_cat_color( cat_id );
                tc = Colors.indexMenu( i );
                //color = "black";
                catcount++;
            }
        }
    } //component
    //HorizontalSlide {
    Item {
        id: itemBox;
        //anchors.left: parent.left;
        anchors.right: parent.right; anchors.rightMargin: root.width * 0.0002;
        anchors.top: parent.top; anchors.topMargin: root.height * 0.007;
        //anchors.bottom: ticket.bottom;
        //anchors.leftMargin: 10;
        height: ticket.height * 0.98;
        width: height * 0.50;
        //to: root.width - width;// - root.width * 0.0005;
        //from: parent.width;
        x: parent.width;
        //fade: true;
        //height: parent.height * 0.7;
        ListView {
            id: catlist;

            spacing: height * 0.005;
            cacheBuffer: 1000; //NOTE: QML deletes the damn category when its not viewable... This fixes it. Badly
            delegate: menuCatDelegate;
            anchors.fill: parent;
            model: catModel;
            clip: true;
            //orientation: ListView.Horizontal;
        }
    } //itemBox

    VerticalSlide {
        id: gridbox;
        anchors.left: ticket.right; anchors.leftMargin: root.width * 0.005;
        //anchors.top: root.top; anchors.topMargin: root.height * 0.01;
        //anchors.bottom: itemBox.bottom;// anchors.bottomMargin: root.height * 0.2;
        //anchors.right: itemBox.left; anchors.rightMargin: root.width * 0.005;
        height: ticket.height * 0.98;
        width: root.width * 0.4225;//itemBox.x - to - root.width * 0.005;
        to: root.height * 0.01;
        //to: ticket.x + ticket.width + root.width * 0.005;
        from: -height;//root.width;
        y: -height;
        //x: root.width;
        reshow: true;
        color: "#ccffffff";
        radius: 10;
        GridView {
            id: itemgrid;
            anchors.top: parent.top; anchors.topMargin: 5;
            anchors.left: parent.left; anchors.leftMargin: 5;
            anchors.right: parent.right; anchors.rightMargin: 5;
            anchors.bottom: parent.bottom; anchors.bottomMargin: 5;

            cellHeight: cellWidth;
            cellWidth: width * 0.245;
            delegate: itemdelegate;
            clip: true;
            model: MenuLib.api.itemModel;
            interactive: MenuLib.api.itemModel.count > 16;
            //flickableDirection: Flickable.VerticalFlick;
            //pressDelay: 0;
            boundsBehavior: Flickable.StopAtBounds;
            flow: GridView.TopToBottom;
            /*add : Transition {
                NumberAnimation { properties: "x"; from: gridbox.width; duration: 300; easing.type: Easing.InOutCirc; }
                NumberAnimation { property: "opacity"; from: 0; to: 1.0; duration: 600; easing.type: Easing.InOutCirc; }
            }*/


            Component {
                id: itemdelegate;
                MenuItem {
                    color : Colors.indexMenu( MenuLib.map_cat_color(item_catid) );
                    width: itemgrid.width * 0.24;
                    height: width;
                    //sensitive: true;
                    //act_on_cancel: true;
                    onItemClicked : {
                        item_actions.hide();
                        if( MenuLib.hassubs(_id) )
                            gridbox.show();
                        Methods.selectItem(_name, _id, _issub);
                    }
                }
            }
        }
    }

    Ticket {
        id: ticket;
        anchors.left: parent.left; anchors.leftMargin: parent.width * 0.015;
        //x: parent.width/2 - width * 1.5;
        y: (parent.height/2 - height/2);
        ticketTaxPercent : TicketLib.TaxPercent;
        ticketSubTotal : TicketLib.API.subTotal;
        showEditButton: false;
        showOrderButton: true;

        Behavior on y { PropertyAnimation { duration: 500; easing.type: Easing.InOutCirc; } }

        onItemClick : {
            var index = Methods.selectCategory( "name", _cat_id );
            catlist.positionViewAtIndex(index, ListView.beginning );
            if( MenuLib.hassubs(_item_id) ) {
                Methods.navigateTo( _item_id );
            }
            else {
                Methods.navigateTo( _cat_id );
            }
            gridbox.show();
            sub_item_actions.hide();
            adjust_actions.hide();

            item_actions.show();
        }

        onFinalItemClick : {
            catlist.positionViewAtIndex(Methods.selectCategory( "", _cat_id ), ListView.beginning );
            if( MenuLib.hassubs( _item_id ) ) {
                Methods.navigateTo( _item_id );
            }
            else {
                Methods.navigateTo( _cat_id );
            }
            gridbox.show();
            sub_item_actions.hide();
            adjust_actions.hide();

            item_actions.show();
        }

        onSubItemClick : {
            item_actions.hide();
            adjust_actions.hide();
            sub_item_actions.show();
        }

        onLongClick : {
            item_actions.hide();
            sub_item_actions.hide();
            adjust_actions.show();
        }
        Component.onCompleted : {
            TicketLib.selectTicket( ticket );
        }

        onPrintClick : {
            TicketLib.API.print( sale_id );
        }
    }
    Slide {
        id: sub_item_actions;
        anchors.top: item_actions.top;
        //anchors.left: ticket.right; anchors.leftMargin: 10;
        x: -width;
        height: item_actions.height;
        width: item_actions.width;
        color: "black";
        from: -width;
        to: 0;
        property: "x";
        duration: 300;
        reshow: true;
        Text {
            anchors.right: parent.right; anchors.rightMargin: parent.width * 0.025;
            anchors.verticalCenter: parent.verticalCenter;
            text: "Modify";
            font.family: "Chunkfive";
            font.pixelSize: parent.height * 0.28;
            color: "white";
        }

        Row {
            anchors.verticalCenter: parent.verticalCenter;
            anchors.left: parent.left; anchors.leftMargin: parent.width * 0.02;
            spacing: 10;

            Button {
                //anchors.horizontalCenter: parent.horizontalCenter;
                width: item_actions.height * 0.9;
                height: width;
                color: Colors.orange3;
                label: "Void";
                icon: "x_ico.svg";
                radius: 5;

                onButtonClick : {
                    Methods.manager_action = void_item;
                    if( ticket.item && ticket.item.item_isfinal ) {
                        admin_check.show();
                        //App.warning( "Can't void a finalized Item" );
                    }
                    else {
                        TicketLib.delete_item( ticket.selected_id );
                        TicketLib.updateListAdd();
                        //TicketLib.refresh();
                        ticket.ticketSubTotal = TicketLib.API.subTotal;
                        ticket.ticketOwed = TicketLib.API.owed;
                    }
                    sub_item_actions.hide();
                }
            }
        }
    }
    Slide {
        id: adjust_actions;
        anchors.top: item_actions.top;
        x: -width;
        height: item_actions.height;
        width: item_actions.width;
        color: "black";
        property: "x";
        from: -width;
        to: 0;
        reshow: true;
        duration: 300;
        onClicked : { hide(); }
        Text {
            anchors.right: parent.right; anchors.rightMargin: parent.width * 0.025;
            anchors.verticalCenter: parent.verticalCenter;
            text: "Adjust";
            font.family: "Chunkfive";
            font.pixelSize: parent.height * 0.28;
            color: "white";
        }

        Row {
            anchors.verticalCenter: parent.verticalCenter;
            anchors.left: parent.left; anchors.leftMargin: parent.width * 0.025;
            //anchors.horizontalCenter: parent.horizontalCenter;
            //anchors.top: parent.top; anchors.topMargin: item_actions.width * 0.2;
            spacing: 10;

            Button {
                //anchors.horizontalCenter: parent.horizontalCenter;
                width: adjust_actions.height * 0.95;
                height: width;
                color: Colors.green3;
                radius: 5;
                icon: "plus_minus_ico.svg";
                label: "Adjust Price";

                onButtonClick : {
                    Methods.manager_action = modify_item;
                    admin_check.show();
                }
            }

            Button {
                //anchors.horizontalCenter: parent.horizontalCenter;
                width: item_actions.height * 0.95;
                height: width;
                color: Colors.orange3;
                label: "Void";
                icon: "x_ico.svg";
                radius: 5;
                onButtonClick : {
                    Methods.manager_action = void_item;
                    if( ticket.item && ticket.item.item_isfinal ) {
                        admin_check.show();
                        //App.warning( "Can't void a finalized Item" );
                        adjust_actions.hide();
                    }
                    else {
                        TicketLib.delete_item( ticket.selected_id );
                        TicketLib.updateListAdd();

                        ticket.ticketSubTotal = TicketLib.API.subTotal;
                        ticket.ticketOwed = TicketLib.API.owed;
                        adjust_actions.hide();
                    }
                }
            }
        }
    }

    HorizontalSlide {
        id: item_actions;
        //anchors.right: ticket.right;
        //anchors.left: ticket.right; anchors.leftMargin: 10;
        anchors.top: ticket.bottom; anchors.bottomMargin: root.height * 0.004;
        x: -width;
        height: parent.height * 0.2;//height;
        width: parent.width;// * 0.6;//parent.width / 9;
        color: "#cc000000";
        reshow: true;
        from: -width;
        catch_input: true;
        onClicked: toggle();
        //fade: true;
        to: 0;
        duration: 300;


        property int item_menu_id : 0;
        property int cat_menu_id : 0;

        Text {
            anchors.right: parent.right; anchors.rightMargin: parent.width * 0.025;
            anchors.verticalCenter: parent.verticalCenter;
            text: "Modify";
            font.family: "Chunkfive";
            font.pixelSize: parent.height * 0.28;
            color: "white";
        }

        Row {
        //Column {
            //anchors.horizontalCenter: parent.horizontalCenter;
            anchors.verticalCenter: parent.verticalCenter;
            anchors.left: parent.left; anchors.leftMargin: item_actions.width * 0.025;
            //anchors.top: parent.top; anchors.topMargin: item_actions.width * 0.2;
            spacing: 10;

            Button {
                //anchors.horizontalCenter: parent.horizontalCenter;
                width: item_actions.height * 0.95;
                height: width;
                color: Colors.blue3;
                icon: "comment_ico.svg";
                label: "Comment";
                pixelSize: height * 0.15;
                radius: 5;
                onButtonClick : comment_screen.show();
            }

            Button {
                //anchors.horizontalCenter: parent.horizontalCenter;
                width: item_actions.height * 0.95;
                height: width;//item_actions.width * 0.9;
                color: Colors.green3;
                label: "Refill";
                icon: "refresh_ico.svg";
                radius: 5;
                pixelSize: height * 0.15;

                onButtonClick : {
                    Methods.selectCategory( "", ticket.item.item_parentid );
                    navigateTo( ticket.item.item_menuid );
                    navigateTo( ticket.item.item_menuid );
                }
            }

            Button {
                //anchors.horizontalCenter: parent.horizontalCenter;
                width: item_actions.height * 0.95;
                height: width;//item_actions.width * 0.9;
                color: Colors.violet3;
                label: "Hold";
                icon: "pause_white_ico.svg";
                pixelSize: height * 0.15;
                radius: 5;
            }

            Button {
                //anchors.horizontalCenter: parent.horizontalCenter;
                width: item_actions.height * 0.95;
                height: width;//item_actions.width * 0.9;
                color: Colors.red2;
                label: "Fire";
                icon: "fire_ico.svg";
                pixelSize: height * 0.15;
                radius: 5;
            }
        }
    }



    FadeView {
        id: final_action_screen;
        dim: true;
        duration: 300;
        dimColor: Colors.make( "#000000", "99" );
        color: Colors.make( Colors.blue2, "22" );

        onHalfShown : {
            final_action_slide.show();
        }

        Slide {
            id: final_action_slide;
            width: parent.width;
            height: parent.height * 0.3;
            color: "black";
            to: 0;
            x: -width;

        }
    }

    TextInputView {
        id: comment_screen;
        max: 80;

        onAccept : {
            MenuLib.add_comment( comment_screen.text );
            TicketLib.updateListAdd();
            hide();
        }

        onReject : {
            hide();
        }
    }

    FadeView {
        id: admin_check;
        duration: 300;
        color: Colors.make( Colors.violet2, "22" );
        dim: true;
        dimColor: Colors.make( "#000000", "bb" );

        onHalfShown : {
            basic_pad.show();
        }

        onClicked : {
            hide();
            basic_pad.hide();
        }

        onHiding : {
            basic_pad.hide();
        }

        LoginPad {
            id: basic_pad;
            bColor: Colors.make( Colors.blue3, "aa" );

            onOkClick : {
                if( App._API.check_user_level( _password ) === 2 ) {
                    admin_check.hide();
                    Methods.manager_action( App.get_user_id( _password ) );
                    Methods.manager_action = null;
                }
                else {
                    basic_pad.fail();
                }
            }

            onXClick : {
                admin_check.hide();
                Methods.manager_action = null;
            }
        }
    }

    CashCountView {
        id: cash_input;
        color: Colors.make( Colors.blue3, "aa" );
        accept_text: "Modify";
        reject_text: "Cancel";

        onReject : {
            hide();
        }

        onAccept : {
            TicketLib.API.modify_price( total );
            TicketLib.API.updateListAdd();
            ticket.ticketSubTotal = TicketLib.API.subTotal;
            ticket.ticketOwed = TicketLib.API.owed;
            hide();
        }

        Text {
            anchors.left: parent.left; anchors.leftMargin: parent.width * 0.05;
            anchors.top: parent.top; anchors.topMargin: parent.height * 0.1;
            font.family: "Chunkfive";
            font.pixelSize: parent.height * 0.1;
            color: "white";
            text: "Enter new amount: ";
        }

    }

}//root
