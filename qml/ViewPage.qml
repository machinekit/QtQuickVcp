import QtQuick 2.0
import Hal.Controls 1.0

Item  {
    width: 500
    height: 500

        Button {
            id: myButton
            name: "button"

            anchors.bottom: parent.bottom
            anchors.left: parent.left
            text: qsTr("Button")
        }

        Led {
            id: myRadio
            name: "led"

            anchors.bottom: parent.bottom
            anchors.right: parent.right
            blink: true
            //text: qsTr("RadioButton")
        }

        VirtualJoystick {
            id: virtualJoyStick
            nameX: "xVelocity"
            nameY: "yVelocity"

            anchors.top: parent.top
            anchors.right: parent.right

            width: parent.height* 0.6
            height: width

            //autoCenter: false
        }

        Slider {
            id: slider
            name: "xVelocity"

            width: virtualJoyStick.width
            minimumValue: -100
            maximumValue: 100
            anchors.top: virtualJoyStick.bottom
            anchors.right: parent.right
        }

        ProgressBar {
            id: test
            name: "velocityOut"

            width: virtualJoyStick.width
            minimumValue: -100
            maximumValue: 100
            anchors.top: slider.bottom
            anchors.right: parent.right
        }

}
