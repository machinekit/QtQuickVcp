import QtQuick 2.0
import Machinekit.PathView 1.0

Canvas3D {
    property int axes: 3
    property real axesLength: 5.0
    property real textSize: 1.0
    property color xAxisColor: "red"
    property color yAxisColor: "green"
    property color zAxisColor: "blue"

    id: root
    onPaint: {
        context.prepare(this)
        context.reset()

        context.color(root.xAxisColor)
        context.beginUnion()
        context.line(root.axesLength, 0.0, 0.0)
        context.translate(root.axesLength, -root.textSize/2.0, 0.0)
        context.scale(root.textSize, root.textSize, root.textSize)
        context.text("X")
        context.endUnion()

        if (axes > 1)
        {
            context.color(root.yAxisColor)
            context.beginUnion()
            context.line(0.0, root.axesLength, 0.0)
            context.translate(root.textSize/2.0, root.axesLength, 0.0)
            context.rotate(90, 0.0, 0.0, 1.0)
            context.scale(root.textSize, root.textSize, root.textSize)
            context.text("Y")
            context.endUnion()
        }

        if (axes > 2)
        {
            context.color(root.zAxisColor)
            context.beginUnion()
            context.line(0.0, 0.0, root.axesLength)
            context.translate(0.0, 0.0, root.axesLength)
            context.rotate(90, 1.0, 0.0, 0.0)
            context.scale(root.textSize, root.textSize, root.textSize)
            context.text("Z", GLView3D.AlignCenter)
            context.endUnion()
        }
        context.update()
    }

    Component.onCompleted: {
        onAxesChanged.connect(needsUpdate)
        onAxesLengthChanged.connect(needsUpdate)
        onTextSizeChanged.connect(needsUpdate)
        onXAxisColorChanged.connect(needsUpdate)
        onYAxisColorChanged.connect(needsUpdate)
        onZAxisColorChanged.connect(needsUpdate)
    }
}
