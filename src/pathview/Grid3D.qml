import QtQuick 2.0
import Machinekit.PathView 1.0

Canvas3D {
    property vector2d size: Qt.vector2d(5.0, 5.0)
    property real lineWidth: 1.0
    property color color: "gray"
    property real intervalX: 0.5
    property real intervalY: 0.5

    id: root
    onPaint: {
        var posX = 0
        var posY = 0

        context.reset()
        context.color(root.color)
        context.lineWidth(root.lineWidth)
        context.beginUnion()
        while (posX <= root.size.x) {
            context.lineFromTo(posX, 0.0, 0.0,
                               posX, root.size.y, 0.0)
            posX += intervalX
        }

        while (posY <= root.size.y) {
            context.lineFromTo(0.0, posY, 0.0,
                               root.size.x, posY, 0.0)
            posY += intervalY
        }
        context.endUnion()
    }

    Component.onCompleted: {
        onSizeChanged.connect(needsUpdate)
        onLineWidthChanged.connect(needsUpdate)
        onColorChanged.connect(needsUpdate)
        onIntervalXChanged.connect(needsUpdate)
        onIntervalYChanged.connect(needsUpdate)
    }
}
