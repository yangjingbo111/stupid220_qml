import QtQuick 2.7
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.0
import QtGraphicalEffects 1.0
import TcpDataClient 1.0

ApplicationWindow {
    visible: true
    width: 1280     //main window size 1280*800
    height: 800
    title: qsTr("Echo Sounder(qml)")
    //visibility: "FullScreen"   //show fullscreen

    /**
      main window background
      **/
    Rectangle {
        id: app_bkg
        anchors.fill: parent
        color: "#4EA4D0"
    }



    /**
      left page
      **/
    Page {
        id: left_page
        width: parent.width * 7 / 15  //
        height: parent.height
        //渐变背景
        LinearGradient {
            anchors.fill: parent
            start: Qt.point(0,0)
            end: Qt.point(0,height)
            gradient: Gradient {
                GradientStop {position: 0.0; color: Qt.rgba(0,0,0,1)}
                GradientStop {position: 0.5; color: Qt.rgba(0,0,80/255,1)}
                GradientStop {position: 1.0; color: Qt.rgba(0,80/255,160/255,1)}

            }

        }
    }

    /**
      right page
      **/
    Page {
        id: right_page
        anchors.right: parent.right
        width: parent.width * 8 / 15  //
        height: parent.height
        //渐变背景
        LinearGradient {
            anchors.fill: parent
            start: Qt.point(0,0)
            end: Qt.point(0,height)
            gradient: Gradient {
                GradientStop {position: 0.0; color: Qt.rgba(0,0,0,1)}
                GradientStop {position: 0.5; color: Qt.rgba(0,0,180/255,1)}
                GradientStop {position: 1.0; color: Qt.rgba(0,180/255,160/255,1)}

            }

        }

        TcpDataClient {
            id: bscan
            width: parent.width
            height: parent.height
            anchors.right: parent.right
            Component.onCompleted: {
                console.log("invoke connect function from qml world...")
                tryConnectDataServer()
            }
            onImageStatusChanged: {
                //console.log("image status has changed...")
                update()
            }
        }
    }

    /**
      Window header
      **/
    header: WinTitle {
        id: win_title
        //width: parent.width
        //height: parent.height * 17 / 200
    }

    /**
      window footer
      **/
    footer: CustomeFooter {
        id: win_footer
        //width: parent.width
        //height: 68
        //simulate the msg manually
        onSendMessage: {
            var type = JsTools.randomType()

            //console.log(JsTools.randomType())
            var msg = Qt.formatTime(new Date(),"hh:mm:ss")
            listview.sendMseeage(type, "this message is generated at " + msg)
        }
        onDisconnectServer: {
            console.log("disconnect from server ------")
            bscan.mannuallyDisconnectFromServer()
        }
    }
}
