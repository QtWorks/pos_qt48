import QtQuick 1.1
//import QtQuick.Window 2.0
import "app.js" as App;
//import UserInfo 1.0
import "user.js" as User;
import "colors.js" as Colors;

Rectangle {
    property int userInput : 0;
    width: 1600;//App.screen_w * 9;
    height: 900;//App.screen_h * 9;
    //anchors.centerIn: parent;
    color: Colors.make( Colors.white, "22");

    FadeView {
        id: fadeview;
        duration: 300;
        color: Colors.make( Colors.violet2, "99" );
        dim: true;
        dimColor: Colors.make( "#000000" , "cc" );

        onHalfHidden : {
            App.switchUser();
        }

        LoginPad {
            id: pad;
            property string passcode: "";
            onClickStateChanged : {
                if(buttonID == 11){
                    passcode = "";
                    maskOpacity = 0.0;
                    mask = "";
                } else if(buttonID != 10){
                    passcode += buttonID;
                    maskOpacity = 1.0;
                    mask += "*";
                }
                else {
                    maskOpacity = 0.0;
                    mask = "";
                    if( User.login( passcode ) > -1 ) {
                        //Logins sucessful
                        bgColor = Colors.make( Colors.green3, "aa" );
                        /*fadeview.duration = 500; */
                        fadeview.hide(400);
                        pad.hide(400);
                        sucessTimer.start();
                        passcode = "";
                    } else {
                        fail();
                        //bgColor = Colors.make( Colors.red2, "cc" );
                        //failTimer.start();
                    }
                    passcode = "";
                }
            }
        }
    }

    Timer {
        id: sucessTimer;
        running: false;
        interval: 1000;
        repeat: false;

        onRunningChanged : {
            if( running ) {
                //fadeview.hide(400);
                //pad.hide(400);
            }
        }
    }
    Timer {
        id: failTimer;
        running: false;
        interval: 250;
        repeat: false;

        onRunningChanged : {
            if( !running ) {
               pad.bgColor = "#00000000";
            }
        }
    }
    Component.onCompleted : {
        fadeview.show();
        pad.show();
    }
}
