import QtQuick 1.1
//import QtQuick.Window 2.0
//import QtGraphicalEffects 1.0;
import Qt.Cmain 1.0
import UserInfo 1.0
import MenuInfo 1.0
import TicketInfo 1.0
import Reports 1.0
import Settings 1.0;

import "ticket.js" as TicketLib
import "menu.js" as MenuApi;
import "app.js" as App
import "colors.js" as Colors;
import "user.js" as User;
import "reports.js" as ReportsApi;
import "settings.js" as SettingsApi;

Rectangle {
    id: main;
    width: App.screen_w;
    height: App.screen_h;

    UserInfo { id: userApi; }
    CMain { id: mainApi; }
    MenuInfo { id: menuApi; }
    Reports { id: reportsApi; }
    Settings { id: settingsApi; }
    TicketInfo { id: ticketApi; }

    Component.onCompleted: {
        //App.screen_h = Screen.height;
        //App.screen_w = Screen.width;
        App.init( mainApi );
        App.setRoot(main);
        App.setLoader( mainLoader );
        TicketLib.init( ticketApi );
        MenuApi.init( menuApi );
        User._init( userApi );
        ReportsApi.init( reportsApi );
        SettingsApi.init( settingsApi );

        App.showLoginScreen();
    }

    Image{
        id: bg;
            width: parent.width;
            height: parent.height;
            source: App.bg_filepath;
            fillMode: Image.PreserveAspectCrop;
    }
    function enableNavbar()  {  actionArea.enabled = true; }
    function disableNavbar() {  actionArea.enabled = false; }
    function hideNavbar()    {  navback.y = navback.height; }//actions.height = 0; }
    function navBack()       {  navback.back(); }
    function hide_choice()   { noticePopup.hide(); action_slider.showing = false; }
    function show_osk( callback )         { App.keyboard_callback = callback; keyboard.show(); }
    function hide_osk()         { keyboard.hide(); }

    function choice( text ) {
        if( action_slider.showing ) {
            noticePopup.message = text;
            action_slider.show();
            noticePopup.showChoice = true;
            return;
        }

        yesButton.label = "Yes";
        noticePopup.message = text;
        noticePopup.showChoice = true;
        noticePopup.show();
    }
    function warning( text ) {
        yesButton.label = "Ok";
        noticePopup.message = text;
        noticePopup.showChoice = false;
        noticePopup.show();
    }

    function loadLoginScreen(){
        mainLoader.source = "";
        mainLoader.source = "LoginScreen.qml";//, {"width" : main.width, "height" : main.height });
    }

    function loadTableScreen(){
        //mainLoader.sourceComponent = undefined;
        //mainLoader.setSource("TableView.qml", {"width" : main.width, "height" : main.height });
        mainLoader.source = "";
        mainLoader.source = "TableView.qml";
        App.refreshTables();
    }

    function loadMenuScreen(){
        //mainLoader.asynchronous = false;
        mainLoader.source = "";
        mainLoader.source = "MenuView.qml";//, {"width" : main.width, "height" : main.height });
        //mainLoader.asynchronous = true;
    }

    function loadSplitTicketScreen() {
        mainLoader.source = "";
        mainLoader.source = "SplitTicket.qml";
        //mainLoader.setSource("SplitTicket.qml", {} );
    }

    function switchUser(){
         App.username = User.first_name + " " + User.last_name;//UserInfo.GetActiveUserName();
         App.usertitle = User.title;//UserInfo.GetActiveUserTitle();
         App.userlevel = User.level;//UserInfo.GetActiveUserLevel();


        if ( App.userlevel == 2) { //2 == Admin/Manager
                disableNavbar();
                mainLoader.source = "";
                mainLoader.source = "AdminConsole.qml"/*,
                                     {      "width":main.width,
                                            "height": main.height,
                                            "uName" : App.username,
                                            "uTitle" : App.usertitle,
                                     });*/
                //mainLoader.asynchronous = true;
        }
        else {//if ( App.userlevel ) { //1 == Server/Employee
                enableNavbar();
                mainLoader.source = "";
                mainLoader.source = "UserView.qml"/*,
                                     {
                                         "width":   main.width,
                                         "height":  main.height,
                                         "uName" :  App.username,
                                         "uTitle" : App.usertitle
                                     });*/

                //mainLoader.asynchronous = true;
                App.refreshActiveSales();
        }
    }

    function exitQML(){
        mainLoader.sourceComponent = undefined;
        Qt.quit();
    }

    Loader {
        id: mainLoader;
        //asynchronous: true;
        //focus: true;
        visible: ( status == Loader.Ready );
    }

    OnScreenKeyboard {
        id: keyboard;

        onKeyClick : {
            App.keyboard_callback( _key );
        }
    }

    FadeView {
        id: back_fader;
        duration: 300;
        color: Colors.make( Colors.blue6, "88" );
        dim: true;
        dimColor: "#cc000000";

        onClicked : {
            hide();
            navback.y = navback.height;
        }
    }

    Item {
        id: actionArea;
        anchors.bottom: parent.bottom;
        anchors.right: parent.right;
        anchors.left: parent.left;
        height: 100;
        property bool enabled : false;

        onEnabledChanged : {
            if( enabled == false && navback.y < navback.height) {
                back_fader.hide();
                navback.y = navback.height;
            }
        }

        MouseArea {
            //anytime a user clicks on this area we show the navbar
            anchors.fill: parent;
            anchors.topMargin: 70;
            preventStealing: true;
            //propagateComposedEvents: true;
            onClicked : {
                if( parent.enabled == true ) {
                    navback.y = (navback.y < navback.height) ? navback.height : parent.height - navback.height;
                    if( navback.y < navback.height ) {
                        back_fader.hide();
                    }
                    else {
                        back_fader.show();
                    }
                }
                else {
                    mouse.accepted = false;
                }
            }
        }

        Button {
            id: navback;
            anchors.right: parent.right;
            icon: "x_ico.svg";
            label: "Back";
            textColor: Colors.yellow3;//"black";
            color: "#00000000";//Colors.yellow3;
            width: 300;
            height: 300;
            y: height;
            font: "Chunkfive";
            pixelSize: height * 0.15;
            radius: 10;
            Behavior on y { NumberAnimation { duration: 180; easing.type: Easing.InOutCirc;} }
            onButtonClick : { back() }
            function back() {
                if( back_fader.running )
                    return;
                back_fader.hide();
                y = height;
                if( App.curViewName == "UserView" ) {
                    App.logoutUser();
                    App.setBackCB( null );
                }
                else if( App.curViewName == "TableView" ) {
                    App.showCurrentUserScreen();
                    App.refreshActiveSales();
                    App.setBackCB( null );
                    y = height;
                }
                else if( App.curViewName == "MenuView" ) {
                    if( App.backCB ) {
                        App.backCB();
                        App.setBackCB( null );
                        return;
                    }
                    //TicketInfo.discard_changes();
                    App.showCurrentUserScreen();
                    App.refreshActiveSales();
                    App.setBackCB( null );
                    y = height;
                }
                else if( App.curViewName == "SplitView" ) {
                    if( App.backCB ) {
                        App.backCB();
                        App.setBackCB( null );
                        return;
                    }

                    App.showCurrentUserScreen();
                    App.refreshActiveSales();
                    App.setBackCB( null );
                    y = height;
                }
            }
        }
    }

    FadeView {
        id: noticePopup;
        duration: 300;
        dim: true;
        dimColor: Colors.make( "#000000", "aa" );
        property alias message: noticeText.text;
        property alias showChoice: noButton.visible;
        color: Colors.make( Colors.blue2, "22" );

        onHalfShown : {
            /*if( noButton.visible ) { //showChoice ) {
                yesButton.label = "Yes";
            }
            else {
                yesButton.label = "Ok";
            }*/
            action_slider.show();
        }

        function okClick( state ) {
            hide();
            exitQML();
        }

        onHiding : {
            action_slider.hide();
        }

        Slide {
            id: action_slider;
            width: parent.width;
            height: parent.height * 0.4;
            color: "#cc000000"
            y: parent.height/2 - height/2;
            x: width;
            to: 0;
            from: width;
            property: "x";
            reshow: true;

            onHid : {
                noticePopup.hide();
            }

            Item {
                anchors.left: parent.left; anchors.leftMargin: parent.width * 0.05;
                anchors.right: btnrow.left; anchors.rightMargin: parent.width * 0.05;
                height: parent.height * 0.9;
                anchors.verticalCenter: parent.verticalCenter;
                Text {
                    id: noticeText;
                    anchors.fill: parent;
                    font.family: "Chunkfive";
                    wrapMode: Text.Wrap;
                    elide: Text.ElideRight;
                    color: "white";
                    font.pixelSize: parent.height * 0.20;
                    verticalAlignment: Text.AlignVCenter;
                    horizontalAlignment: Text.AlignHCenter;
                }
            }

            Row {
                id: btnrow;
                anchors.right: parent.right; anchors.rightMargin: parent.width * 0.025;
                anchors.verticalCenter: parent.verticalCenter;
                spacing: 10;

                Button {
                    id: noButton;
                    width: action_slider.height * 0.9;
                    height: action_slider.height * 0.9;
                    label: "No";
                    color: Colors.red2;
                    textColor: "white";
                    icon: "x_ico.svg";
                    font: "Chunkfive";
                    pixelSize: height * 0.15;
                    //pointSize: 20;
                    radius: 5;
                    visible: false;
                    onButtonClick : {
                        App.popupCB( false );
                        action_slider.toggle();
                    }
                }
                Button {
                    id: yesButton;
                    width: action_slider.height * 0.9;
                    height: action_slider.height * 0.9;
                    label: "Yes";
                    textColor: "white";
                    pixelSize: height * 0.15;
                    font: "Chunkfive";
                    color: Colors.green3;
                    icon: "check_white_ico.svg";
                    radius: 5;
                    onButtonClick: {
                        if( noticePopup.showChoice ) {
                            App.popupCB( true );
                        }
                        action_slider.toggle();
                    }
                }
            }
        }
    }
}
