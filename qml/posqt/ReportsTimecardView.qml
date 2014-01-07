import QtQuick 1.1
import "app.js" as App;
import "settings.js" as Settings;
import "reports.js" as Reports;
import "colors.js" as Colors;

FadeView {
    width: App.screen_w;
    height: App.screen_h;
    color: Colors.make( Colors.blue3, "aa" );
    dim: true;
    dimColor: "#cc000000";
    property int seconds_total : 0;

    Component.onCompleted: {
        show();
    }

    onHidden : {
        App.load_subview("");
    }

    onClicked : {
        hide();
    }

    Item {
        anchors.left: parent.left; anchors.leftMargin: parent.width * 0.001;
        anchors.verticalCenter: parent.verticalCenter;
        width: parent.width * 0.4;
        height: parent.height * 0.5;

        ListView {
            id: userlist;
            spacing: 5;
            anchors.fill: parent;
            model: Settings.api.userModel;
            delegate: user_item;
            clip: true;

            Component {
                id: user_item;
                Button {
                    height: userlist.height/5 - 5;
                    width: userlist.width * 0.95;
                    anchors.horizontalCenter: user_item.horizontalCenter;
                    font: "Molot";
                    color: Colors.blue6;
                    pixelSize: height * 0.2;
                    label: user_name;
                    onButtonClick : {
                        seconds_total = 0;
                        Reports.api.get_emp_hours(user_id);
                    }
                }
            }
        }
    }

    Text {
        anchors.bottom: tc_spacer.top; anchors.bottomMargin: parent.height * 0.01;
        anchors.left: tc_spacer.left;
        //visible: seconds_total > 0;
        text: "Total Hours: " + (seconds_total / 3600).toFixed(2);
        font.family: "Molot";
        font.pixelSize: parent.height * 0.05;
        color: "white";
    }

    Item {
        id: tc_spacer;
        anchors.right: parent.right; anchors.rightMargin: parent.width * 0.001;
        anchors.verticalCenter: parent.verticalCenter;
        width: parent.width * 0.4;
        height: parent.height * 0.5;

        ListView {
            id: timecard_list;
            spacing: 5;
            model: Reports.api.empTimecardModel;
            delegate: timecard_item;
            anchors.fill: parent;
            clip: true;

            Component {
                id: timecard_item;
                Button {
                    height: timecard_list/5 - 5;
                    width: timecard_list.width * 0.95;
                    anchors.verticalCenter: timecard_item.horizontalCenter;
                    font: "Molot";
                    color: Colors.yellow3;
                    pixelSize: height * 0.2;
                    label: start_time.slice(0,19) + "  " + end_time.slice(0, 19);

                    Component.onCompleted : {
                        seconds_total += total_seconds;
                    }

                    Text {
                        anchors.bottom: parent.bottom;
                        anchors.right: parent.right; anchors.rightMargin: parent.width * 0.01;
                        text: (total_seconds/3600).toFixed(2);
                        color: "white";
                        font.family: "Molot";
                        font.pixelSize: parent.height * 0.2;
                    }
                }
            }
        }
    }
}
