import QtQuick 2.1
import HelperWidgets 2.0
import QtQuick.Layouts 1.0

Section {
    anchors.left: parent.left
    anchors.right: parent.right
    caption: qsTr("ColorPicker")

    SectionLayout {
        Label {
            text: qsTr("Alpha visible")
        }

        SecondColumnLayout {
            CheckBox {
                backendValue: backendValues.alphaVisible
                implicitWidth: 180
            }
            ExpandingSpacer {

            }
        }
    }
}

