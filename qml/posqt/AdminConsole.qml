import QtQuick 1.1
import "colors.js" as Colors
import "app.js" as App
import "user.js" as User

Item {
    id: adminConsole;
    width: App.screen_w;
    height: App.screen_h;

    property int pageIndex : 0;
    property color headerColor: Colors.neon_blue;
    property bool active : false;

    function set_interactive( interactive ) {
        parallax.interactive = interactive;
    }

    function viewChange(){
        indicator.moveTo(parallax.position);
    }

    Component.onCompleted : {
        App.admin_console = adminConsole;
    }

    ParallaxView {
        id: parallax;
        anchors.left: parent.left;
        anchors.bottom: parent.bottom;
        width: parent.width;
        height: parent.height;

        onPositionChanged : viewChange();

        Item {
            id: page1;
            width: App.screen_w;
            height: App.screen_h;
            Backdrop{}
            AdminView {}
        }

        Item {
            id: page2;
            width: page1.width;
            height: page1.height;
            Backdrop{}
            SettingsView {}
        }

        Item {
            id: page3;
            width: page1.width;
            height: page1.height;
            Backdrop{}
            ReportView {}
        }
    }


    function onexitchoice( choice )
    {
        if( choice )
            App.exit();
        App.popupCB = null;
    }

   Toolbar {
        id: tab;
        onItemClick: {
            if(item === 3){
                App.choice( "Really Exit?", onexitchoice );
            } else if( item === -1 ){
                App.logoutUser();
            }
            adminConsole.pageIndex = item;
            indicator.moveTo(item);
            parallax.scrollTo(item);
        }
   }
   Rectangle {
        id: indicator;
        color: Colors.blue7;
        width: tab.width/5 - 5;
        height: parent.height * 0.005;
        x: tab.x + tab.width/5;
        anchors.top: tab.bottom;
        property int index : 0;
        function moveTo(index)  {
            indicator.index = index;
            indicator.x = tab.x + (tab.width/5) + ((index) * (tab.width/5));
        }
        Behavior on x { SmoothedAnimation { velocity: 400; } }
   }
   Row {
       id: colorRow;
       anchors.left: parent.left;
       anchors.bottom: parent.bottom;
       anchors.right: parent.right;

       spacing: 3;
       Rectangle{
           id: blue;
           width: parent.width/3;// * 2;
           height: 6;
           color: Colors.main_blue;
       }
       Rectangle{
           id: green;
           width: blue.width;
           height: 6;
           color: Colors.main_green;
       }

        Rectangle{
            id: yellow;
           width: blue.width;
           height: 6;
           color: Colors.main_yellow;
        }
   }

    TimeBar{ id: clock;}

    UserIcon {
        id: userIcon;
        userName: User.first_name + ", " + User.last_name;
        userTitle: User.title;
    }
}
