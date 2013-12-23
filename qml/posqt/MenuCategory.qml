import QtQuick 1.1

Button {
    id: root;
    property int cat_id : -1;
    property color tc : undefined;
    property string cat_name : "Undefined";
    property bool selected : false;
    signal catClick();
    signal itemClick(variant _name, variant _id, variant _issub);

    function select() {
        selected = true;
    }

    function deselect() {
        if( !selected ) return;
        selected = false;
    }

    label: cat_name;
    color: tc;
    textColor: "white";
    pixelSize: height * 0.25;
    font: "Chunkfive";
    onButtonClick: catClick();
}
