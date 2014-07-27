import QtQuick 2.0
import Machinekit.PathView 1.0

Canvas3D {
    property vector3d size: Qt.vector3d(10.0, 10.0, 10.0)
    property color color: "red"
    property bool lineStipple: true
    property real lineStippleLength: size.x / 10.0

    id: root

    onPaint: {
        context.reset()
        context.lineStipple(root.lineStipple, root.lineStippleLength)
        context.color(root.color)
        context.beginUnion()
        context.lineTo(root.size.x, 0.0, 0.0)
        context.lineTo(root.size.x, root.size.y, 0.0)
        context.lineTo(0.0, root.size.z, 0.0)
        context.lineTo(0.0, 0.0, 0.0)
        context.lineTo(0.0, 0.0, root.size.z)
        context.lineTo(root.size.x, 0.0, root.size.z)
        context.lineTo(root.size.x, root.size.y, root.size.z)
        context.lineTo(0.0, root.size.y, root.size.z)
        context.lineTo(0.0, 0.0, root.size.z)
        context.resetTransformations()
        context.lineFromTo(0.0, root.size.y, 0.0,
                           0.0, root.size.y, root.size.z)
        context.lineFromTo(root.size.x, root.size.y, 0.0,
                           root.size.x, root.size.y, root.size.z)
        context.lineFromTo(root.size.x, 0.0, 0.0,
                           root.size.x, 0.0, root.size.z)
        context.endUnion()
    }

    Component.onCompleted: {
        onSizeChanged.connect(needsUpdate)
        onColorChanged.connect(needsUpdate)
        onLineStippleChanged.connect(needsUpdate)
        onLineStippleLengthChanged.connect(needsUpdate)
    }
}
