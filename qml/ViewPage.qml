import QtQuick 2.0
import Hal.Controls 1.0 as H
import QtQuick.Controls 1.0
import Hal 1.0 as HAL

Item  {
    id: item1
    width: 500
    height: 500

        Column {
            anchors.rightMargin: 10
            anchors.leftMargin: 10
            anchors.bottomMargin: 10
            anchors.topMargin: 10
            anchors.fill: parent
            spacing: 0

            GroupBox {
                height: parent.height*0.3
                anchors.right: parent.right
                anchors.rightMargin: 0
                anchors.left: parent.left
                anchors.leftMargin: 0
                title: qsTr("feedback")

                Column {
                    anchors.rightMargin: 10
                    anchors.leftMargin: 10
                    anchors.bottomMargin: 10
                    anchors.topMargin: 10
                    anchors.fill: parent
                    spacing: 0

                    GroupBox {
                        height: parent.height*0.33
                        anchors.right: parent.right
                        anchors.rightMargin: 0
                        anchors.left: parent.left
                        anchors.leftMargin: 0
                        title: qsTr("position")

                        H.ProgressBar {
                            name: "fb_pos"

                            anchors.fill: parent
                            minimumValue: 0
                            maximumValue: 100
                        }
                    }
                    GroupBox {
                        height: parent.height*0.33
                        anchors.right: parent.right
                        anchors.rightMargin: 0
                        anchors.left: parent.left
                        anchors.leftMargin: 0
                        title: qsTr("velocity")

                        H.Gauge {
                            name: "fb_vel"

                            anchors.fill: parent
                            minimumValue: -100
                            maximumValue: 100
                        }
                    }
                    GroupBox {
                        height: parent.height*0.33
                        anchors.right: parent.right
                        anchors.rightMargin: 0
                        anchors.left: parent.left
                        anchors.leftMargin: 0
                        title: qsTr("acceleration")

                        H.Gauge {
                            name: "fb_acc"

                            anchors.fill: parent
                            minimumValue: -100
                            maximumValue: 100
                        }
                    }
                }
            }

            GroupBox {
                height: parent.height*0.5
                anchors.right: parent.right
                anchors.rightMargin: 0
                anchors.left: parent.left
                anchors.leftMargin: 0
                title: qsTr("control")

                Column {
                    anchors.rightMargin: 10
                    anchors.leftMargin: 10
                    anchors.bottomMargin: 10
                    anchors.topMargin: 10
                    anchors.fill: parent
                    spacing: 0

                    GroupBox {
                        height: parent.height*0.25
                        anchors.right: parent.right
                        anchors.rightMargin: 0
                        anchors.left: parent.left
                        anchors.leftMargin: 0
                        title: qsTr("commanded position")

                        H.Slider {
                            name: "cmd_pos"

                            anchors.fill: parent
                            minimumValue: 0
                            maximumValue: 100
                        }
                    }
                    GroupBox {
                        height: parent.height*0.25
                        anchors.right: parent.right
                        anchors.rightMargin: 0
                        anchors.left: parent.left
                        anchors.leftMargin: 0
                        title: qsTr("maximum velocity")

                        H.Slider {
                            name: "cmd_maxvel"

                            anchors.fill: parent
                            minimumValue: 0
                            maximumValue: 100
                        }
                    }
                    GroupBox {
                        height: parent.height*0.25
                        anchors.right: parent.right
                        anchors.rightMargin: 0
                        anchors.left: parent.left
                        anchors.leftMargin: 0
                        title: qsTr("maximum acceleration")

                        H.Slider {
                            name: "cmd_maxacc"

                            anchors.fill: parent
                            minimumValue: 0
                            maximumValue: 100
                        }
                    }
                    GroupBox {
                        height: parent.height*0.25
                        anchors.right: parent.right
                        anchors.rightMargin: 0
                        anchors.left: parent.left
                        anchors.leftMargin: 0
                        title: qsTr("lowpass filter gain")

                        H.Slider {
                            name: "lowpass_gain"

                            anchors.fill: parent
                            minimumValue: 0
                            maximumValue: 1
                        }
                    }
                }
            }
            GroupBox {
                height: parent.height*0.2
                anchors.right: parent.right
                anchors.rightMargin: 0
                anchors.left: parent.left
                anchors.leftMargin: 0
                title: qsTr("misc")

                Row {
                    id: row1
                    anchors.rightMargin: 10
                    anchors.leftMargin: 10
                    anchors.bottomMargin: 10
                    anchors.topMargin: 10
                    anchors.fill: parent
                    spacing: 0

                    Column {
                        width: parent.width * 0.5
                        anchors.top: parent.top
                        anchors.topMargin: 10
                        anchors.bottom: parent.bottom
                        anchors.bottomMargin: 10
                        spacing: 0

                        H.ToggleButton {
                            name: "togglebutton1"

                            height: parent.height*0.5
                            anchors.right: parent.right
                            anchors.rightMargin: 0
                            anchors.left: parent.left
                            anchors.leftMargin: 0
                            text: qsTr("togglebutton")

                            HAL.Pin {
                                name: "togglebutton1-not"
                                type: HAL.Pin.HAL_BIT
                                direction: HAL.Pin.HAL_OUT
                                value: !parent.value
                            }
                        }
                        H.ToggleButton {
                            name: "scope_trigger"

                            height: parent.height*0.5
                            anchors.right: parent.right
                            anchors.rightMargin: 0
                            anchors.left: parent.left
                            anchors.leftMargin: 0
                            text: qsTr("trigger scope")
                        }
                    }
                    Column {
                        width: parent.width * 0.5
                        anchors.top: parent.top
                        anchors.topMargin: 10
                        anchors.bottom: parent.bottom
                        anchors.bottomMargin: 10
                        spacing: 0

                        H.Led {
                            name: "led1"

                            x: parent.width * 0.4
                            y: parent.height * 0.5
                            width: parent.width * 0.15
                            height: width
                            blink: false
                        }
                    }
                }
            }
        }

}
