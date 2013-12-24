import QtQuick 1.1
import "colors.js" as Colors;
import "app.js" as App;

//Rectangle {
Item {
    id: root;
    anchors.fill: parent;
    //label: "Login";
    //color: Colors.blue_two;//"#299DD6";
    property bool buttonHighlight: true;
    property bool buttonBackdrop: false;
    property int buttonPointSize: 35;
    property color bColor : Colors.make( Colors.blue3, "cc" );//"#299DD6";
    property int buttonWidth : width * 0.1;//150;
    property int buttonHeight: width * 0.1;//150;
    property int buttonRadius: 5;
    property int buttonID: -1;
    property bool clickState: false;
    property string mask : "";
    property real maskOpacity : 0.0;
    property color bgColor: "#00000000";
    property string pass : "";

    PropertyAnimation {
        id: failani;

        target: root;
        duration: 500;
        easing.type: Easing.InOutCirc;
        property: "bColor";
        to: Colors.red2;

        onRunningChanged : {
        //onStopped : {
            if( !running )
                bColor = Colors.make( Colors.blue3, "aa" );
        }
    }

    function fail( duration ) {
        if( duration )
            failani.duration = duration;
        failani.start();
    }

    function show() {
        pass = "";
        leftColumn.show();
        centerColumn.show();
        rightColumn.show();
    }

    function hide( interval ) {
        pass = "";
        leftColumn.hide( interval );
        centerColumn.hide( interval );
        rightColumn.hide( interval );
    }

    signal xClick();
    signal okClick( variant _password );

    Slide {
        id: leftColumn;
        x: App.screen_w/2 - width*1.5;
        y: App.screen_h;//parent.height/2 - height/2;
        to: 0;
        from: App.screen_h;
        width: buttonWidth + 5;
        duration: 400;
        color: bgColor;
        Behavior on color { PropertyAnimation { duration: 400; easing.type: Easing.InOutCirc;}}

        Column {
            spacing: 5;
            anchors.centerIn: parent;
            Button {
                radius: buttonRadius;
                label: "7";
                color: bColor;
                width: buttonWidth;
                height: buttonHeight;
                pixelSize: height * 0.5;
                font: "Molot";
                onButtonClick : {
                    root.clickState = !root.clickState;
                   root.buttonID = 7;
                    pass += label;
                }

            }
            Button {
                radius: buttonRadius;
                useBackdrop: buttonBackdrop;
                color: bColor;
                label: "4";
                width: buttonWidth;
                pixelSize: height * 0.5;
                height: buttonHeight;
                font: "Molot";
                onButtonClick : {
                   root.buttonID = 4;
                    root.clickState = !root.clickState;
                    pass += label;
                }
            }
            Button {
                radius: buttonRadius;
                useBackdrop: buttonBackdrop;
                color: bColor;
                width: buttonWidth;
                pixelSize: height * 0.5;
                font: "Molot";
                height: buttonHeight;
                 label: "1";
                 onButtonClick : {
                   root.buttonID = 1;
                    root.clickState = !root.clickState;
                    pass += label;
                }
            }
            Button {
                radius: buttonRadius;
                useBackdrop: true;
                useHighlight: false;
                color: bColor;
                pointSize: buttonPointSize;
                width: buttonWidth;
                height: buttonHeight;
                label : "";
                icon: "red_exit_ico.svg";
                onButtonClick : {
                    root.buttonID = 11;
                    root.clickState = !root.clickState;
                    root.xClick();
                }
            }
        }
    }

    Slide {
        id: centerColumn;
        y: -height;//( buttonHeight * 3 + 3 * 10);
        to: 0;
        from: -height;
        //to: 0;
        duration: 300;
        x: App.screen_w/2 - width/2;
        width: buttonWidth + 5;
        color: bgColor;
        Behavior on color { PropertyAnimation { duration: 400; easing.type: Easing.InOutCirc;}}

        Column {
            spacing: 5;
            anchors.centerIn: parent;//verticalCenter: parent.verticalCenter;

            Button {
                radius: buttonRadius;
                color: bColor;
                label: "8";
                width: buttonWidth;
                pixelSize: height * 0.5;
                font: "Molot";
                height: buttonHeight;
                onButtonClick : {
                   root.buttonID = 8;
                    root.clickState = !root.clickState;
                    pass += label;
                }
            }
            Button {
                radius: buttonRadius;
                color: bColor;
                width: buttonWidth;
                height: buttonHeight;
                pixelSize: height * 0.5;
                font: "Molot";
                 label: "5";
                 onButtonClick : {
                   root.buttonID = 5;
                    root.clickState = !root.clickState;
                    pass += label;
                }
            }

            Button {
                radius: buttonRadius;
                color: bColor;
                width: buttonWidth;
                height: buttonHeight;
                pixelSize: height * 0.5;
                font: "Molot";
                 label: "2";
                 onButtonClick : {
                   root.buttonID = 2;
                    root.clickState = !root.clickState;
                    pass += label;
                }
            }
            Button {
                radius: buttonRadius;
                color: bColor;
                width: buttonWidth;
                height: buttonHeight;
                label: "0";
                pixelSize: height * 0.5;
                font: "Molot";
                onButtonClick : {
                   root.buttonID = 0;
                    root.clickState = !root.clickState;
                    pass += label;
                }
            }
        }
    }
    Slide {
        id: rightColumn;
        to: 0;
        from: parent.height;
        x: parent.width/2 + width/2;
        y: parent.height;
        width: buttonWidth + 5;
        color: bgColor;
        Behavior on color { PropertyAnimation { duration: 400; easing.type: Easing.InOutCirc; }}

        Column {
            spacing: 5;
            anchors.centerIn: parent;
            Button {
                radius: buttonRadius;
                color: bColor;
                label: "9";
                width: buttonWidth;
                height: buttonHeight;
                pixelSize: height * 0.5;
                font: "Molot";
                onButtonClick : {
                   root.buttonID = 9;
                    root.clickState = !root.clickState;
                    pass += label;
                }
             }

            Button {
                radius: buttonRadius;
                color: bColor;
                width: buttonWidth;
                height: buttonHeight;
                label: "6";
                pixelSize: height * 0.5;
                font: "Molot";
                onButtonClick : {
                   root.buttonID = 6;
                    root.clickState = !root.clickState;
                    pass += label;
                }
            }

            Button {
                radius: buttonRadius;
                color: bColor;
                width: buttonWidth;
                height: buttonHeight;
                label: "3";
                pixelSize: height * 0.5;
                font: "Molot";
                onButtonClick : {
                   root.buttonID = 3;
                    root.clickState = !root.clickState;
                    pass += label;
                }
            }
            Button {
                radius: buttonRadius;
                useHighlight: false;
                color: bColor;
                width: buttonWidth;
                height: buttonHeight;
                icon : "check_green_ico.svg"
                label: "";
                pixelSize: height * 0.5;
                font: "Molot";
                onButtonClick : {
                   root.buttonID = 10;
                    root.clickState = !root.clickState;
                    root.okClick( pass );
                    pass = "";
                }

            }
        }
   }
   Rectangle {
        width: parent.width + 10;
        height: 5;
        color: "#299DD6";
        anchors.bottomMargin: 50;
        anchors.leftMargin: -5;
        anchors.bottom: parent.bottom;
        anchors.left: parent.left;
        opacity: maskOpacity;
        Behavior on opacity { PropertyAnimation { duration: 200; }}
        Text {
            //anchors.centerIn: parent;
            anchors.horizontalCenter: parent.horizontalCenter;
            anchors.bottom: parent.top;
            color: Colors.blue3;//"white";
            font.pointSize: 32;
            font.family: "Chunkfive";
            font.weight: Font.Light;
            font.letterSpacing: 6;
            text: mask;
        }
    }

}

