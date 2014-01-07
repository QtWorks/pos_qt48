import QtQuick 1.1
import "colors.js" as Colors;

Button {
    width: root.height * 0.06;
    height: root.height * 0.06;
    radius: 5;
    font: "Chunkfive";
    pixelSize: height * 0.25;
    color: Colors.blue3;
    onButtonClick: {
        if( label.length == 1 ) {
            root.keyClick( label )
            if( rshift ) {
                root.shift = false;
            }
        }
        else {
            if( label === "BACKSPACE" || label === "backspace" )
                root.keyClick("backspace");
            if( label === "SPACE" || label === "space" )
                root.keyClick(" ");
        }
    }

    property bool rshift : root.shift;
    property bool rcaps : root.caps;

    onRcapsChanged : {
        if( rcaps ) {
            root.shift = false;
            label = label.toUpperCase();
        }
        else {
            root.shift = false;
            label = label.toLowerCase();
        }
    }

    onRshiftChanged : {
        if( !rcaps && rshift )
            label = label.toUpperCase();
        else if( !rcaps )
            label = label.toLowerCase();
    }
}
