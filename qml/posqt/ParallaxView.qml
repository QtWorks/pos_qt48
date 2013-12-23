import QtQuick 1.1

Item {
    id: root;
    property int currentIndex : 0;
    default property alias content : visualModel.children;
    property real position : 0.0;


    signal scrollTo(int index)
    onScrollTo : {
        list.positionViewAtIndex(index, ListView.Contain);
    }


    ListView {
        id: list;
        anchors.fill : parent;
        spacing: 5;

        currentIndex : root.currentIndex;
        onHorizontalVelocityChanged: {
            root.position = contentX / root.width;
        }
        orientation: Qt.Horizontal;
        boundsBehavior: Flickable.StopAtBounds
        model: VisualItemModel { id: visualModel; }


        highlightRangeMode: ListView.StrictlyEnforceRange;
        snapMode: ListView.SnapToItem;
    }
}
