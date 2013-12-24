import QtQuick 1.1
import "colors.js" as Colors;
import "app.js" as App;
import "menu.js" as Menu;
import "settings.js" as Settings;
import "ticket.js" as TicketLib;

FadeView {
    id: root;
    width: App.screen_w;
    height: App.screen_h;
    property string cat_name : "";

    color: Colors.make( Colors.red2, "aa" );
    dim: true;
    dimColor: Colors.make( "#000000", "cc" );
    duration: 400;

    onHidden : {
        App.load_subview("");
    }

    onHalfShown : {
        cat_name = "";
        Menu.select( -1 );
        TicketLib.set_sale( -1 );
        whitebar.show();
        category_list_slide.show();
        whitebar_bottom.show();
    }

    onClicked : {
        hide(1000)

        whitebar.hide(800);
        item_edit_slide.hide(200);
        category_list_slide.hide(400);
        whitebar_bottom.hide(300);
    }

    Component.onCompleted : {
        show();
    }

    function change_item_name( newname )
    {
        Settings.api.set_itemname( newname );
        item_edit_slide.show();
    }

    function create_newitem( newname )
    {
        Settings.new_item(Settings.api.item_id, newname)
    }

    function onItemDelete( choice ) {
        if( choice ) {
            Settings.delete_item( Settings.api.item_id );
            item_edit_slide.hide();
            App.hide_choice();
        }
    }

    Image {
        source: "menu_ico.svg";
        sourceSize.width: parent.height * 0.25;
        sourceSize.height: parent.height * 0.25;
        anchors.left: parent.left; anchors.leftMargin: parent.width * 0.03;
        anchors.verticalCenter: parent.verticalCenter;
    }

    Slide {
        id: category_list_slide;
        anchors.right: parent.right; anchors.rightMargin: parent.width * 0.01;
        //anchors.left: item_edit_slide.right; anchors.leftMargin: parent.width * 0.01;
        y: -height;
        from: -height;
        to: 0;
        width: App.screen_w * 0.45;
        height: App.screen_h * 0.85;
        color: "#99000000";

        Item {
            anchors.top: parent.top; anchors.topMargin: parent.height * 0.15;
            anchors.bottom: parent.bottom; anchors.bottomMargin: parent.height * 0.01;
            anchors.left: parent.left; anchors.leftMargin: parent.width * 0.01;
            width: parent.width;

            //ListView {
            GridView {
                id: category_list;
                cellHeight: parent.height/3;
                cellWidth: parent.width/3;
                snapMode: GridView.SnapOneRow;
                flow: GridView.TopToBottom;
                boundsBehavior: Flickable.StopAtBounds;
                interactive: model.count > 9;
                //pressDelay: 0;

                Component {
                    id: category;
                    MenuItem {
                        height: category_list.height/3 - 10;
                        width: category_list.width/3 - 10;
                        color: Colors.green4;
                        //sensitive: true;
                        //act_on_cancel: true;
                        radius: 3;
                        onItemClicked: {
                            if( item_name === "__ADD_ITEM__" ) {
                                Settings.name_change_cb = create_newitem;
                                item_name_screen.text = "";
                                item_name_screen.show();
                                return;
                            }
                            if( item_name === "__BACK_ITEM__" ) {
                                item_edit_slide.hide();
                                Settings.select( Settings.api.item_parentid );
                                return;
                            }

                            if( item_id !== Settings.api.item_id ) {
                                if( Settings.menuitem_modifiable(item_id) )
                                    item_edit_slide.show();
                            }

                            if( Settings.menuitem_modifiable(item_id) ) {
                                item_edit_slide.show();
                                Settings.select( item_id );
                            }
                        }

                        Component.onCompleted : {
                            if( !Settings.menuitem_modifiable(item_id) )
                                color = "gray";
                        }
                    }
                }

                model: Settings.api.menuModel;
                delegate: category;
                anchors.fill: parent;
                clip: true;

                /*add : Transition {
                    NumberAnimation { property: "y"; from: 0; duration: 200; easing.type: Easing.InOutCirc; }
                }*/
            }
        }

        Text {
            id: selection_name;
            anchors.top: parent.top; anchors.topMargin: parent.height * 0.03
            anchors.horizontalCenter: parent.horizontalCenter;
            text: Settings.api.item_name;
            width: parent.width * 0.95;
            height: parent.height * 0.1;
            color: "white";
            font.pixelSize: ((Settings.api.item_name.length > 50 ) ? height * 0.2 : height * 0.4);
            font.family: "Molot";
            horizontalAlignment: Text.AlignHCenter;
            wrapMode: Text.Wrap;
        }
    }
    Slide {
        id: item_edit_slide;
        //anchors.left: parent.left; anchors.leftMargin: parent.width * 0.01;
        anchors.right: category_list_slide.left; anchors.rightMargin: parent.width * 0.01;
        y: -height;
        from: -height;
        to: 0;
        width: App.screen_w * 0.3;
        height: category_list_slide.height;
        color: "#aaffffff";
        duration: 400;
        reshow: true;

        onShown : {
        }

        Button {
            id: item_name_b;
            anchors.horizontalCenter: parent.horizontalCenter;
            anchors.top: parent.top; anchors.topMargin: parent.height * 0.15;

            radius: 3;
            color: Colors.grey5;
            width: parent.width * 0.95;
            height: parent.height * 0.2;

            onButtonClick : {
                Settings.name_change_cb = change_item_name;
                item_name_screen.text = Settings.api.item_name;
                item_name_screen.show();
            }
            Text {
                id: nametext
                anchors.bottom: parent.bottom; anchors.bottomMargin: parent.height * 0.04
                anchors.left: parent.left; anchors.leftMargin: parent.width * 0.25;
                anchors.right: parent.right; anchors.rightMargin: parent.width * 0.04;
                color: "white";//Colors.tran_toolbar;

                text: Settings.api.item_name;
                font.family: "Chunkfive";
                font.pixelSize: (Settings.api.item_name.length > 50) ? parent.height * 0.15 : parent.height * 0.18;
                font.capitalization: Font.AllUppercase;
                wrapMode: Text.WordWrap;
                horizontalAlignment: Text.AlignRight;
                elide: Text.ElideRight;
                maximumLineCount: 5;
            }

            Image {
                anchors.left: parent.left; anchors.leftMargin: parent.width * 0.01;
                anchors.verticalCenter: parent.verticalCenter;

                sourceSize.width: parent.height * 0.6;
                sourceSize.height: parent.height * 0.6;

                source: "reports_ico3.svg";
            }
        }
        Button {
            id: item_price_b;
            anchors.horizontalCenter: parent.horizontalCenter;
            anchors.top: item_name_b.bottom; anchors.topMargin: parent.height * 0.01;

            radius: 3;
            color: Colors.blue6;
            width: parent.width * 0.95;
            height: parent.height * 0.2;

            onButtonClick : {
                Settings.name_change_cb = change_item_name;
                item_price_screen.show();
            }

            Text {
                text: Settings.api.item_price.toFixed(2);
                font.family: "Molot";
                font.pixelSize: parent.height * 0.3;
                color: "white";
                anchors.right: parent.right; anchors.rightMargin: parent.width * 0.1;
                anchors.verticalCenter: parent.verticalCenter;
            }

            Image {
                anchors.left: parent.left; anchors.leftMargin: parent.width * 0.01;
                anchors.verticalCenter: parent.verticalCenter;

                sourceSize.width: parent.height * 0.6;
                sourceSize.height: parent.height * 0.6;

                source: "tag_ico.svg";
            }
        }
        Button {
            id: item_delete_b;
            anchors.horizontalCenter: parent.horizontalCenter;
            anchors.top: item_price_b.bottom; anchors.topMargin: parent.height * 0.01;

            radius: 3;
            color: Colors.red2;
            pixelSize: parent.height * 0.03;
            width: parent.width * 0.95;
            height: parent.height * 0.2;

            onButtonClick: App.choice( "Delete " + Settings.api.item_name + "?", onItemDelete );

            Text {
                text: "Delete"
                font.family: "Molot";
                font.pixelSize: parent.height * 0.32;
                color: "white";
                anchors.right: parent.right; anchors.rightMargin: parent.width * 0.1;
                anchors.verticalCenter: parent.verticalCenter;
            }

            Image {
                anchors.left: parent.left; anchors.leftMargin: parent.width * 0.01;
                anchors.verticalCenter: parent.verticalCenter;
                sourceSize.width: parent.height * 0.6;
                sourceSize.height: parent.height * 0.6;
                source: "x_ico.svg";
            }
        }

        Image {
            id: edit_ico;
            anchors.top: parent.top; //anchors.topMargin: parent.height * 0.12;
            anchors.horizontalCenter: parent.horizontalCenter;
            source: "edit_ico.svg";
            sourceSize.width: parent.width * 0.15;
            sourceSize.height: parent.width * 0.15;
        }
    }

    Slide {
        id: whitebar;
        anchors.top: parent.top; anchors.topMargin: parent.height * 0.11;
        width: parent.width;
        height: 4;
        color: "white";
        property: "x";
        x: -width;
        from: -width;
        to: 0;
        duration: 400;
    }

    Slide {
        id: whitebar_bottom;
        anchors.bottom: parent.bottom; anchors.bottomMargin: parent.height * 0.14;
        width: parent.width;
        height: 4;
        color: "white";
        x: width;
        to: 0;
        duration: 400;
    }

    FadeView {
        id: item_price_screen;
        dim: true;
        color: Colors.make( Colors.violet3, "aa" );
        dimColor: "#cc000000";

        onHalfShown : {
            price_input.clear();
            price_input.show(Menu.api.item_price);
            price_slide.show();
        }

        onClicked : {
            price_input.hide();
            price_slide.hide();
            hide();
        }
        Slide {
            id: price_slide;
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
                    id: price_cancel_btn;
                    pointSize: 20;
                    color: Colors.red2;
                    label: "Cancel";
                    icon: "x_ico.svg";
                    height: price_slide.height * 0.9;
                    width: height;
                    radius: 10;
                    onButtonClick : {
                        price_slide.hide( 100 );
                        price_input.hide();
                        item_price_screen.hide( 1000 );
                    }

                }
                Button {
                    id: price_done_btn;
                    pointSize: 20;
                    color: Colors.green3;
                    icon: "check_white_ico.svg";
                    label: "Finish";
                    width: price_slide.height * 0.9;
                    height: width;
                    radius: 10;
                    onButtonClick : {
                        Settings.set_itemprice( price_input.total );

                        price_slide.hide( 100 );
                        price_input.hide();
                        item_price_screen.hide(1000);
                        item_edit_slide.show();
                    }
                }
             }
         }
        CashInput {
            id: price_input;
            spacing: 10;
        }
    }

    TextInputView {
        id: item_name_screen;
        max: 128;

        onAccept : {
            hide();
            Settings.name_change_cb( text );
        }

        onReject : {
            hide();
        }
    }
}
