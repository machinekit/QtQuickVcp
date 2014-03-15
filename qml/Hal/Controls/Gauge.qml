import QtQuick 2.0
import QtQuick.Controls 1.1
import QtQuick.Controls.Styles 1.1
import Hal 1.0 as HAL

ProgressBar {
    property string name: "gauge"
    property double minimumValue: -pinScale.value   // can be overwritten
    property double maximumValue: pinScale.value    // can be overwritten
    property bool showText: true
    property double value: 0
    property bool hovered: progressBar1.hovered || progressBar2.hovered

    id: main

    width: 100
    height: 62

    style: ProgressBarStyle {
        progress: Item {
        }
    }

    ProgressBar {
        id: progressBar1

        anchors.top: parent.top
        anchors.bottom: parent.bottom
        anchors.left: parent.left
        width: parent.width * 0.5

        minimumValue: 0
        maximumValue: -main.minimumValue
        rotation: 180
        value: (main.value < 0)?-main.value:0

        style: ProgressBarStyle {
            background: Item {
            }
        }
    }

    ProgressBar {
        id: progressBar2

        anchors.top: parent.top
        anchors.bottom: parent.bottom
        anchors.left: progressBar1.right
        anchors.right: parent.right

        minimumValue: 0
        maximumValue: main.maximumValue
        value: (main.value > 0)?main.value:0

        style: ProgressBarStyle {
            background: Item {
            }
        }
    }

    Text {
        id: progressText1

        anchors.centerIn: parent
        text: main.value.toFixed(2)
        visible: main.showText
    }

    HAL.Pin {
        id: pin

        name: main.name
        type: HAL.Pin.HAL_FLOAT
        direction: HAL.Pin.HAL_IN
    }

    HAL.Pin {
        id: pinScale

        name: main.name + ".scale"
        type: HAL.Pin.HAL_FLOAT
        direction: HAL.Pin.HAL_IN
    }

    Binding { target: main; property: "value"; value: pin.value}
}
