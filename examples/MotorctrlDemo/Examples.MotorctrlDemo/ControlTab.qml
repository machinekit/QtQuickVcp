import QtQuick 2.0
import QtQuick.Controls 1.1
import QtQuick.Layouts 1.1
import QtQuick.Window 2.0
import Machinekit.Controls 1.0
import Machinekit.HalRemote.Controls 1.0
import Machinekit.HalRemote 1.0

Tab {
    id: main
    title: "Control"
    active: true

    Item {
        anchors.fill: parent

        ColumnLayout {
            anchors.margins: Screen.logicalPixelDensity*3
            anchors.fill: parent
            spacing: 0

            RowLayout {
                Layout.fillWidth: true
            GroupBox {
                Layout.fillWidth: true
                Layout.preferredHeight: main.height * 0.4
                title: qsTr("Feedback")

                RowLayout {
                    anchors.fill: parent
                    spacing: 5

                    ColumnLayout {
                        Layout.fillWidth: true
                        HalGauge {
                            id: positionProgressBar
                            Layout.fillWidth: true
                            name: "fb_pos"
                            minimumValue: 0
                            maximumValue: 100
                            fancy: true
                            z0BorderValue: 100
                            z1BorderValue: 0
                            z0Color: "red"
                            z1Color: "red"
                        }
                        HalPin {
                            name: "fb_pos.scale"
                            type: HalPin.Float
                            direction: HalPin.In
                        }
                        LogChart {
                            Layout.fillWidth: true
                            Layout.preferredHeight: main.height * 0.2
                            width: 200
                            height: 200
                            value: positionProgressBar.value
                            minimumValue: positionProgressBar.minimumValue
                            maximumValue: positionProgressBar.maximumValue
                            timeSpan: 30000
                            xGrid: 1000
                            yGrid: 10
                            leftTextVisible: false
                            rightTextVisible: false
                            maximumLogSize: 650
                            sampleInterval: 50
                            updateInterval: 100
                            autoSampling: logSwitch.checked
                            autoUpdate: logSwitch.checked
                        }
                        Label {
                            Layout.fillWidth: true
                            text: qsTr("Position")
                            horizontalAlignment: Text.AlignHCenter

                            Switch {
                                id: logSwitch
                                anchors.left: parent.left
                                anchors.top: parent.top
                            }
                        }
                    }
                    ColumnLayout {
                        HalRoundGauge {
                            Layout.alignment: Layout.Center
                            height: width
                            name: "fb_vel"
                            minimumValue: -100
                            maximumValue: 100
                            zeroValue: 0
                            fancy: false
                        }
                        Label {
                            Layout.fillWidth: true
                            text: qsTr("Velocity")
                            horizontalAlignment: Text.AlignHCenter
                        }
                        HalPin {
                            name: "fb_vel.scale"
                            type: HalPin.Float
                            direction: HalPin.In
                        }
                    }
                    ColumnLayout {
                        HalRoundGauge {
                            Layout.alignment: Layout.Center
                            height: width
                            name: "fb_acc"
                            minimumValue: -100
                            maximumValue: 100
                            zeroValue: 0
                            fancy: false
                        }
                        Label {
                            Layout.fillWidth: true
                            text: qsTr("Acceleration")
                            horizontalAlignment: Text.AlignHCenter
                        }
                        HalPin {
                            name: "fb_acc.scale"
                            type: HalPin.Float
                            direction: HalPin.In
                        }
                    }

                }
            }
            GroupBox {
                Layout.fillWidth: false
                Layout.preferredHeight: main.height * 0.4
                title: qsTr("Misc")

                RowLayout {
                    id: row1
                    anchors.fill: parent
                    spacing: 5

                    ColumnLayout {
                        Layout.fillHeight: true
                        Layout.fillWidth: true
                        spacing: 5

                        HalButton {
                            name: "togglebutton1"

                            Layout.fillHeight: true
                            Layout.fillWidth: true
                            text: qsTr("togglebutton")
                            checkable: true

                            HalPin {
                                name: "togglebutton1-not"
                                type: HalPin.Bit
                                direction: HalPin.Out
                                value: !parent.value
                            }
                        }
                        HalButton {
                            name: "scope_trigger"

                            Layout.fillHeight: true
                            Layout.fillWidth: true
                            text: qsTr("trigger scope")
                            checkable: true
                        }
                    }

                    ColumnLayout {
                        Layout.fillHeight: true
                        Layout.fillWidth: true
                        spacing: 5

                        HalLed {
                            name: "led1"
                            Layout.preferredHeight: main.height * 0.08
                            Layout.preferredWidth: main.height * 0.08
                            blink: false
                        }
                    }
                }
            }

            }

            GroupBox {
                Layout.fillWidth: true
                title: qsTr("Control")

                ColumnLayout {
                    anchors.fill: parent
                    spacing: 5

                    Label {
                        Layout.fillWidth: true
                        text: qsTr("Commanded position")
                    }
                    RowLayout {
                        Layout.fillWidth: true
                        HalSlider {
                            id: cmdPosSlider
                            Layout.fillWidth: true
                            name: "cmd_pos"
                            minimumValue: 0
                            maximumValue: 100
                            value: 0
                            tickmarksEnabled: false
                            minimumValueVisible: false
                            maximumValueVisible: false
                            valueVisible: true
                        }
                    }
                    RowLayout {
                        Layout.fillWidth: true
                        Item {
                            Layout.fillWidth: true
                        }
                        ColumnLayout {
                            Layout.fillHeight: true
                            HalSlider {
                                id: cmdMaxVelSlider
                                Layout.alignment: Layout.Center
                                Layout.preferredWidth: main.width * 0.25
                                name: "cmd_maxvel"
                                minimumValue: 0
                                maximumValue: 100
                                value: 20
                                tickmarksEnabled: false
                            }
                            Label {
                                Layout.fillWidth: true
                                text: qsTr("Max velocity")
                                horizontalAlignment: Text.AlignHCenter
                            }
                        }
                        ColumnLayout {
                            Layout.fillHeight: true
                            HalSlider {
                                id: cmdMaxAccSlider
                                Layout.alignment: Layout.Center
                                Layout.preferredWidth: main.width * 0.25
                                name: "cmd_maxacc"
                                minimumValue: 0
                                maximumValue: 100
                                value: 10
                                tickmarksEnabled: false
                            }
                            Label {
                                Layout.fillWidth: true
                                text: qsTr("Max acceleration")
                                horizontalAlignment: Text.AlignHCenter
                            }
                        }
                        ColumnLayout {
                            Layout.fillHeight: true
                            HalSlider {
                                id: lowpassgainSlider
                                Layout.alignment: Layout.Center
                                Layout.preferredWidth: main.width * 0.25
                                name: "lowpass_gain"
                                minimumValue: 0
                                maximumValue: 0.9
                                value: 0.01
                                decimals: 2
                                tickmarksEnabled: false
                            }
                            Label {
                                Layout.fillWidth: true
                                text: qsTr("Lowpass filter gain")
                                horizontalAlignment: Text.AlignHCenter
                            }
                        }
                        Item {
                            Layout.fillWidth: true
                        }
                    }
                    Item {
                        Layout.fillHeight: true
                    }
                }
            }
            }
    }
}

