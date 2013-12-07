import QtQuick 2.0
import NeedleStyle 1.0

Rectangle {
    width: 900
    height: 600

    Knob {
        id: a
        anchors.left: parent.left
        chunkStyle: 1
        dialEndValue: 75
        chunkColor: "#E89C84"
        dialSize: 200
    }

    Text {
        id: ta
        anchors.top: a.bottom
        anchors.horizontalCenter: a.horizontalCenter
        anchors.topMargin: 10
        text : a.currentEndValue
        font.bold: true
        color: "blue"
    }

    Knob {
        id: b
        anchors.left: a.right
        chunkStyle: 0
        dialEndValue: 50
        dialSize: 200

        chunkMultiColor: false
    }

    Text {
        id: tb
        anchors.top: b.bottom
        anchors.horizontalCenter: b.horizontalCenter
        anchors.topMargin: 10
        text : b.currentEndValue
        font.bold: true
        color: "green"
    }

    Knob {
        id: c
        anchors.top: ta.bottom
        anchors.topMargin: 10
        anchors.horizontalCenter: ta.horizontalCenter
        chunkStyle: 2
        dialEndValue: 80
        dialSize: 200
        dialPointStyle: NeedleStyle.Point
    }

    Text {
        id: tc
        anchors.top: c.bottom
        anchors.horizontalCenter: c.horizontalCenter
        anchors.topMargin: 10
        text : c.currentEndValue
        font.bold: true
        color: "red"
    }



    Knob {
        id: d
        anchors.left: c.right
        anchors.top: tb.bottom
        anchors.topMargin: 10
        chunkStyle: 0
        dialEndValue: 90
        dialSize: 200
        chunkMultiColor: true
    }

    Text {
        id: td
        anchors.top: d.bottom
        anchors.horizontalCenter: d.horizontalCenter
        anchors.topMargin: 10
        text : d.currentEndValue
        font.bold: true
        color: "green"
    }


    Knob {
        id: e
        anchors.left: b.right
        anchors.top: parent.top
        chunkStyle: 2
        dialEndValue: 65
        dialSize: 200
        dialPointStyle: NeedleStyle.Point
        meter: true
        chunkColor: "#FEC56B"
    }

    Text {
        id: te
        anchors.top: e.bottom
        anchors.horizontalCenter: e.horizontalCenter
        anchors.topMargin: 10
        text : e.currentEndValue
        font.bold: true
        color: "red"
    }


    Knob {
        id: f
        anchors.top: te.bottom
        anchors.topMargin: 10
        anchors.horizontalCenter: te.horizontalCenter
        chunkStyle: 2
        dialEndValue: 35
        dialSize: 200
        dialPointStyle: NeedleStyle.Groove
    }

    Text {
        id: tf
        anchors.top: f.bottom
        anchors.horizontalCenter: f.horizontalCenter
        anchors.topMargin: 10
        text : f.currentEndValue
        font.bold: true
        color: "red"
    }

    Knob {
        id: g
        anchors.top: parent.top
        anchors.left: e.right
        chunkStyle: 2
        dialEndValue: 35
        chunkColor: "#C5518F"
        dialSize: 200
        dialPointStyle: NeedleStyle.Round
    }

    Text {
        id: tg
        anchors.top: g.bottom
        anchors.horizontalCenter: g.horizontalCenter
        anchors.topMargin: 10
        text : g.currentEndValue
        font.bold: true
        color: "red"
    }
}
