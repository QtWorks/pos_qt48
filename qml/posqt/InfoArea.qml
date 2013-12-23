import QtQuick 1.1

Item {
    id: infoArea;
    property bool open : false;
    property string fontFamily: "Helvetica";
    property real pointSize: 12;
    property string label : "";
    default property alias children : actionArea.children;

    Rectangle {
        id: actionArea;
        width: ctarea.width;
        height: ctarea.height;
        x: ctarea.x;
        y: ctarea.y;
        //radius: 20;
        //border.width: 2;
        //border.color: "black"
        color: "white";

        states : [
            State {
                name: "open";
                PropertyChanges { target: actionArea; y: (ctarea.y + actionArea.height); }
            },
            State {
                name: "close";
                PropertyChanges { target: actionArea; y: (ctarea.y); }
            }
        ]

        transitions:[ Transition {
            from: "open";
            to: "close";
            NumberAnimation { target: actionArea; properties: "y"; duration: 100; }
            },
            Transition {
                from: "close";
                to: "open";
                NumberAnimation { target: actionArea; properties: "y"; duration: 100; }
            }
        ]


    }

    Rectangle {
        id: ctarea;
        width: infoArea.width;
        height: infoArea.height;
        anchors.centerIn: parent;
        color: "black";

        Text {
            id: infoText;
            anchors.fill: parent;
            color: "white";
            font.family: fontFamily;
            font.pointSize: pointSize;
            textFormat: Text.RichText;
            text: label;
        }
    }

    Canvas {
        id: canvas;
        width: 30;//ctarea.width/8;
        height: 20;//ctarea.height/3;
        default property color color: "black";
        x: ctarea.x + ctarea.width/1.5;
        y: ctarea.y + ctarea.height - ctarea.border.width;

        onPaint : {
            var ctx = canvas.getContext("2d");
            ctx.clearRect(0, 0, canvas.width, canvas.height);
            if(!infoArea.open)
                return;
            ctx.save();
            ctx.fillStyle = ctarea.color;
            ctx.strokeStyle = ctarea.border.color;
            ctx.lineWidth = ctarea.border.width;
            ctx.beginPath();
            ctx.moveTo(0,0);
            //ctx.lineTo(canvas.width, 0);
            ctx.lineTo(canvas.width/2, canvas.height);
            ctx.lineTo(canvas.width, 0);
            ctx.fill();
            //ctx.stroke();
            ctx.closePath();
            ctx.restore();
        }
    }

    MouseArea{
        anchors.fill: parent;
        id: mouseArea;
        onClicked:{
            infoArea.open = !infoArea.open;
            actionArea.state = (infoArea.open) ? "open" : "close";
            canvas.requestPaint();
        }
    }

}
