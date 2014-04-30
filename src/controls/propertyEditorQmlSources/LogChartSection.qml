import QtQuick 2.1
import HelperWidgets 2.0
import QtQuick.Layouts 1.0

Column {
    anchors.left: parent.left
    anchors.right: parent.right

    Section {
        anchors.left: parent.left
        anchors.right: parent.right
        caption: qsTr("LogChart")

        SectionLayout {
            Label {
                text: qsTr("Value")
            }

            SecondColumnLayout {
                SpinBox {
                    backendValue: backendValues.value
                    minimumValue: -999999999999
                    maximumValue: 999999999999
                    decimals: 2
                }
                ExpandingSpacer {
                }
            }

            Label {
                text: qsTr("Target value")
            }

            SecondColumnLayout {
                SpinBox {
                    backendValue: backendValues.targetValue
                    minimumValue: -999999999999
                    maximumValue: 999999999999
                    decimals: 2
                }
                ExpandingSpacer {
                }
            }

            Label {
                text: qsTr("Max log size")
            }

            SecondColumnLayout {
                SpinBox {
                    backendValue: backendValues.maximumLogSize
                    minimumValue: 1
                    maximumValue: 999999999999
                    decimals: 0
                }
                ExpandingSpacer {
                }
            }

            Label {
                text: qsTr("Sample interval")
            }

            SecondColumnLayout {
                SpinBox {
                    backendValue: backendValues.sampleInterval
                    minimumValue: 1
                    maximumValue: 999999999999
                    decimals: 0
                }
                ExpandingSpacer {
                }
            }

            Label {
                text: qsTr("Update interval")
            }

            SecondColumnLayout {
                SpinBox {
                    backendValue: backendValues.sampleInterval
                    minimumValue: 1
                    maximumValue: 999999999999
                    decimals: 0
                }
                ExpandingSpacer {
                }
            }

            Label {
                text: qsTr("Auto sampling")
            }

            SecondColumnLayout {
                CheckBox {
                    backendValue: backendValues.autoSampling
                    implicitWidth: 180
                }
                ExpandingSpacer {

                }
            }

            Label {
                text: qsTr("Auto update")
            }

            SecondColumnLayout {
                CheckBox {
                    backendValue: backendValues.autoUpdate
                    implicitWidth: 180
                }
                ExpandingSpacer {

                }
            }
        }
    }

    ValueChartSection {

    }
}
