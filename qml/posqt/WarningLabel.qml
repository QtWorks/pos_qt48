import QtQuick 1.1

Image {
    sourceSize.width: 64;
    sourceSize.height: 64;
    property int count: 0;
    property color textColor: "white";
    property bool pressed: false;
    property bool active: false;

    Behavior on x {PropertyAnimation{duration:300}}
    Behavior on scale {PropertyAnimation{duration:200;}}

    Text {
        color: textColor;
        text: count;
        font.pointSize: 10;
        font.family: "Helvetica";
        anchors.right: parent.right;
        anchors.bottom: parent.bottom;
    }

    MouseArea {
        anchors.fill: parent;
        onPressed : {
            parent.pressed = !parent.pressed;
        }
    }

    Timer {
        id: _myTimer;
        triggeredOnStart: true;
        interval: 200;
        running: false;
        repeat: true;
        onTriggered: {
            parent.scale = (parent.scale == 1.0) ? 0.8 : 1.0;
        }
    }

    function start() {
        _myTimer.start();
    }
    function stop() {
        parent.scale = 1.0;
        _myTimer.stop();
    }
}
