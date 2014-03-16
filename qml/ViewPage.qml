import QtQuick 2.0
import Hal.Controls 1.0 as H
import QtQuick.Controls 1.1
import QtQuick.Layouts 1.1
import Hal 1.0 as HAL

Item  {
    id: item1
    width: 500
    height: 500

        ColumnLayout {
            anchors.margins: 10
            anchors.fill: parent
            spacing: 0

            GroupBox {
                Layout.fillWidth: true
                Layout.preferredHeight: item1.height * 0.3
                title: qsTr("feedback")

                ColumnLayout {
                    anchors.fill: parent
                    spacing: 5

                    GroupBox {
                        Layout.fillWidth: true
                        Layout.fillHeight: true
                        title: qsTr("position")

                        H.ProgressBar {
                            name: "fb_pos"

                            anchors.fill: parent
                            minimumValue: 0
                            maximumValue: 100
                        }
                    }
                    GroupBox {
                        Layout.fillWidth: true
                        Layout.fillHeight: true
                        title: qsTr("velocity")

                        H.Gauge {
                            name: "fb_vel"

                            anchors.fill: parent
                            minimumValue: -100
                            maximumValue: 100
                        }
                    }
                    GroupBox {
                        Layout.fillWidth: true
                        Layout.fillHeight: true
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
                Layout.fillWidth: true
                Layout.preferredHeight: item1.height * 0.5
                title: qsTr("control")

                ColumnLayout {
                    anchors.fill: parent
                    spacing: 5

                    GroupBox {
                        Layout.fillWidth: true
                        Layout.fillHeight: true
                        title: qsTr("commanded position")

                        H.Slider {
                            name: "cmd_pos"

                            anchors.fill: parent
                            minimumValue: 0
                            maximumValue: 100
                        }
                    }
                    GroupBox {
                        Layout.fillWidth: true
                        Layout.fillHeight: true
                        title: qsTr("maximum velocity")

                        H.Slider {
                            name: "cmd_maxvel"

                            value: 20
                            anchors.fill: parent
                            minimumValue: 0
                            maximumValue: 100
                        }
                    }
                    GroupBox {
                        Layout.fillWidth: true
                        Layout.fillHeight: true
                        title: qsTr("maximum acceleration")

                        H.Slider {
                            name: "cmd_maxacc"

                            value: 10
                            anchors.fill: parent
                            minimumValue: 0
                            maximumValue: 100
                        }
                    }
                    GroupBox {
                        Layout.fillWidth: true
                        Layout.fillHeight: true
                        title: qsTr("lowpass filter gain")

                        H.Slider {
                            name: "lowpass_gain"

                            value: 0.01
                            anchors.fill: parent
                            minimumValue: 0
                            maximumValue: 0.9
                        }
                    }
                }
            }
            GroupBox {
                Layout.fillWidth: true
                Layout.fillHeight: true
                title: qsTr("misc")

                RowLayout {
                    id: row1
                    anchors.fill: parent
                    spacing: 5

                    ColumnLayout {
                        Layout.fillHeight: true
                        Layout.fillWidth: true
                        //Layout.: 2
                        spacing: 5

                        H.ToggleButton {
                            name: "togglebutton1"

                            Layout.fillHeight: true
                            Layout.fillWidth: true
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

                            Layout.fillHeight: true
                            Layout.fillWidth: true
                            text: qsTr("trigger scope")
                        }
                    }

                    ColumnLayout {
                        Layout.preferredWidth: parent.width *0.4
                        Layout.fillHeight: true
                        spacing: 5

                        H.Led {
                            name: "led1"

                            x: parent.width * 0.4
                            y: parent.height * 0.3
                            width: parent.width * 0.15
                            height: width
                            blink: false
                        }
                    }
                }
            }
        }

}
