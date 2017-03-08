import QtQuick 2.7
import QtGraphicalEffects 1.0

Item {
    id: win_title
    /**
      leave the width and height to be decided by main window
      **/
    width: parent.width
    height: 68

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
    //应用程序标题
    Text {
        id: app_name
        text: qsTr("ECHO SOUNDER")
        color: Qt.rgba(100/255,200/255,1,1)
        font {
            bold: true
            pixelSize: 50
        }
        anchors {
            left: parent.left
            verticalCenter: win_title.verticalCenter
            leftMargin: 8
        }

    }

    Image {
        id: icon_new_msg
        anchors {
            left: app_name.right
            leftMargin: 16
            verticalCenter: parent.verticalCenter
        }
        source: "qrc:/res/beep.png"
        visible: true
    }

    //时间文本
    Text {
        id: time_text
        property var locale: Qt.locale("cn")
        property date currentTime: new Date()
        property string timeString : Qt.formatTime(currentTime,"hh:mm:ss")
        color: Qt.rgba(1,1,1,1)
        font {
            pixelSize: 25
        }
        anchors {
            right: parent.right
            bottom: date_text.top
            bottomMargin: 4
            rightMargin: 8
        }
        text: timeString
    }
    //日期文本
    Text {
        id: date_text
        property var locale: Qt.locale("cn")
        property date currentTime: new Date()
        property string dateString : Qt.formatDate(currentTime,"yyyy/MM/dd")
        color: Qt.rgba(1,1,1,1)
        font {
            pixelSize: 25
        }
        anchors {
            bottom: parent.bottom
            bottomMargin: 4
            right: parent.right
            rightMargin: 8
        }
        text: dateString
    }
    //定时器 500ms
    Timer {
        id: timer
        property bool show_icon: true
        interval: 500
        repeat: true
        running: true


        onTriggered: {
            show_icon = !show_icon
            icon_new_msg.visible = show_icon
            time_text.text = Qt.formatTime(new Date(),"hh:mm:ss")
            date_text.text = Qt.formatDate(new Date(),"yyyy/MM/dd")

        }
    }
}
