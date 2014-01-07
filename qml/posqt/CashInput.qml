import QtQuick 1.1;
import "app.js" as App;
import "colors.js" as Colors;

Item {
    id: rootWindow;
    property double total : 0.0;
    property string _raw : "";
    property color keyColor : Colors.make( Colors.blue3, "aa" );
    property color extra_keyColor: Colors.make( Colors.marine_violet, "aa" );
    property int dim : 150;
    property int spacing: 10;
    property int point_size : 20;
    property alias show_input : amountInput.visible;

    signal buttonPressed(variant _key);
    signal extraDigitPressed(string _extra);
    signal payClicked(variant _total);
    signal cancelClick();

    anchors.fill: parent;

    function hide() {
        digit_slide0.hide(1000);
        digit_slide1.hide(800);
        digit_slide2.hide(600);
        extra_digit_slide.hide(400);
        amount_slide.hide(200);
    }

    function show( amt ) {
        if( amt ){
            _raw = String(amt.toFixed(2))
            total = Number(_raw);//amt.toFixed(2);
            total = total.toFixed(2);
            amountInput.text = String(amt.toFixed(2))
        }
        digit_slide0.show(200);
        digit_slide1.show(400);
        digit_slide2.show(600);
        extra_digit_slide.show(800);
        if( show_input )
            amount_slide.show( 1000 );
    }

    function clear() {
        _raw = "";
        total = 0.0;
        amountInput.text = "";
    }

    Slide {
        id: amount_slide;
        to: 0;
        from: width;
        x: width;
        property: "x";
        width: parent.width;
        height: parent.height * 0.15;

        color: "black";
        radius: 10;

        anchors.top: parent.top; anchors.topMargin: height * 0.4;

        Rectangle {
            color: "black";
            width: parent.width * 0.28;
            height: parent.height * 0.7;
            anchors.verticalCenter: parent.verticalCenter;
            anchors.right: parent.right; anchors.rightMargin: parent.width * 0.365;

            Text {
                id: amountInput;
                anchors.fill: parent;
                text: "";
                font.pixelSize: parent.height * 0.85;
                font.family: "Molot";
                color: "white";
                maximumLineCount: 1;
                horizontalAlignment: Text.AlignHCenter;
                verticalAlignment: Text.AlignVCenter;
            }
        }
    }

    Slide {
        id: digit_slide0;

        height: parent.height;
        width: parent.width * 0.09;
        to: 0;
        from: -height;
        y: -height;

        anchors.right: digit_slide1.left;

        color: "#88000000";
        Column {
            spacing: 5;
            anchors.horizontalCenter: parent.horizontalCenter;
            anchors.top: parent.top; anchors.topMargin: parent.height * 0.07;

            Button { font:"Molot";height: digit_slide0.width * 0.95; width: height; radius : 1; pixelSize: height * 0.3; color: keyColor; label: "7"; onButtonClick : { buttonPressed( label ); } }
            Button { font:"Molot";height: digit_slide0.width * 0.95; width: height; radius : 1; pixelSize: height * 0.3; color: keyColor; label: "4"; onButtonClick : { buttonPressed( label ); } }
            Button { font:"Molot";height: digit_slide0.width * 0.95; width: height; radius : 1; pixelSize: height * 0.3; color: keyColor; label: "1"; onButtonClick : { buttonPressed( label ); } }
            Button {font:"Molot"; height: digit_slide0.width * 0.95; width: height; radius : 1; pixelSize: height * 0.3; color: keyColor; label: "0"; onButtonClick : { buttonPressed( label ); } }
        }
    }
    Slide {
        id: digit_slide1;

        height: parent.height;
        width: parent.width * 0.09;
        to: 0;
        from: -height;
        y: -height;

        anchors.right: digit_slide2.left;

        color: "#88000000";
        Column {
            spacing: 5;
            anchors.horizontalCenter: parent.horizontalCenter;
            anchors.top: parent.top; anchors.topMargin: parent.height * 0.07;
            Button { font:"Molot";height: digit_slide1.width * 0.95; width: height; radius : 1; pixelSize: height * 0.3; color: keyColor; label: "8"; onButtonClick : { buttonPressed( label ); } }
            Button { font:"Molot";height: digit_slide1.width * 0.95; width: height; radius : 1; pixelSize: height * 0.3; color: keyColor; label: "5"; onButtonClick : { buttonPressed( label ); } }
            Button { font:"Molot";height: digit_slide1.width * 0.95; width: height; radius : 1; pixelSize: height * 0.3; color: keyColor; label: "2"; onButtonClick : { buttonPressed( label ); } }
            Button { font:"Molot";height: digit_slide1.width * 0.95; width: height; radius : 1; pixelSize: height * 0.3; color: keyColor; label: "."; onButtonClick : { buttonPressed( label ); } }
        }
    }
    Slide {
        id: digit_slide2;

        height: parent.height;
        width: parent.width * 0.09;
        to: 0;
        from: -height;
        y: -height;

        anchors.right: extra_digit_slide.left;
        color: "#88000000";
        Column {
            spacing: 5;
            anchors.horizontalCenter: parent.horizontalCenter;
            anchors.top: parent.top; anchors.topMargin: parent.height * 0.07;

            Button {  font:"Molot";height: digit_slide2.width * 0.95; width: height; radius : 1; pixelSize: height * 0.3; color: keyColor; label: "9"; onButtonClick : { buttonPressed( label ); } }
            Button { font:"Molot"; height: digit_slide2.width * 0.95; width: height; radius : 1; pixelSize: height * 0.3; color: keyColor; label: "6"; onButtonClick : { buttonPressed( label ); } }
            Button { font:"Molot"; height: digit_slide2.width * 0.95; width: height; radius : 1; pixelSize: height * 0.3; color: keyColor; label: "3"; onButtonClick : { buttonPressed( label ); } }
            Button {
                height: digit_slide2.width * 0.95;
                width: height;
                radius: 1;
                pointSize: point_size;
                color: keyColor;
                onButtonClick : {
                    _raw = _raw.slice(0, -1);
                    buttonPressed("");
                }
                Image {
                    anchors.fill: parent;
                    sourceSize.height: parent.height;
                    sourceSize.width: parent.width;
                    source: "x_ico.svg";
                }
            }
        }
    }

    Slide {
        id: extra_digit_slide;
        to: 0;
        from: -height;
        color: "#88000000";
        y: -height;
        width: parent.width * 0.09;
        height: parent.height;

        anchors.right: parent.right; anchors.rightMargin: width/3;

        Column {
            id: extraColumn;

            anchors.horizontalCenter: parent.horizontalCenter;
            anchors.top: parent.top; anchors.topMargin: parent.height * 0.07;
            spacing: 5;

            Button {
                id: tenBtn;
                pixelSize: height * 0.4;
                font: "Molot";
                height: width;
                width: extra_digit_slide.width * 0.95;
                radius: 1;
                color: extra_keyColor;
                label: "10";
                onButtonClick : { extraDigitPressed( label ); }
            }
            Button {
                pixelSize: height * 0.4;
                font: "Molot";
                color: extra_keyColor;
                height: width;
                width: extra_digit_slide.width * 0.95;
                radius: 1;
                label: "20";
                onButtonClick : { extraDigitPressed( label ); }
            }
            Button {
                pixelSize: height * 0.4;
                color: extra_keyColor;
                radius: 1;
                height: width;
                font: "Molot";
                width: extra_digit_slide.width * 0.95;
                label: "50";
                onButtonClick :  extraDigitPressed( label );
            }
            Button {
                height: width;
                width: extra_digit_slide.width * 0.95;
                radius: 1;
                font: "Molot";
                pixelSize: height * 0.4;
                color: extra_keyColor;
                label: "100";
                onButtonClick: extraDigitPressed( label );
            }
        }//Column
    }



    onExtraDigitPressed : {
        total += Number(_extra);
        _raw = String(total);
        amountInput.text = "$ " + total.toFixed(2);
    }

    onButtonPressed : {
        //don't allow leading zero's
        if( _key === "0" && !_raw.length ) return;

        var deci_p = _raw.indexOf(".");
        if( deci_p != -1 ) {
            //Don't allow two decimals. Only allow 2 decimal places
            if( _key !== "." && (_raw.length - deci_p < 3) ) {
                _raw += _key;
            }
        } else {
            //Adding a decimal to empty amount prepend a '0'
            if( _key === '.' && !_raw.length ) {
                _raw = "0.";
            }
            else {
                _raw += _key;
            }
        }

        if( !_raw ) {
            amountInput.text = "$";
            total = 0;
        }
        else {
            total = Number(_raw);
            amountInput.text = "$ " + _raw;

            total = total.toFixed(2);

            amountInput.text = "$ " + _raw;
        }
    }
}
