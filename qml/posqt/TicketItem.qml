import QtQuick 1.1
import "colors.js" as Colors;
import "ticket.js" as TicketLib;

/* Use this Item inside a ticket listview component to represent an order */

Item {
    id: orderroot;
    anchors.right: parent.right;
    anchors.rightMargin: ticketBG.width * 0.025;
    width: ticketBG.width * 0.95;
    height: ticketView.height * 0.2;//15;

    signal click(variant _iid,variant _cid);
    signal subClick( variant _iid, variant _refill);
    signal itemLongClick(variant _iid, variant _cid);
    signal finalClick(variant _iid, variant _cid);

    Item {
        id: order;
        height: orderroot.height;
        width: orderroot.width;
        clip: true;
            Button {
                id: ticketItemButton;
                anchors.right: parent.right;
                color: "#299dd6";
                width: order.width;
                height: order.height;
                label: "";
                clip: true;
                hoverEnabled: true;

                Text {
                    id: order_index;
                    color: "white";
                    width: parent.width * 0.05;
                    anchors.verticalCenter: parent.verticalCenter;
                    anchors.left: parent.left;
                    anchors.leftMargin: width * 0.5;
                    font.family: "Chunkfive";
                    font.pixelSize: parent.height * 0.4;
                    text: item_displayid;
                    visible: (!item_is_sub);
                }


                Text { //Visual
                    id: order_name;
                    anchors.top: parent.top;
                    anchors.bottom: parent.bottom;
                    anchors.left: order_index.right;
                    anchors.right: float_text.left;
                    color: "white";
                    font.family: "Chunkfive";
                    font.pixelSize: parent.height * 0.2;
                    //font.pointSize: 18;
                    text: item_name;
                    elide: Text.ElideRight;
                    horizontalAlignment: Text.AlignHCenter;
                    verticalAlignment: Text.AlignVCenter;
                    wrapMode: Text.Wrap;
                }

                Text {
                    id: total_text;
                    anchors.top: parent.top; anchors.topMargin: parent.height * 0.02;
                    anchors.right: parent.right; anchors.rightMargin: parent.width * 0.02;
                    text: (item_total > 0.009) ? (item_total - item_total % 1) : "";
                    font.family: "Molot";
                    font.pixelSize: parent.height * 0.4;
                    color: "white";
                }

                Text {
                    id: float_text;
                    visible: (item_total);
                    text: String((item_total % 1).toFixed(2)).slice(1);
                    anchors.right: total_text.right;
                    anchors.top: total_text.bottom; anchors.topMargin: parent.height * 0.1;
                    color: "white";
                    font.family: "Molot";
                    font.pixelSize: parent.height * 0.3;
                }

                Rectangle {
                    anchors.fill: parent;
                    color: Colors.make( Colors.red2, "cc" );
                    visible: item_void;
                    MouseArea { //Steal any clicks coming into the item when its marked for void
                        anchors.fill: parent;
                    }

                    Text {
                        anchors.fill: parent;
                        horizontalAlignment: Text.AlignHCenter;
                        verticalAlignment: Text.AlignVCenter;
                        text: " VOID ";
                        color: "white";
                        font.family: "Chunkfive";
                        font.pixelSize: parent.height * 0.5;
                    }
                }

                onButtonClick: {
                    ticketView.positionViewAtIndex( item_id - 1, ListView.Beginning );
                    selected_id = item_id;
                    item = orderListModel.get( item_id );
                    TicketLib.select( selected_id );

                    if( split_mode ) { //During split mode all normal item actions are dissabled
                        splitSelect( item_id );
                        return;
                    }

                    if( !ticketBox.editable ) return;

                    if( item_is_sub ) { //check if its a sub
                        orderroot.subClick( item_menuid, item_refill );
                        return;
                    }

                    if( !item_isfinal)
                        orderroot.click( item_menuid, item_parentid );
                    else
                        orderroot.finalClick( item_menuid, item_parentid );
                }

                onLongClick : {
                    ticketView.positionViewAtIndex( item_id - 1, ListView.Beginning );
                    selected_id = item_id;
                    item = orderListModel.get( item_id );
                    TicketLib.select( selected_id );

                    if( !split_mode ) {
                        orderroot.itemLongClick( item_menuid, item_parentid );
                    }
                }

                Component.onCompleted: {
                    if(item_is_sub && !item_comment) {
                        width = width * 0.65;
                        order_index.visible = false;//font.pointSize = 9;
                        order_name.font.pointSize = height * 0.2;
                        //total_text.text = item_subid;
                        if( item_total !== 0 ) {
                            color = "#8bc53f";
                        }
                    }
                    else if( item_comment ) {
                        width = width * 0.8;
                        color = Colors.yellow3;
                        order_name.font.pointSize = 13;
                        order_index.visible = false;
                        total_text.visible = false;
                    }
                    if( !ticketBox.editable ) {
                        color = Colors.make( Colors.blue3, "cc" );//transparent_white;//"gray";
                        return;
                    }
                    if( item_isfinal ) {
                        color = Colors.violet2;//Colors.make( Colors.orange, "99" );//tran_white;
                    }
                }
            }
    }

    Image {
        id: highlight;
        anchors.verticalCenter: parent.verticalCenter;
        anchors.left: parent.right; anchors.leftMargin: -10;
        source: "ticket_indicator.svg";
        sourceSize.width: 32;
        sourceSize.height: 32;

        visible: (selected_id == item_id);
    }
}
