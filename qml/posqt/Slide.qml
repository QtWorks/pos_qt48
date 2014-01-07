import QtQuick 1.1;

Rectangle {
    id: root;
    width: parent.width;
    height: parent.height;
    color: "#00000000";
    property int  to : 0;
    property int  from : 0;
    property int duration : 200;
    property bool showing : false; //!< In the process of being shown
    property bool reshow  : false; //!< true = slide will do a hide then a show IF ALREADY showing
    property bool catch_input : false;
    property bool fade: false;
    property alias property : animation.property;
    property alias easing : animation.easing.type;
    property alias running : animation.running;
    default property alias children : content.children;

    signal shown();
    signal hid();
    signal halfHid();
    signal hideMe();
    signal showMe();
    signal clicked();

    PropertyAnimation {
        id: fadeani;

        target: root;
        duration: root.duration * 2.0;
        easing.type: Easing.InOutCirc;
        property: "opacity";
        to: 0.0;
        from: 1.0;
    }

    PropertyAnimation {
        id: animation;

        target: root;
        duration: root.duration;
        easing.type: Easing.InOutCirc;
        property: "y";
        to: 0;

        property variant callback : undefined;

        onRunningChanged : {
            if(running) {
                return;
            }
            if( callback )
                callback();
            duration = root.duration;
            callback = undefined;
        }
    }

    MouseArea  {
        anchors.fill: parent;
        preventStealing: true;
        //propagateComposedEvents: true;
        onClicked: {
            if( catch_input )
                parent.clicked();
            else
                mouse.accepted = false;
        }
    }

    Item { id: content; anchors.fill: parent; }

    function toggle()   { if( showing ) hide(); else show(); }

    /// Show the slide.
    /// @param new_duration Override the default duration of the animation.
    ///                     Setting this to zero causes an instant change, w/o animation.
    ///                     Calling w/o arguments causes default behavior.
    function show( new_duration ) {
        showMe();

        if( showing && reshow ) {
            if( animation.property == "y" )
                root.y = from;
            else
                root.x = from;
        }

        if( new_duration === 0 || new_duration ) {
            animation.duration = new_duration;
        }

        if( fade ) { fadeani.to = 1.0; fadeani.from = 0.0; fadeani.start() }
        showing = true;
        animation.to = to;
        animation.callback = root.shown;
        animation.start();

    }

    /// Hide the slide.
    /// @param new_duration Override the default duration of the animation.
    ///                     Setting this to zero causes an instant change, w/o animation.
    ///                     Calling w/o arguments causes default behavior.
    function hide( new_duration ) {
        hideMe();

        if( new_duration === 0 || new_duration ) {
            animation.duration = new_duration;
        }

        if( fade ) { fadeani.to = 0.0; fadeani.from = 1.0; fadeani.start() }
        showing = false;
        animation.callback = root.hid;
        animation.to = from;
        animation.start();

    }
}
