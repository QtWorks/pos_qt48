import QtQuick 1.1;

Image {
    id: list_slider;
    sourceSize.height: 64;
    source: "list_slider.svg";
    property int slide_duration : 0;
    property string icon : "yellow_exp_ico.svg";
    property variant source_warning : undefined;

    Behavior on x {
        PropertyAnimation {
            duration:slide_duration;
            onRunningChanged : {
                if(list_slider.x == 201){
                    if(source_warning)
                        source_warning.count--;
                    list_slider.destroy();
                }
            }
        }
    }

    Image {
        source: icon;
        sourceSize.width: 32;
        sourceSize.height: 32;
        anchors.left:parent.left;
        anchors.leftMargin: 10;
        anchors.verticalCenter: parent.verticalCenter;
    }
    MouseArea {
        anchors.fill: parent;
        onPressed : {
            slide_duration = 100;
            parent.x = 201;
        }
    }
}
