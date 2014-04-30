import QtQuick 2.1
import HelperWidgets 2.0
import QtQuick.Layouts 1.0

Section {
        anchors.left: parent.left
        anchors.right: parent.right
        caption: qsTr("Button")

        SectionLayout {

            Label {
                text: qsTr("Text")
                toolTip:  qsTr("The text shown on the button")
            }

            SecondColumnLayout {
                LineEdit {
                    backendValue: backendValues.text
                    implicitWidth: 180
                }
                ExpandingSpacer {

                }
            }

            Label {
                text: qsTr("Checked")
                toolTip: qsTr("The state of the button")
            }

            SecondColumnLayout {
                CheckBox {
                    backendValue: backendValues.checked
                    implicitWidth: 180
                }
                ExpandingSpacer {

                }
            }

            Label {
                text: qsTr("Checkable")
                toolTip: qsTr("Determines whether the button is checkable or not.")
            }

            SecondColumnLayout {
                CheckBox {
                    backendValue: backendValues.checkable
                    implicitWidth: 180
                }
                ExpandingSpacer {

                }
            }

            Label {
                text: qsTr("Enabled")
                toolTip: qsTr("Determines whether the button is enabled or not.")
            }

            SecondColumnLayout {
                CheckBox {
                    backendValue: backendValues.enabled
                    implicitWidth: 180
                }
                ExpandingSpacer {

                }
            }


            Label {
                text: qsTr("Default button")
                toolTip: qsTr("Sets the button as the default button in a dialog.")
            }

            SecondColumnLayout {
                CheckBox {
                    backendValue: backendValues.defaultbutton
                    implicitWidth: 180
                }
                ExpandingSpacer {

                }
            }

            Label {
                text: qsTr("Tool tip")
                toolTip: qsTr("The tool tip shown for the button.")
            }

            SecondColumnLayout {
                LineEdit {
                    backendValue: backendValues.toolTip
                    implicitWidth: 180
                }
                ExpandingSpacer {

                }
            }

            Label {
                text: qsTr("Focus on press")
                toolTip: "Determines whether the button gets focus if pressed."
            }

            SecondColumnLayout {
                CheckBox {
                    backendValue: backendValues.activeFocusOnPress
                    implicitWidth: 180
                }
                ExpandingSpacer {

                }
            }


            Label {
                text: qsTr("Icon source")
                toolTip: qsTr("The URL of an icon resource.")
            }

            SecondColumnLayout {
                LineEdit {
                    backendValue: backendValues.iconSource
                    implicitWidth: 180
                }
                ExpandingSpacer {

                }
            }


        }
    }