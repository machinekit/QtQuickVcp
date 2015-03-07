import QtQuick 2.0
import Knobs 1.0
import QtQuick.Controls 1.2

Rectangle {
    width: 1280
    height: 460

    Column {
        anchors.left: parent.left
        anchors.leftMargin: 10
        anchors.top: parent.top
        anchors.topMargin: 10
        spacing: 30
        Row {
            spacing: 10
            Knob {
                id: kn
                size: 200
                style: Knob.Pie
                pieType: Knob.Flat
                value: 75
            }

            Knob {
                size: 200
                style: Knob.Arc
                value: 25
                color: "#EF6555"
            }

            Knob {
                size: 200
                style: Knob.Needle
                needleType: Knob.Point
                value: 50
                color: "#FEC56B"
            }

            Knob {
                size: 200
                style: Knob.Pie
                pieType: Knob.Curve
                value: 65
                color: "#C55186"
            }

            Knob {
                size: 200
                style: Knob.Needle
                needleType: Knob.Groove
                value: 35
            }

            Knob {
                size: 200
                style: Knob.Needle
                needleType: Knob.Round
                value: 88
                color: "#96040F"
            }
        }

        Row {
            spacing: 10

            Knob {
                size: 200
                style: Knob.Arc
                percent: 50
                mode: Knob.Percent
                color: "#EF6555"
            }

            Knob {
                size: 200
                style: Knob.Pie
                pieType: Knob.Curve
                percent: 90
                mode: Knob.Percent
                color: "#C55186"
            }

            Knob {
                size: 200
                style: Knob.Arc
                value: 2500
                maximumValue: 5000
            }

            Knob {
                size: 200
                style: Knob.Pie
                value: 8563
                maximumValue: 10000
                color: "#EF6555"
            }

            Item {
                width: 200
                height: 200
                Knob {
                    size: 100
                    style: Knob.Pie
                    pieType: Knob.Curve
                    value: 135
                    maximumValue: 200
                    color: "#D7AAFB"
                }
            }

            Knob {
                size: 200
                style: Knob.Pie
                pieType: Knob.Curve
                value: 25
                maximumValue: 100
                color: "#FF3300"
                backgroundColor: "black"
                foregroundColor: "#00FF99"
                borderColor: "#003399"
                textColor: "#8D18D7"
            }
        }
    }
}

