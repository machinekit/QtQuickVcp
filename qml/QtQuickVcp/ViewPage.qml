import QtQuick 2.0
import QtQuick.Controls 1.1
import Hal 1.0 as HAL
import "components"

Item  {
    width: 100
    height: 62



        Button {
            id: myButton

            anchors.bottom: parent.bottom
            anchors.left: parent.left
            text: qsTr("Button")
            checkable: true

            HAL.Pin {
                id: myPin

                name: "button"
                type: HAL.Pin.HAL_BIT
                direction: HAL.Pin.HAL_OUT
                value: myButton.checked
            }
        }

        RadioButton {
            id: myRadio

            anchors.bottom: parent.bottom
            anchors.right: parent.right
            text: qsTr("RadioButton")

            HAL.Pin {
                id: myRadioPin

                name: "led"
                type: HAL.Pin.HAL_BIT
                direction: HAL.Pin.HAL_IN
            }

            checked: myRadioPin.value
        }

        VirtualJoystick {
            id: virtualJoyStick

            anchors.top: parent.top
            anchors.right: parent.right

            width: parent.height* 0.6
            height: width
            //autoCenter: false

            HAL.Pin {
                id: xVelocityPin

                name: "xVelocity"
                type: HAL.Pin.HAL_FLOAT
                direction: HAL.Pin.HAL_OUT
                value: virtualJoyStick.xVelocity
            }

            HAL.Pin {
                id: yVelocityPin

                name: "yVelocity"
                type: HAL.Pin.HAL_FLOAT
                direction: HAL.Pin.HAL_OUT
                value: virtualJoyStick.yVelocity
            }

        }

        Slider {
            id: test

            width: virtualJoyStick.width
            minimumValue: -100
            maximumValue: 100
            anchors.top: virtualJoyStick.bottom
            anchors.right: parent.right

            value: velocityOut.value

            HAL.Pin {
                id: velocityOut

                name: "velocityOut"
                type: HAL.Pin.HAL_FLOAT
                direction: HAL.Pin.HAL_IN
            }

            //Binding { target: virtualJoyStick; property: "xVelocity"; value: test.value}
            //Binding { target: test; property: "value"; value: virtualJoyStick.xVelocity}
        }
}
