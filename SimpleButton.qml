import QtQuick 2.7
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.3
import QtGraphicalEffects 1.0


Rectangle {
    id: btn
    width: 120
    height: 50
    radius: 8
    anchors.verticalCenter: parent.verticalCenter
    color: btn_bkg_color
    property string btn_text: "Button"
    property color btn_text_color: "white"
    property int btn_text_size: 18
    property color btn_bkg_color: "#005588"
    signal clicked()


    Text {
        id: disp_text
        //anchors.fill: parent
        anchors.centerIn: parent
        color: btn_text_color
        text: btn_text
        font.pixelSize: btn_text_size
    }

    MouseArea {
        anchors.fill: parent
        onPressed: {
            //parent.color = "red"
            parent.scale = 1.1
        }
        onReleased: {
            //parent.color = "darkblue"
            parent.scale = 1
        }
        onClicked: {
            //console.log("button pressed!")
            btn.clicked()
        }
    }
}
