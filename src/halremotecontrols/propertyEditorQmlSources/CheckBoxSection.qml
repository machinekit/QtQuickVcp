import QtQuick 2.1
import HelperWidgets 2.0
import QtQuick.Layouts 1.0

Section {
    anchors.left: parent.left
    anchors.right: parent.right
    caption: qsTr("Check Box")

    SectionLayout {

        Label {
            text: qsTr("Text")
            toolTip:  qsTr("The text shown on the check box")
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
            toolTip: qsTr("The state of the check box")
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
            text: qsTr("Focus on press")
            toolTip: qsTr("Determines whether the check box gets focus if pressed.")
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
