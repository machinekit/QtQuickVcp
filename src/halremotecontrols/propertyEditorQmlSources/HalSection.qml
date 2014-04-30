import QtQuick 2.1
import HelperWidgets 2.0
import QtQuick.Layouts 1.0

Section {
    anchors.left: parent.left
    anchors.right: parent.right
    caption: qsTr("Hal")

    SectionLayout {

        Label {
            text: qsTr("Name")
            toolTip: qsTr("Name of the main HAL pin")
        }

        SecondColumnLayout {
            LineEdit {
                backendValue: backendValues.name
                implicitWidth: 180
            }
            ExpandingSpacer {

            }
        }

        /*Label {
            text: qsTr("Type")
        }

        SecondColumnLayout {
            ComboBox {
                model: ["HAL_FLOAT", "HAL_S32", "HAL_U32", "HAL_BIT"]
                backendValue: backendValues.halPin_type
                implicitWidth: 180
                Layout.fillWidth: true
                scope: "Pin"
            }

            ExpandingSpacer {

            }
        }

        Label {
            text: qsTr("Direction")
        }

        SecondColumnLayout {
            ComboBox {
                model: ["HAL_IN", "HAL_OUT", "HAL_IO"]
                backendValue: backendValues.halPin_direction
                implicitWidth: 180
                Layout.fillWidth: true
                scope: "Pin"
            }

            ExpandingSpacer {

            }
        }*/

    }
}
