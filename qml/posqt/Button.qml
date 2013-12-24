import QtQuick 1.1

Item {
    id: button;
    width: 100;
    height: 100;

    property color color: "#00000000";
    property alias radius: backdrop.radius;
    property color onHoverColor: "gray";
    property color borderColor: "white";//backdrop.border.color;
    property int borderWidth: 0;//backdrop.border.width;
    property real   textWordSpacing: 0.0;
    property bool useBackdrop : true;
    property bool useHighlight: true;
    property bool toggled : false;
    property bool animated: false;
    property variant mouse : buttonMouseArea;
    default property alias children : content.children;
    property alias icon: icon.source;
    property alias labelWidth: buttonLabel.width;
    property alias font : buttonLabel.font.family;
    property alias pixelSize : buttonLabel.font.pixelSize;
    property alias pointSize: buttonLabel.font.pointSize;
    property alias textColor: buttonLabel.color;
    property alias textWeight: buttonLabel.font.weight;
    property alias label : buttonLabel.text;
    property alias halign: buttonLabel.horizontalAlignment;
    property alias textanchors : buttonLabel.anchors;
    property alias act_on_cancel : buttonMouseArea.cancel;
    property alias icon_width : icon.width;
    property alias icon_height : icon.height;
    property alias preventStealing: buttonMouseArea.preventStealing;
    property bool  sensitive: false;

    //Visual modifiers
    property bool orientIconLeft : false;
    property bool labelOverIcon : false;
    property bool orientIconRight : false;

    signal cancelClick();
    signal buttonClick();
    signal longClick();
    signal doubleClick();

    //Behaviors
    Behavior on width { PropertyAnimation { duration: 200 * animated; easing.type: Easing.InOutCirc; }}
    Behavior on height { PropertyAnimation { duration: 200 * animated; easing.type: Easing.InOutCirc; }}
    PropertyAnimation {
        id: colorani;

        duration: 150;
        property: "color";
        target: backdrop;
/*        onRunningChanged: {
        //onStopped : {
            if(!running)
                backdrop.color = Qt.binding( function() { return button.color; } );
        }*/
    }

    onOrientIconLeftChanged : {
        if( orientIconLeft ) {
            icon.anchors.centerIn = undefined;
            icon.anchors.left = button.left;
            icon.anchors.verticalCenter = button.verticalCenter;
            icon.anchors.leftMargin = icon.sourceSize.width/4;

            buttonLabel.anchors.centerIn = button;
        }
    }

    onOrientIconRightChanged : {
        if( orientIconRight ) {
            icon.anchors.centerIn = undefined;
            icon.anchors.right = button.right;
            icon.anchors.verticalCenter = button.verticalCenter;
            icon.anchors.rightMargin = icon.sourceSize.width/4;

            buttonLabel.anchors.centerIn = button;
        }
    }

    onButtonClick : {
        /*button.color = Qt.darker(color, 3.0);
        if(useBackdrop)
            backdrop.color = Qt.darker(button.color, 1.5);
        else
            backdrop.color = button.color;

        buttonLabel.color = Qt.lighter(textColor, 4.0);*/
    }

    MouseArea {
        id: buttonMouseArea;
        anchors.fill: parent;
        onCanceled: { if( cancel ) cancelClick(); }
        onClicked: buttonClick();
        onDoubleClicked : doubleClick();
        onPressAndHold : longClick();

        property bool cancel : false;

        onEntered: if(sensitive) buttonClick();/*{
            if( !colorani.running ) {
                colorani.duration = 200;    MouseArea {
        id: ticketMouseArea;
        visible: (split_mode || virtual);
        anchors.fill: parent;
        preventStealing: true;
        //propagateComposedEvents: true;
        onPressed : {
            if( TicketLib.split_mode ) {
                if( hasItem() )
                    split_select_destination( ticketBox, sale_id );
                else
                    split_ticket_select( ticketBox );
            } else {
                mouse.accepted = false;
            }
        }
    }
                colorani.to = Qt.darker( backdrop.color, 1.25 );
                colorani.start();
            }
        } */

        onExited:{
            //backdrop.color = Qt.lighter(backdrop.color, 2);//color;
            //buttonLabel.color = Qt.lighter(buttonLabel.color, 1.0);
        }
        onReleased: {
            //parent.color = color;
            //buttonLabel.color = textColor;
        }
    }

    Rectangle {
        id: backdrop;
        anchors.fill: parent;
        color: button.color;
    }


    Image {
        id: icon;
        anchors.centerIn: parent;
        source: "";//button.icon;
        sourceSize.width: 48 * (parent.width/100);
        sourceSize.height: 48 * (parent.height/100);
        smooth: true;

        Component.onCompleted : {
            if( parent.orientIconLeft ) {
                anchors.centerIn = undefined;
                anchors.left = parent.left;
                anchors.verticalCenter = parent.verticalCenter;
            }
        }
    }





    Text {
        id: buttonLabel;
        font.family: "Helvetica";
        width: parent.width / 2;
        horizontalAlignment: Text.AlignHCenter;
        anchors.bottom: parent.bottom;
        anchors.bottomMargin: parent.height/2 - icon.height*0.8 - buttonLabel.height/2;
        anchors.horizontalCenter: parent.horizontalCenter;
        text: label;
        color: "white";
        wrapMode: Text.WordWrap;
        Component.onCompleted : {
            if( parent.labelOverIcon ) {
                anchors.centerIn = icon;
            }
        }
    }


    Item {
        id: content;
        anchors.fill: parent;
    }
}
