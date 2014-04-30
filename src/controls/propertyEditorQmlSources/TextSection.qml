import QtQuick 2.1
import HelperWidgets 2.0
import QtQuick.Layouts 1.0


Section {
    anchors.left: parent.left
    anchors.right: parent.right
    caption: qsTr("Text")

    SectionLayout {
        Label {
            text: qsTr("Decimals")
        }

        SecondColumnLayout {
            SpinBox {
                backendValue: backendValues.decimals
                minimumValue: 0
                maximumValue: 10
                decimals: 0
            }
            ExpandingSpacer {
            }
        }

        Label {
            text: qsTr("Prefix")
        }

        SecondColumnLayout {
            LineEdit {
                backendValue: backendValues.prefix
                Layout.fillWidth: true
            }
            ExpandingSpacer {
            }
        }

        Label {
            text: qsTr("Suffix")
        }

        SecondColumnLayout {
            LineEdit {
                backendValue: backendValues.suffix
                Layout.fillWidth: true
            }
            ExpandingSpacer {
            }
        }

        Label {
            text: qsTr("Visibility")
            toolTip: qsTr("Determines whether the values are visible or not.")
        }

        SecondColumnLayout {
            ColumnLayout {
                CheckBox {
                    text: qsTr("Value")
                    backendValue: backendValues.valueVisible
                }
                CheckBox {
                    text: qsTr("Minimum value")
                    backendValue: backendValues.minimumValueVisible
                }
                CheckBox {
                    text: qsTr("Maximum value")
                    backendValue: backendValues.maximumValueVisible
                }
            }

            ExpandingSpacer {
            }
        }
    }
}
