import QtQuick 1.1

Row {
    spacing: 0;
    Rectangle{
        id: timebox;
         height: 100;
         width: 0;//230;
         visible: false;
         //color: "#a0bfc0c3";
         color: "#a02c2a29";
         Behavior on width { PropertyAnimation {duration: 200;}}
         onWidthChanged: {
             if(width == 0){
                 visible = false;
             }
         }
         Column {
             anchors.top: parent.top;
             anchors.topMargin: 20;
             anchors.horizontalCenter: parent.horizontalCenter;
             spacing: 5;
             Text {
                 id: timeText;
                 anchors.horizontalCenter: parent.horizontalCenter;
                 color : "white";
                 text: "12 : 00 PM";
                 font.pointSize: 15;
                 font.family: "Helvetica";
                 property int hour : 0;
                 property int minute : 0;
                 property int second : 0;

                 Timer {
                     interval: 100; running: true; repeat: true;
                     onTriggered : {
                         /*variant time = new Date();
                         variant hrs = (time.getHours() < 10) ? "0" + time.getHours() : time.getHours();
                         variant min = (time.getMinutes() < 10) ? "0" + time.getMinutes() : time.getMinutes();
                         variant sec = (time.getSeconds() < 10) ? "0" + time.getSeconds() : time.getSeconds();
                         parent.text = hrs + " : " + min + " : " + sec;
                         variant rot = time.getSeconds()/60 * 360;
                         secondHand.rotation = rot;
                         rot = time.getHours()/12 * 360;
                         hourHand.rotation = rot;*/
                     }
                 }

             }

             Rectangle {
                 color: "black";
                 width: 150;
                 height: 1;
             }
             Item {
                 anchors.horizontalCenter: parent.horizontalCenter;
                 height: 30;
                 width: 140;
                 Text {
                     id: dayofweek;
                     anchors.left: parent.left;
                     color: "#24d3dc";//"white";
                     font.pointSize: 13;
                     font.family: "Helvetica";
                     text: "Sat";

                 }
                 Text {
                     anchors.left: dayofweek.right;
                     font.pointSize: 13;
                     font.family: "Helvetica";
                    text:", January 2013";
                 }
             }
         }
    }
    Button {
        id: timeButton;
        height: timebox.height;
        width: 80;
        color: "#2c2a29";
        label: "";
        icon: "clock_ico.svg";
        opacity: 0.5;
        onButtonClick: {
            if(timebox.width){
                timebox.width = 0;
                time_text.text = timeText.text;
                opacity = 0.5;
                radius = 0;
                fillbar.visible = false;
            } else {
                timebox.width = 230;
                timebox.visible = true;
                time_text.text = "Time"
                opacity = 1.0;
                fillbar.visible = true;
                radius = 10;
            }
        }
        Text {
            id: time_text;
            color: "white";
            anchors.bottom: parent.bottom;
            anchors.right: parent.right;
            font.pointSize: 8;
            visible: false;
            font.family: "Helvetica";
            text: timeText.text;
        }
        Rectangle {
            id: fillbar;
            color: timeButton.color;
            height: parent.height;
            width: 20;
            anchors.left: parent.left;
            anchors.bottom: parent.bottom;
            visible: false;
        }
    }
}
