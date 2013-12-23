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
        }
        else {
            if( label === "BACKSPACE" || label === "backspace" )
                root.keyClick("backspace");
            if( label === "SPACE" || label === "space" )
                root.keyClick(" ");
        }
    }

    property bool rshift : root.shift;

    onRshiftChanged : {
        if( rshift )
            label = label.toUpperCase();
        else
            label = label.toLowerCase();
    }
}
