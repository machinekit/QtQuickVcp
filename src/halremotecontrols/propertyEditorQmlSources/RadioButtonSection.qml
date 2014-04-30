import QtQuick 2.1
import HelperWidgets 2.0
import QtQuick.Layouts 1.0

Section {
    anchors.left: parent.left
    anchors.right: parent.right
    caption: qsTr("Radio Button")

    SectionLayout {

        Label {
            text: qsTr("Text")
            toolTip: qsTr("The text label for the radio button")
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
            toolTip: qsTr("Determines whether the radio button is checked or not.")
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
            toolTip: "Determines whether the radio button gets focus if pressed."
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
 
