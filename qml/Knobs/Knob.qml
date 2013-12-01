import QtQuick 2.0
import QtGraphicalEffects 1.0
import Meter 1.0

Item {
    id: knob
    property alias dialSize: knob.width

    property color chunkColor: "skyblue"
    property color dialColor: "#EEEEEE"
    property color dialTextColor: "red"
    property int dialEndValue: 25
    property int chunkStyle: 0
    property int dialPointStyle: 1
    property bool chunkMultiColor: false
    property bool meter: false

    property int currentEndValue

    width: 200
    height: width

    Rectangle
    {
        id: lowerborder
        anchors.centerIn: parent
        width: parent.width
        height: width
        radius: width/2
        antialiasing: true
        color: "lightgray"
    }

    Rectangle
    {
        id: lower
        anchors.centerIn: parent
        width: parent.width-2
        height: width
        radius: width/2
        antialiasing: true
        color: dialColor
    }

    Loader {
        id: meterloader
        anchors.fill: parent
        asynchronous: true
    }

    Component {
        id: componentmeter
        Meter {
        }
    }

    Loader {
        id: loader
        anchors.fill: parent
        asynchronous: true
    }

    Binding {
        target: knob
        property: "currentEndValue"
        value: loader.item.endValue
        when: loader.status === Loader.Ready
    }

    Rectangle
    {
        id: upperborder
        anchors.centerIn: parent
        width: dialPointStyle < 2 ? parent.width/1.5 : parent.width/1.2
        antialiasing: true
        height: width
        radius: width/2

        LinearGradient {
            source: upperborder
            anchors.fill: upperborder
            gradient: Gradient {
                GradientStop { position: 0.0; color: "transparent" }
                GradientStop { position: 1.0; color: "darkgray" }
            }
        }
    }

    Rectangle
    {
        id: upper
        anchors.centerIn: parent
        width: upperborder.width-2
        height: width
        radius: width/2

        Text {
            id: indicator
            anchors.centerIn: parent
            color: dialTextColor
            font.pointSize: 16
        }
    }

    Component.onCompleted: {
        switch(chunkStyle)
        {
        case 0: loader.source = "Pie.qml"; break;
        case 1: loader.source = "Arc.qml"; break;
        case 2: loader.source = "Needle.qml"; break;
        }

        if(meter)
            meterloader.sourceComponent = componentmeter
    }
}
