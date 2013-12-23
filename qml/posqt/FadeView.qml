import QtQuick 1.1;

Rectangle {
    id: root;

    anchors.fill: parent;
    visible: false;
    opacity: 0.0;
    property alias dim : dimmer.visible;
    property alias dimColor : dimmer.color;
    property alias running : fade_ani.running;
    property int duration : 300;
    property bool halfshown : false;
    default property alias children : content.children;

    signal clicked();
    signal shown();
    signal hidden();
    signal halfShown();
    signal halfHidden();
    signal hiding();

    function show( new_duration ) {
        if( fade_ani.running ) return;

        fade_ani.duration = (new_duration) ? new_duration : fade_ani.duration;
        fade_ani.to = 1;
        fade_ani.callback = shown;

        event_timer.interval = fade_ani.duration / 2.07;
        event_timer.callback = halfShown;

        fade_ani.start();
    }

    function hide( new_duration ) {
        if( fade_ani.running ) return;


        fade_ani.duration = (new_duration) ? new_duration : fade_ani.duration;
        fade_ani.to = 0;
        fade_ani.callback = hidden;
        fade_ani.start();

        event_timer.interval = fade_ani.duration / 2.07;
        event_timer.callback = halfHidden;

        hiding();
    }

    Timer {
        id: event_timer;
        interval: duration;
        running: false;
        repeat: false;
        property variant callback : undefined;

        onRunningChanged : {
            if( !running && callback ) {
                callback();
            }
        }
    }

    PropertyAnimation {
        id: fade_ani;

        target: root;
        duration: root.duration;
        easing.type: Easing.InOutCirc;
        property: "opacity";
        to: 0;

        property variant callback : undefined;

        onStarted : {
            event_timer.start();
            if( to ) root.visible = true;
        }

        onRunningChanged : {
            if( running ) return;
            if( !to ) root.visible = false;

            if( callback ) {
                duration = root.duration;
                return callback();
            }

        }
    }

    MouseArea {
        anchors.fill: parent;
        onClicked: {
            if( !fade_ani.running ) {
                parent.clicked();
            }
        }
    }

    Rectangle {
        id: dimmer;
        color: "#99000000";
        anchors.fill: parent;
        visible: false;
    }

    Item { id: content; anchors.fill: parent; }
}
