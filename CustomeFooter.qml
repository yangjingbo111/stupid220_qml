import QtQuick 2.7
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.3
import QtGraphicalEffects 1.0
//import HomeButton 1.0    //defined in C++
//import SqlMsgModel 1.0
Item {
    id: footer
    width: parent.width
    height: 68

    //渐变背景
    LinearGradient {
        anchors.fill: parent
        start: Qt.point(0,0)
        end: Qt.point(0,height)
        gradient: Gradient {
            GradientStop {position: 1.0; color: Qt.rgba(0,0,0,1)}
            GradientStop {position: 0.5; color: Qt.rgba(0,0,80/255,1)}
            GradientStop {position: 0; color: Qt.rgba(0,80/255,160/255,1)}

        }
    }
    /*HomeButton {
        id: home_btn_cpp
    }
*/
    signal sendMessage
    signal disconnectServer


    Row {
        spacing: 16//32
        topPadding: 16
        leftPadding: (parent.width - add_msg_btn.width - /*messageField.width -*/ spacing*5 - home_btn.width*6)/2
        SimpleButton {
            id: home_btn
            btn_text: qsTr("Home")
            btn_text_size: 25
            onClicked: {
                //console.log("home clicked")
                home_btn_cpp.
                home_btn_cpp.back_to_home()
            }
        }
        SimpleButton {
            id: settings_btn
            btn_text: qsTr("Settings")
            btn_text_size: 25
        }
        SimpleButton {
            id: filter_btn
            btn_text: qsTr("Filter")
            btn_text_size: 25
        }
        SimpleButton {
            id: prev_btn
            btn_text: qsTr("Prev")
            btn_text_size: 25
        }
        SimpleButton {
            id: next_btn
            btn_text: qsTr("Next")
            btn_text_size: 25
        }
        SimpleButton {
            id: print_btn
            btn_text: qsTr("Print")
            btn_text_size: 25
            onClicked: {
                //console.log("home clicked")
                home_btn_cpp.set_brightness()
            }
        }


        //manual add message button
        SimpleButton {
            id: add_msg_btn
            btn_text: qsTr("Add Msg")
            btn_text_size: 25
            btn_text_color: "red"
            onClicked: {
                console.log("add msg to database")//+text_area.canPaste)
                footer.sendMessage()
                //text_area.text = "";
                 //ListView.model.sendMessage("518", "message")
                //home_btn_cpp.set_brightness()
            }
        }

        SimpleButton {
            id: disconnect_btn
            btn_text: qsTr("Disconnect")
            btn_text_size: 25
            btn_text_color: "black"
            onClicked: {
                footer.disconnectServer()
            }
        }


    }
    /*
    Flickable{
        id: messageField
        width: 300
        height: 68
        anchors.right: parent.right
        TextArea.flickable: TextArea {
            id: text_area

            placeholderText: qsTr("Compose message")
            color: "white"
            font.pixelSize: 20
            wrapMode: TextArea.Wrap

        }
    }
*/
}
