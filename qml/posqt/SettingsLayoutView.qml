import QtQuick 1.1
import "colors.js" as Colors;
import "settings.js" as Settings;
import "app.js" as App;

FadeView {
    id: root;
    width: App.screen_w;
    height: App.screen_h;
    color: Colors.make( Colors.blue5, "aa" );
    dim: true;
    dimColor: "#cc000000";
    property variant cb : null;

    onHalfShown : {
    }

    onClicked : {
        hide();
    }
    Image {
        source : "table_no_legs_ico.svg";
        sourceSize.width: parent.height * 0.25;
        sourceSize.height: parent.height * 0.25;
        anchors.left: parent.left; anchors.leftMargin: parent.width * 0.03;
        anchors.verticalCenter: parent.verticalCenter;
    }

}
