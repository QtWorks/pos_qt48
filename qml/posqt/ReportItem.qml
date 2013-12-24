import QtQuick 1.1;
import "colors.js" as Colors;
import "app.js" as App;

Item {
    anchors.horizontalCenter: parent.horizontalCenter;

    Button {
        id: reportButton;
        anchors.fill: parent;
        label: "";
        color : "#00000000";
        pointSize : 30;
        sensitive: true;
        font : "Molot";
        radius: 5;
        onButtonClick : report_item_click(  item_id, item_label, item_total,
                                            item_salecount, item_year, item_month, item_day );

        Component.onCompleted : {
            switch( item_label ) {
                case "year" :
                    item_label_text.text = "Year " + item_year;
                    item_total_text.text = item_total.toFixed(2);
                    break;
                case "month" :
                    item_label_text.text = App.months[item_month - 1];
                    break;
                case "day" :
                    item_label_text.text = "Day " + item_day;
                    break;
                case "sale" :
                    var h = (item_hour < 10) ? ("0" + item_hour) : item_hour;
                    var m = (item_minute < 10) ? ("0" + item_minute) : item_minute;
                    item_label_text.text = h + ":" + m +" - #" + item_id + " eid: " + emp + " Total: " + item_total.toFixed(2);
                    item_label_text.font.pointSize = 18;
                    break;
                case "Back to Year" :
                    item_label_text.text = "Back to " + item_year ;
                    break;
                case "Back to Month" :
                    item_label_text.text = "Back to " + App.months[item_month - 1] + ", " + item_year;
                    break;
                case "Back to All" :
                    item_label_text.text = "back to all years";
                    break;
            }
        }

        Text {
            id: item_total_text;
            anchors.right: parent.right; anchors.rightMargin: parent.width * 0.1;
            anchors.verticalCenter: parent.verticalCenter;
            font.family: "Molot";
            font.pointSize: 20;
            color: "white";
            text: (item_total) ? item_total.toFixed(2) : "";
        }

        Text {
            id: item_label_text;
            anchors.left: parent.left; anchors.leftMargin: parent.width * 0.1;
            anchors.verticalCenter: parent.verticalCenter;
            font.family: "Molot";
            font.pointSize: 20;
            color: "white";
        }

        Rectangle {
            id: header;
            width: parent.width;
            height: 4;
            color: "white";
            anchors.bottom: parent.bottom; anchors.bottomMargin: -10;
        }
    }

}
