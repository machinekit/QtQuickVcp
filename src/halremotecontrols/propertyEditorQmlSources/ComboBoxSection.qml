import QtQuick 2.1
import HelperWidgets 2.0
import QtQuick.Layouts 1.0

Column {
    anchors.left: parent.left
    anchors.right: parent.right

    Section {
        anchors.left: parent.left
        anchors.right: parent.right
        caption: qsTr("Combo Box")

        SectionLayout {

            Label {
                text: qsTr("Tool tip")
                toolTip: qsTr("The tool tip shown for the combobox.")
            }

            SecondColumnLayout {
                LineEdit {
                    backendValue: backendValues.tooltip
                    implicitWidth: 180
                }
                ExpandingSpacer {

                }
            }

            Label {
                text: qsTr("Focus on press")
                toolTip: "Determines whether the combobox gets focus if pressed."
            }

            SecondColumnLayout {
                CheckBox {
                    backendValue: backendValues.activeFocusOnPress
                    implicitWidth: 180
                }
                ExpandingSpacer {

                }
            }

        }
    }
}