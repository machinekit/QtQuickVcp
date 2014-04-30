import QtQuick 2.1
import HelperWidgets 2.0
import QtQuick.Layouts 1.0

Column {
    anchors.left: parent.left
    anchors.right: parent.right

    Section {
        anchors.left: parent.left
        anchors.right: parent.right
        caption: qsTr("Gauge")

        SectionLayout {
            Label {
                text: qsTr("Value")
            }

            SecondColumnLayout {
                SpinBox {
                    backendValue: backendValues.value
                    minimumValue: backendValues.minimumValue
                    maximumValue: backendValues.maximumValue
                    decimals: 2
                }
                ExpandingSpacer {
                }
            }

            Label {
                text: qsTr("Minimum value")
            }

            SecondColumnLayout {
                SpinBox {
                    backendValue: backendValues.minimumValue
                    minimumValue: -999999999999
                    maximumValue: backendValues.maximumValue
                    decimals: 2
                }
                ExpandingSpacer {
                }
            }

            Label {
                text: qsTr("Maximum value")
            }

            SecondColumnLayout {
                SpinBox {
                    backendValue: backendValues.maximumValue
                    minimumValue: backendValues.minimumValue
                    maximumValue: 999999999999
                    decimals: 2
                }
                ExpandingSpacer {
                }
            }

            Label {
                text: qsTr("Zero value")
            }

            SecondColumnLayout {
                SpinBox {
                    backendValue: backendValues.zeroValue
                    minimumValue: backendValues.minimumValue
                    maximumValue: backendValues.maximumValue
                    decimals: 2
                }
                ExpandingSpacer {
                }
            }

            Label {
                text: qsTr("z0 border value")
            }

            SecondColumnLayout {
                SpinBox {
                    backendValue: backendValues.z0BorderValue
                    minimumValue: backendValues.minimumValue
                    maximumValue: backendValues.maximumValue
                    decimals: 2
                }
                ExpandingSpacer {
                }
            }

            Label {
                text: qsTr("z1 border value")
            }

            SecondColumnLayout {
                SpinBox {
                    backendValue: backendValues.z1BorderValue
                    minimumValue: backendValues.minimumValue
                    maximumValue: backendValues.maximumValue
                    decimals: 2
                }
                ExpandingSpacer {
                }
            }

            Label {
                text: qsTr("Invert")
                toolTip: qsTr("Determines whether the gauge is inverted or not.")
            }

            SecondColumnLayout {
                CheckBox {
                    backendValue: backendValues.invert
                    implicitWidth: 180
                }
                ExpandingSpacer {

                }
            }

            Label {
                text: qsTr("Fancy")
                toolTip: qsTr("Determines whether the bar should be styled fancy or not.")
            }

            SecondColumnLayout {
                CheckBox {
                    backendValue: backendValues.fancy
                    implicitWidth: 180
                }
                ExpandingSpacer {

                }
            }

            Label {
                text: qsTr("Orientation")
            }

            SecondColumnLayout {
                SpinBox {
                    backendValue: backendValues.orientation
                    minimumValue: 0
                    maximumValue: 1
                    decimals: 0
                }
                ExpandingSpacer {
                }
            }

        }
    }

    TextSection {
    }

    Section {
            anchors.left: parent.left
            anchors.right: parent.right
            caption: qsTr("Text color")

            ColorEditor {
                        caption: qsTr("Text color")
                        backendendValue: backendValues.textColor
                        supportGradient: false
                    }
    }

    Section {
            anchors.left: parent.left
            anchors.right: parent.right
            caption: qsTr("Background color")

            ColorEditor {
                        caption: qsTr("Background color")
                        backendendValue: backendValues.backgroundColor
                        supportGradient: false
                    }
    }

    Section {
            anchors.left: parent.left
            anchors.right: parent.right
            caption: qsTr("z0 color")

            ColorEditor {
                        caption: qsTr("z0 color")
                        backendendValue: backendValues.z0Color
                        supportGradient: false
                    }
    }

    Section {
            anchors.left: parent.left
            anchors.right: parent.right
            caption: qsTr("z1 color")

            ColorEditor {
                        caption: qsTr("z1 color")
                        backendendValue: backendValues.z1Color
                        supportGradient: false
                    }
    }

    Section {
            anchors.left: parent.left
            anchors.right: parent.right
            caption: qsTr("z2 color")

            ColorEditor {
                        caption: qsTr("z2 color")
                        backendendValue: backendValues.z2Color
                        supportGradient: false
                    }
    }
}
