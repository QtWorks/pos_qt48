import QtQuick 1.1

Item {
    id: warning_box;
    anchors.right: parent.right;
    anchors.top: parent.top;
    anchors.topMargin: 2;
    //anchors.topMargin: -220;
    width: 200;
    height: 64

    property string current_icon : "";
    property variant lastItem: undefined;

    Rectangle{
        id: backdrop;
        width: 64;
        height: 300;
        y: -117;
        x: 20;
        rotation: -90;
        gradient: Gradient {
            GradientStop { position: 0.0; color: "#002c2a29";}
            GradientStop { position: 1.0; color: "#ff2c2a29";}
        }
        MouseArea{
            anchors.fill: parent;
            onPressed: {
                yellow_warning.x = 0;
            }
        }
    }

    Column {
        id: item_area;
        height: 800;
        width: 200;
        anchors.top : parent.bottom;
        anchors.right: parent.right;
        spacing: 2;
        //color: "#00000000";
        move: Transition{PropertyAnimation{properties:"y,x"; duration:200;}}
    }
    function fillWarnings(item, icon)
    {
        /*Notes:
          ListSlider element kills itself upon reaching the x of 201(offscreen)
        */

        item.active = !item.active;
        if(item.active)
            item.start();
        else
            item.stop();

        for(var itm in item_area.children){//Remove any previous children
            item_area.children[itm].destroy();// = 201;
        }

        if(item == lastItem){
            lastItem = undefined;
            return;
        }

        if(lastItem){
            lastItem.active = false;
            lastItem.stop();
        }

        var component = Qt.createComponent("ListSlider.qml");
        var slider;
        var i = 0;
        for(i = 0; i < item.count; i ++){
            slider = component.createObject( item_area,
                                            {
                                                "x": item_area.width,
                                                "source_warning" : item,
                  //                              "y": (i * 64) + (i * 2) ,
                                                "slide_duration" :100 + ((i * 30)),
                                                "icon": icon });
            if(slider)
                slider.x = 0;
        }

        lastItem = item;
    }
    WarningLabel {
        id: yellow_warning;
        source: "yellow_warning_ico.svg";
        x: parent.width;
        count: 10;
        onXChanged : {
            if( x == 0 ){
                green_warning.x = 66;
            }
        }
        onPressedChanged : {
            fillWarnings(yellow_warning, "yellow_exp_ico.svg");
        }
    }
    WarningLabel {
        id: green_warning;
        source: "green_warning_ico.svg";
        x: parent.width;
        count: 20;
        onXChanged : {
            if(x == 66){
                red_warning.x = 132;
            }
        }
        onPressedChanged : {
            fillWarnings(green_warning, "green_exp_ico.svg");
        }
    }

    WarningLabel {
        id: red_warning;
        source: "red_warning_ico.svg";
        x: parent.width;
        onPressedChanged : {
             fillWarnings(red_warning, "red_exp_ico.svg");
        }
        count: 5;
    }
}
