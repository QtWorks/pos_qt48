import QtQuick 1.1
import "colors.js" as Colors;
import "app.js" as App;

FadeView {
    id: root;
    width: App.screen_w;
    height: App.screen_h;
    color: Colors.make( Colors.yellow3, "aa" );
    dim: true;
    dimColor: "#cc000000";
    property variant cb : null;

    onHidden : {
        App.load_subview("");
    }

    onHalfShown : {
    }

    onClicked : {
        hide();
    }

    Component.onCompleted : {
        show();
    }

    Image {
        source : "print_ico.svg";
        sourceSize.width: parent.height * 0.25;
        sourceSize.height: parent.height * 0.25;
        anchors.left: parent.left; anchors.leftMargin: parent.width * 0.03;
        anchors.verticalCenter: parent.verticalCenter;
    }
}
