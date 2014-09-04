import QtQuick 2.0
import Machinekit.PathView 1.0

Canvas3D {
    property int axes: 3
    property vector3d minimum: Qt.vector3d(0.0, 0.0, 0.0)
    property vector3d maximum: Qt.vector3d(10.0, 10.0, 10.0)
    property color color: "red"
    property bool lineStipple: true
    property real lineStippleLength: (maximum.x - minimum.x)  / 10.0

    id: root

    onPaint: {
        var size = Qt.vector3d(maximum.x - minimum.x, maximum.y - minimum.y, maximum.z - minimum.z)
        context.reset()
        context.lineStipple(root.lineStipple, root.lineStippleLength)
        context.color(root.color)
        context.translate(minimum)
        context.beginUnion()
        context.lineTo(size.x, 0.0, 0.0)
        context.lineTo(size.x, size.y, 0.0)
        context.lineTo(0.0, size.y, 0.0)
        context.lineTo(0.0, 0.0, 0.0)
        if (axes > 2) {
            context.lineTo(0.0, 0.0, size.z)
            context.lineTo(size.x, 0.0, size.z)
            context.lineTo(size.x, size.y, size.z)
            context.lineTo(0.0, size.y, size.z)
            context.lineTo(0.0, 0.0, size.z)
            context.resetTransformations()
            context.lineFromTo(0.0, size.y, 0.0,
                               0.0, size.y, size.z)
            context.lineFromTo(size.x, size.y, 0.0,
                               size.x, size.y, size.z)
            context.lineFromTo(size.x, 0.0, 0.0,
                               size.x, 0.0, size.z)
        }
        context.endUnion()
    }

    Component.onCompleted: {
        onAxesChanged.connect(needsUpdate)
        onMinimumChanged.connect(needsUpdate)
        onMaximumChanged.connect(needsUpdate)
        onColorChanged.connect(needsUpdate)
        onLineStippleChanged.connect(needsUpdate)
        onLineStippleLengthChanged.connect(needsUpdate)
    }
}
