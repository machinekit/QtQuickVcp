import QtQuick 2.0
import QtQuick.Controls 1.1
import QtQuick.Layouts 1.1
import QtQuick.Window 2.0
import Machinekit.Application 1.0

Item {
    property var status: {"synced": false}
    property alias textColor: dummyLabel.color
    property alias font: dummyLabel.font
    property int decimals: 4
    property string prefix: ""
    property string suffix: ""
    property int axes: _ready ? status.config.axes : 4
    property var axisHomed: _ready ? status.motion.axis : [{"homed":false}, {"homed":true}, {"homed":false}, {"homed":true}]
    property var axisNames: ["X:", "Y:", "Z:", "A:", "B:", "C:", "U:", "V:", "W:"]
    property var g5xNames: ["G54", "G55", "G56", "G57", "G58", "G59", "G59.1", "G59.2", "G59.3"]
    property int g5xIndex: _ready ? status.motion.g5xIndex : 1
    property var position: getPosition()
    property var dtg: _ready ? status.motion.dtg : {"x":0.12345, "y":0.234,"z":123.12,"a":324.3}
    property var g5xOffset: _ready ? status.motion.g5xOffset : {"x":0.12345, "y":0.234,"z":123.12,"a":324.3}
    property var g92Offset: _ready ? status.motion.g92Offset : {"x":0.12345, "y":0.234,"z":123.12,"a":324.3}
    property var toolOffset: _ready ? status.io.toolOffset : {"x":0.12345, "y":0.234,"z":123.12,"a":324.3}
    property double velocity: _ready ? status.motion.currentVel : 15.4
    property bool offsetsVisible: true
    property int positionFeedback: _ready ? status.config.positionFeedback : ApplicationStatus.ActualPositionFeedback
    property int positionOffset: _ready ? status.config.positionOffset : ApplicationStatus.RelativePositionOffset

    property bool _ready: status.synced
    property var _axisNames: ["x", "y", "z", "a", "b", "c", "u", "v", "w"]

    function getPosition() {
        var basePosition
        if (_ready) {
            basePosition = (positionFeedback == ApplicationStatus.ActualPositionFeedback) ? status.motion.actualPosition : status.motion.position
        }
        else {
            basePosition = {"x":0.12345, "y":0.234,"z":123.12,"a":324.3}
        }

        if (positionOffset == ApplicationStatus.RelativePositionOffset) {
            for (var i = 0; i < axes; ++i) {
                var axisName = _axisNames[i]
                basePosition[axisName] -= g5xOffset[axisName] + g92Offset[axisName] + toolOffset[axisName]
            }
        }

        return basePosition
    }


    id: droRect
    implicitWidth: dummyLabel.font.pixelSize * 14
    implicitHeight: dummyLabel.font.pixelSize * 14

    Label {
        id: dummyLabel
        font.bold: true
        font.pointSize: 10
    }

    Component {
        id: textLine
        Row {
            property double value: 0.12345
            property string title: "X:"
            property string type: "DTG"
            property bool homed: true

            id: root

            Label {
                width: dummyLabel.font.pixelSize * 2

                color: dummyLabel.color
                font: dummyLabel.font
                text: root.type

                Loader {
                    sourceComponent: homedSymbol
                    anchors.top: parent.top
                    anchors.bottom: parent.bottom
                    anchors.horizontalCenter: parent.horizontalCenter
                    width: height
                    onLoaded: {
                        item.color = Qt.binding(function(){return dummyLabel.color})
                    }
                    active: type == ""
                    visible: root.homed
                }
            }
            Label {
                width: dummyLabel.font.pixelSize * 1.3

                color: dummyLabel.color
                font: dummyLabel.font
                text: root.title
                horizontalAlignment: Text.AlignRight
            }
            Label {
                width: dummyLabel.font.pixelSize * 6

                color: dummyLabel.color
                font: dummyLabel.font
                text: droRect.prefix + root.value.toFixed(droRect.decimals) + droRect.suffix
                horizontalAlignment: Text.AlignRight
            }
        }
    }

    Component {
        id: homedSymbol
        Canvas {
            property color color: "black"
            id: canvas
            contextType: "2D"
            width: 100
            height: 100
            onPaint: {
                var ctx = canvas.getContext("2d");
                var radius = Math.ceil(canvas.width/2 * 0.6)
                var size = canvas.width/2

                ctx.save();
                ctx.clearRect(0,0,canvas.width, canvas.height);
                ctx.strokeStyle = color
                ctx.lineWidth = 1
                ctx.fillStyle = color

                ctx.beginPath();
                ctx.moveTo(size-radius,size)
                ctx.arcTo(size-radius,size-radius,size,size-radius,radius)
                ctx.lineTo(canvas.width/2,canvas.height/2);    // right side
                ctx.closePath()
                ctx.stroke()

                ctx.beginPath();
                ctx.moveTo(size+radius,size)
                ctx.arcTo(size+radius,size+radius,size,size+radius,radius)
                ctx.lineTo(canvas.width/2,canvas.height/2);    // right side
                ctx.closePath()
                ctx.stroke()

                ctx.beginPath();
                ctx.moveTo(size,size-radius)
                ctx.arcTo(size+radius,size-radius,size+radius,size,radius)
                ctx.lineTo(canvas.width/2,canvas.height/2);    // right side
                ctx.closePath()
                ctx.stroke()
                ctx.fill()

                ctx.beginPath();
                ctx.moveTo(size,size+radius)
                ctx.arcTo(size-radius,size+radius,size-radius,size,radius)
                ctx.lineTo(canvas.width/2,canvas.height/2);    // right side
                ctx.closePath()
                ctx.stroke()
                ctx.fill()

                ctx.beginPath();
                ctx.moveTo(size, 0)
                ctx.lineTo(size, size*2)
                ctx.closePath()
                ctx.stroke()

                ctx.beginPath();
                ctx.moveTo(0,size)
                ctx.lineTo(size*2, size)
                ctx.closePath()
                ctx.stroke()

                ctx.restore()
            }
        }
    }

    ColumnLayout {
        id: positionLayout
        anchors.left: parent.left
        anchors.top: parent.top
        anchors.margins: Screen.pixelDensity
        spacing: Screen.pixelDensity * 0.7

        Repeater {
            model: droRect.axes
            Loader {
                sourceComponent: textLine
                onLoaded: {
                    item.title = Qt.binding(function(){return droRect.axisNames[index]})
                    item.type = ""
                    item.value = Qt.binding(function(){return droRect.position[droRect._axisNames[index]]})
                    item.homed = Qt.binding(function(){return droRect.axisHomed[index].homed})
                }
            }
        }

        Loader {
            sourceComponent: textLine
            onLoaded: {
                item.title = "Vel:"
                item.type = ""
                item.value = Qt.binding(function(){return droRect.velocity})
            }
            active: !droRect.offsetsVisible
        }
    }

    ColumnLayout {
        id: dtgLayout
        anchors.left: positionLayout.right
        anchors.top: parent.top
        anchors.topMargin: Screen.pixelDensity
        anchors.leftMargin: Screen.pixelDensity * 5
        spacing: Screen.pixelDensity * 0.7
        visible: droRect.offsetsVisible

        Repeater {
            model: droRect.axes
            Loader {
                sourceComponent: textLine
                onLoaded: {
                    item.title = Qt.binding(function(){return droRect.axisNames[index]})
                    item.type = "DTG"
                    item.value = Qt.binding(function(){return droRect.dtg[droRect._axisNames[index]]})
                }
            }
        }
    }

    ColumnLayout {
        id: g5xLayout
        anchors.left: parent.left
        anchors.top: positionLayout.bottom
        anchors.leftMargin: Screen.pixelDensity
        anchors.topMargin: Screen.pixelDensity * 3
        spacing: Screen.pixelDensity * 0.7
        visible: droRect.offsetsVisible

        Repeater {
            model: droRect.axes
            Loader {
                sourceComponent: textLine
                onLoaded: {
                    item.title = Qt.binding(function(){return droRect.axisNames[index]})
                    item.type = Qt.binding(function(){return droRect.g5xNames[droRect.g5xIndex-1]})
                    item.value = Qt.binding(function(){return droRect.g5xOffset[droRect._axisNames[index]]})
                }
            }
        }
    }

    ColumnLayout {
        id: g92Layout
        anchors.left: g5xLayout.right
        anchors.top: dtgLayout.bottom
        anchors.leftMargin: Screen.pixelDensity * 5
        anchors.topMargin: Screen.pixelDensity * 3
        spacing: Screen.pixelDensity * 0.7
        visible: droRect.offsetsVisible

        Repeater {
            model: droRect.axes
            Loader {
                sourceComponent: textLine
                onLoaded: {
                    item.title = Qt.binding(function(){return droRect.axisNames[index]})
                    item.type = "G92"
                    item.value = Qt.binding(function(){return droRect.g92Offset[droRect._axisNames[index]]})
                }
            }
        }
    }

    ColumnLayout {
        id: tloLayout
        anchors.left: parent.left
        anchors.top: g5xLayout.bottom
        anchors.leftMargin: Screen.pixelDensity
        anchors.topMargin: Screen.pixelDensity * 3
        spacing: Screen.pixelDensity * 0.7
        visible: droRect.offsetsVisible

        Repeater {
            model: droRect.axes
            Loader {
                sourceComponent: textLine
                onLoaded: {
                    item.title = Qt.binding(function(){return droRect.axisNames[index]})
                    item.type = "TLO"
                    item.value = Qt.binding(function(){return droRect.toolOffset[droRect._axisNames[index]]})
                }
            }
        }
    }
}