import QtQuick 1.1
import "colors.js" as Colors;
import "settings.js" as Settings;
import "app.js" as App;

FadeView {
    id: root;
    width: App.screen_w;
    height: App.screen_h;

    color: Colors.make( Colors.orange, "aa" );
    dim: true;
    dimColor: "#cc000000";
    property int ulevel : Settings.api.user_level;

    onHalfShown : {
        whitebar.show();
        userlist_slide.show();
    }

    onClicked : {
        hide();
        whitebar.hide();
        userlist_slide.hide();
        user_edit_slide.hide();
    }

    onHidden : {
        App.load_subview("");
    }

    Component.onCompleted : {
        show();
    }

    Image {
        source : "hardhat_ico.svg";
        sourceSize.width: parent.height * 0.25;
        sourceSize.height: parent.height * 0.25;
        anchors.left: parent.left; anchors.leftMargin: parent.width * 0.03;
        anchors.verticalCenter: parent.verticalCenter;
    }

    Slide {
        id: userlist_slide;
        anchors.right: parent.right; anchors.rightMargin: parent.width * 0.01;
        y: -height;
        from: -height;
        to: 0;
        width: parent.width * 0.45;
        height: parent.height * 0.85;
        color: "#99000000";

        Item {
            anchors.top: parent.top; anchors.topMargin: parent.height * 0.15;
            anchors.bottom: parent.bottom; anchors.bottomMargin: parent.height * 0.01;
            anchors.left: parent.left; anchors.leftMargin: parent.width * 0.01;
            anchors.right: parent.right; anchors.rightMargin: parent.width * 0.01;
           // width: parent.width;

            ListView {
                id: userlist;
                spacing: 10;
                anchors.fill: parent;
                model: Settings.api.userModel;
                delegate: user_item;
                clip: true;

                Component {
                    id: user_item;
                    Button {
                        height :userlist.height/5 - 10;
                        width: userlist.width * 0.95;
                        anchors.horizontalCenter: user_item.horizontalCenter;
                        act_on_cancel: true;
                        font: "Molot";
                        color: Colors.blue6;
                        pixelSize: height * 0.2;
                        textColor: "white";
                        label: user_name;
                        sensitive: true;

                        onButtonClick : {
                            if( user_name == "add_user" ) {
                                var id = Settings.api.create_new_user();
                                if( id > 0 ) {
                                    Settings.api.reset_user_list();
                                    Settings.api.select_user( id );
                                    user_edit_slide.show();
                                }

                                return;
                            }
                            Settings.api.select_user( user_id );
                            user_edit_slide.show();
                            /*label = Qt.binding(function() {
                                return (user_id === Settings.api.user_id) ? Settings.api.user_name : user_name;
                            });
                            level.text = Qt.binding(function() {
                                return (user_id === Settings.api.user_id )  ? Settings.api.user_level : user_level;
                            });*/
                            label = function() { return (user_id === Settings.api.user_id) ? Settings.api.user_name : user_name; }
                            //level.text = function() { return (user_id === Settings.api.user_id) ? Settings.api.user_level : user_level; }
                        }

                        Text {
                            id: level;
                            anchors.left: parent.left; anchors.leftMargin: parent.width * 0.1;
                            anchors.verticalCenter: parent.verticalCenter;
                            color: "white";
                            font.family: "Molot";
                            visible: (user_level > -1) ? true : false;
                            font.pixelSize: parent.height * 0.3;
                            text: user_level;
                        }
                        Component.onCompleted : {
                            if( user_first_name === "add_user" ) {
                                color = Colors.green4;
                                label = "Add New User";
                            }
                        }
                    }
                }
            }
        }
    }
    Slide {
        id: user_edit_slide;
        anchors.right: userlist_slide.left; anchors.rightMargin: parent.width * 0.01;
        y: -height;
        from: -height;
        to: 0;
        width: parent.width * 0.2;
        height: userlist_slide.height;
        color: "black";
        duration: 400;
        reshow: true;

        Rectangle {
            id: user_name_b;
            anchors.horizontalCenter: parent.horizontalCenter;
            anchors.top: parent.top; anchors.topMargin: parent.height * 0.15;

            radius: 3;
            color: Colors.grey5;
            width: parent.width * 0.95;
            height: parent.height * 0.2;

            Button {
                id: first_name_btn;
                anchors.top: parent.top;
                anchors.left: paperclip_ico.right; anchors.leftMargin: parent.width * 0.02;
                anchors.right: parent.right;
                height: parent.height * 0.5;
                //color: "red";
                font: "Molot";
                pixelSize: height * 0.4;
                halign: Text.AlignRight;
                label: Settings.api.user_first_name;
                onButtonClick : {
                    Settings.name_change_cb = first_name_change;
                    user_name_screen.text = Settings.api.user_first_name;
                    user_name_screen.show();
                }
            }
            Button {
                id: last_name_btn;
                anchors.top: first_name_btn.bottom;
                anchors.right: parent.right;
                anchors.left: paperclip_ico.right; anchors.leftMargin: parent.width * 0.02;
                anchors.bottom: parent.bottom;
                //color: "blue";
                font: "Molot";
                pixelSize: height * 0.4;
                halign: Text.AlignRight;
                label: Settings.api.user_last_name;
                onButtonClick : {
                    Settings.name_change_cb = last_name_change;
                    user_name_screen.text = Settings.api.user_last_name;
                    user_name_screen.show();
                }
            }

            Image {
                id: paperclip_ico;
                anchors.left: parent.left; anchors.leftMargin: parent.width * 0.01;
                anchors.verticalCenter: parent.verticalCenter;

                sourceSize.width: parent.height * 0.6;
                sourceSize.height: parent.height * 0.6;

                source: "reports_ico3.svg";
            }
        }
        Slide {
            id: level_dial;
            anchors.right: user_level_b.right;
            anchors.left: user_level_b.left;
            height: user_level_b.height;
            radius: 3;
            y: user_level_b.y;
            from: user_level_b.y;
            to: user_level_b.y + height + 10;

            Button {
                anchors.left: parent.left;
                anchors.top: parent.top;
                anchors.bottom: parent.bottom;
                width: parent.width/2 - 5;
                icon: "plus_ico.svg";
                onButtonClick : {
                    Settings.api.set_user_level( Settings.api.user_level + 1 );
                    Settings.api.reset_user_list();
                }
            }

            Button {
                anchors.right: parent.right;
                anchors.top: parent.top;
                anchors.bottom: parent.bottom;
                width: parent.width/2 - 5;
                icon: "minus_ico.svg";
                onButtonClick : {
                    Settings.api.set_user_level( Settings.api.user_level - 1 );
                    Settings.api.reset_user_list();
                }
            }
        }

        Button {
            id: user_level_b;
            anchors.horizontalCenter: parent.horizontalCenter;
            anchors.top: user_name_b.bottom; anchors.topMargin: parent.height * 0.01;

            radius: 3;
            color: Colors.blue6;
            width: parent.width * 0.95;
            height: parent.height * 0.11;

            onButtonClick : {
                level_dial.toggle();
                /*name_change_cb = change_item_name;
                item_price_screen.show();*/
            }

            Text {
                text: Settings.api.user_level;
                font.family: "Helvetica";
                font.pixelSize: parent.height * 0.3;
                color: "white";
                anchors.right: parent.right; anchors.rightMargin: parent.width * 0.1;
                anchors.verticalCenter: parent.verticalCenter;
            }

            Image {
                anchors.left: parent.left; anchors.leftMargin: parent.width * 0.01;
                anchors.verticalCenter: parent.verticalCenter;

                sourceSize.width: parent.height * 0.6;
                sourceSize.height: parent.height * 0.6;

                source: "tag_ico.svg";
            }
        }
        Button {
            id: user_pass_b;
            anchors.horizontalCenter: parent.horizontalCenter;
            anchors.top: level_dial.bottom; anchors.topMargin: parent.height * 0.01;

            radius: 3;
            color: Colors.blue3;
            pixelSize: parent.height * 0.03;
            width: parent.width * 0.95;
            height: parent.height * 0.11;

            onButtonClick : password_input.show();

            Text {
                text: Settings.api.user_password;
                font.family: "Helvetica";
                font.pixelSize: parent.height * 0.3;
                color: "white";
                anchors.right: parent.right; anchors.rightMargin: parent.width * 0.1;
                anchors.verticalCenter: parent.verticalCenter;
            }

            Image {
                anchors.left: parent.left; anchors.leftMargin: parent.width * 0.01;
                anchors.verticalCenter: parent.verticalCenter;
                sourceSize.width: parent.height * 0.6;
                sourceSize.height: parent.height * 0.6;
                source: "eye_ico.svg";
            }
        }

        Button {
            id: user_delete_b;
            anchors.horizontalCenter: parent.horizontalCenter;
            anchors.top: user_pass_b.bottom; anchors.topMargin: parent.height * 0.01;

            radius: 3;
            color: Colors.red2;
            pixelSize: parent.height * 0.03;
            width: parent.width * 0.95;
            height: parent.height * 0.11;

            onButtonClick: App.choice( "Delete " + Settings.api.user_name + "?", on_user_delete );

            Text {
                text: "Delete"
                font.family: "Helvetica";
                font.pixelSize: parent.height * 0.3;
                color: "white";
                anchors.right: parent.right; anchors.rightMargin: parent.width * 0.1;
                anchors.verticalCenter: parent.verticalCenter;
            }

            Image {
                anchors.left: parent.left; anchors.leftMargin: parent.width * 0.01;
                anchors.verticalCenter: parent.verticalCenter;
                sourceSize.width: parent.height * 0.6;
                sourceSize.height: parent.height * 0.6;
                source: "x_ico.svg";
            }
        }

        Image {
            id: edit_ico;
            anchors.top: parent.top; //anchors.topMargin: parent.height * 0.12;
            anchors.horizontalCenter: parent.horizontalCenter;
            source: "edit_ico.svg";
            sourceSize.width: parent.width * 0.15;
            sourceSize.height: parent.width * 0.15;
        }
    }

    Slide {
        id: whitebar;
        anchors.top: parent.top; anchors.topMargin: parent.height * 0.11;
        width: parent.width;
        height: 4;
        color: "white";
        property: "x";
        x: -width;
        from: -width;
        to: 0;
        duration: 400;
    }

    TextInputView {
        id: user_name_screen;
        max: 128;

        onAccept : {
            hide();
            Settings.name_change_cb( _text );
        }

        onReject : {
            text = "";
            hide();
        }
    }

    LoginPad {
        id: password_input;
        bgColor: "white";

        onClickStateChanged : {
            if( buttonID != 10  && buttonID != 11) {
                maskOpacity = 1.0;
                mask += buttonID;
                return;
            }
            maskOpacity = 0.0;
            mask = "";
        }

        onOkClick : {
            Settings.api.set_user_password( _password );
            Settings.api.reset_user_list();
            hide();
        }

        onXClick : hide();
    }

    function on_user_delete( choice )
    {
        if( choice ) {
            user_edit_slide.hide();
            Settings.api.delete_user();
            Settings.api.reset_user_list();
        }
    }

    function first_name_change( first ) {
        Settings.api.set_user_firstname( first );
        Settings.api.reset_user_list();
    }

    function last_name_change( last ) {
        Settings.api.set_user_lastname( last );
        Settings.api.reset_user_list();
    }

    function find_username( id ) {
        return Settings.api.find_user_first_name( id ) + " " + Settings.api.find_user_last_name( id );
    }
}

