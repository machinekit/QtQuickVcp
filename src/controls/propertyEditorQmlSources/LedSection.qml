import QtQuick 2.1
import HelperWidgets 2.0
import QtQuick.Layouts 1.0

Column {
    anchors.left: parent.left
    anchors.right: parent.right

    Section {
        anchors.left: parent.left
        anchors.right: parent.right
        caption: qsTr("Led")

        SectionLayout {
            Label {
                text: qsTr("Value")
            }

            SecondColumnLayout {
                CheckBox {
                    backendValue: backendValues.value
                    implicitWidth: 180
                }
                ExpandingSpacer {

                }
            }

            Label {
                text: qsTr("Blink")
            }

            SecondColumnLayout {
                CheckBox {
                    backendValue: backendValues.blink
                    implicitWidth: 180
                }
                ExpandingSpacer {

                }
            }

            Label {
                text: qsTr("Blink interval")
            }

            SecondColumnLayout {
                SpinBox {
                    backendValue: backendValues.blinkInterval
                    minimumValue: 1
                    maximumValue: 99999999999
                    decimals: 0
                }
                ExpandingSpacer {
                }
            }

            Label {
                text: qsTr("Radius")
            }

            SecondColumnLayout {
                SpinBox {
                    backendValue: backendValues.radius
                    minimumValue: 0
                    maximumValue: 99999999999
                    decimals: 0
                }
                ExpandingSpacer {
                }
            }

            Label {
                text: qsTr("Shine")
            }

            SecondColumnLayout {
                CheckBox {
                    backendValue: backendValues.shine
                    implicitWidth: 180
                }
                ExpandingSpacer {

                }
            }
        }
    }

    Section {
            anchors.left: parent.left
            anchors.right: parent.right
            caption: qsTr("On color")

            ColorEditor {
                        caption: qsTr("On color")
                        backendendValue: backendValues.onColor
                        supportGradient: false
                    }
    }

    Section {
            anchors.left: parent.left
            anchors.right: parent.right
            caption: qsTr("Off color")

            ColorEditor {
                        caption: qsTr("Off color")
                        backendendValue: backendValues.offColor
                        supportGradient: false
                    }
    }
}
