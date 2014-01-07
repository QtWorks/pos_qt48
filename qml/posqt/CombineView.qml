import QtQuick 1.1
import "app.js" as App;
import "colors.js" as Colors;
import "ticket.js" as TicketLib;
import "user.js" as User;

FadeView {
    id: root;
    dim: true;
    dimColor: "#bb000000";
    color: Colors.make( Colors.violet2, "99" );
    property int _dest : -1;
    property int _dest_user : -1;
    height: App.screen_h;
    width: App.screen_w;

    function unload() {
        root.hide();
    }

    function combine_check( choice ) {
        if( choice ) {
            combine( _dest );
            App.hide_choice();
        } else {
            root._dest = -1;
        }
    }

    /// @brief  Combine tickets together
    function combine( dest ) {
        if( root._dest === -1 ) {
            root._dest = dest;
            App.choice( "Continue? This action will transfer items to sale #" +
                        dest + ". And completely remove sale #" +
                        TicketLib.API.saleId + " .", combine_check);
            return;
        }

        TicketLib.API.combine( dest, TicketLib.API.saleId );

        //After combine, select the destenation ticket & refresh all data
        App.api.refresh();
        User.api.refresh();
        if( root._dest_user === User.id ) {
            TicketLib.activate( dest );
        }
        else {
            TicketLib.activate( -1 );
        }

        TicketLib.refresh();

        //Get out of this view
        unload();
    }

    onHidden : {
        App.load_subview( "" );
        App.setBackCB( null );
    }

    Component.onCompleted : {
        App.setBackCB( root.unload );
        show();
    }

    onClicked: {
        hide();
    }

    Item {
        id: pagelabel;
        width: root.width * 0.4;
        height: root.height * 0.2;
        anchors.left: parent.left; anchors.leftMargin: root.width * 0.0025;
        anchors.top: parent.top; anchors.topMargin: root.height * 0.12;

        Text {
            anchors.fill: parent;
            text: "Choose a Destination Sale";
            font.family: "Chunkfive";
            font.pixelSize: root.height * 0.08;
            color: "white";
        }
    }

    Rectangle {
        width: root.width;
        height: width * 0.2;
        color: "white";
        anchors.left: parent.left;
        anchors.verticalCenter: parent.verticalCenter;

        ListView {
            id: salelist;
            anchors.fill: parent;
            anchors.leftMargin: root.width * 0.005; anchors.rightMargin: root.width * 0.005;
            clip: true;
            spacing: width * 0.02;
            delegate: saleitem;
            model: TicketLib.API.allSaleModel;
            orientation: ListView.Horizontal;

            Component {
                id: saleitem;
                TableItem {
                    width: height;
                    height: salelist.height;
                    color: Colors.index( table_id + 1 );
                    sensitive: false;

                    onButtonClick : {
                        root._dest_user = user_id;
                        if( user_id === User.id ) {
                            combine( sale_id );
                        }
                        else {
                            admin_check.saleid = sale_id;
                            admin_check.show();
                        }
                    }

                    Component.onCompleted : {
                        if( user_id !== User.id )
                            color = Colors.grey2;
                    }
                }
            }
        }
    }

   FadeView {
        id: admin_check;
        duration: 300;
        color: Colors.make( Colors.violet2, "22" );
        dim: true;
        dimColor: Colors.make( "#000000", "bb" );
        property int saleid : -1;

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
                    combine( admin_check.saleid );
                }
                else {
                    basic_pad.fail();
                }
            }

            onXClick : {
                admin_check.hide();
            }
        }
    }

}
