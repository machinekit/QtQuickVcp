import QtQuick 2.0
import QtQuick.Controls 1.0
import QtQuick.Controls.Styles 1.0

Button {
    id: button

    property string iconSource

    /*style: ButtonStyle {
            background: Rectangle {
                implicitWidth: 100
                implicitHeight: 25
                border.width: 0 //control.activeFocus ? 2 : 1
                border.color: "#888"
                radius: 4
                gradient: Gradient {
                    GradientStop { position: 0 ; color: control.pressed ? "#ccc" : "#00000000" }
                    GradientStop { position: 1 ; color: control.pressed ? "#aaa" : "#00000000" }
                }
            }
        }*/
    Image {
               source: iconSource
               anchors.verticalCenter: parent.verticalCenter
               fillMode: Image.Stretch //mm Image should shrink if button is too small, depends on QTBUG-14957
               anchors.fill: parent
               anchors.leftMargin: 8
               anchors.rightMargin: anchors.leftMargin
               anchors.topMargin: anchors.leftMargin
               anchors.bottomMargin: anchors.leftMargin
    }
}
