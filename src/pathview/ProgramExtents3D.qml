import QtQuick 2.0
import Machinekit.PathView 1.0

Canvas3D {
    property int axes: 3
    property vector3d maximum: Qt.vector3d(2.0, 3.0, 2.5)
    property vector3d minimum: Qt.vector3d(1.0, 0.8, 0.0)
    property vector3d limitMaximum: Qt.vector3d(1.5, 3.0, 2.5)
    property vector3d limitMinimum: Qt.vector3d(0.0, 0.0, 0.0)
    property color color: "magenta"
    property color limitColor: "red"
    property real textSize: 0.5
    property string prefix: ""
    property string suffix: ""
    property int decimals: 2

    QtObject {
        id: d
        property vector3d programSize: Qt.vector3d(root.maximum.x - root.minimum.x,
                                                   root.maximum.y - root.minimum.y,
                                                   root.maximum.z - root.minimum.z)
    }

    id: root
    onPaint: {
        var lineEnding = root.textSize
        var lineOffset = root.textSize
        var textOffset = root.textSize/4.0

        context.reset()

        if (root.minimum == root.maximum) {
            return
        }

        context.color(root.color)
        context.beginUnion()

            context.translate(root.minimum.x, root.minimum.y - lineOffset, 0.0)
            context.beginUnion()
                context.translate(0.0, lineEnding/2.0, 0.0)
                context.line(0.0, -lineEnding, 0.0)
                context.line(d.programSize.x, 0.0, 0.0)
                context.translate(d.programSize.x, lineEnding/2.0, 0.0)
                context.line(0.0, -lineEnding, 0.0)

                context.translate(d.programSize.x/2.0, -textOffset - root.textSize, 0.0)
                context.scale(root.textSize, root.textSize, root.textSize)
                context.text(root.prefix + d.programSize.x.toFixed(root.decimals) + root.suffix, GLView3D.AlignCenter)

                if (root.limitMinimum.x > root.minimum.x) {
                    context.color(root.limitColor)
                }
                context.translate(d.programSize.x + root.textSize/2.0, -textOffset - lineEnding/2.0, 0.0)
                context.scale(root.textSize, root.textSize, root.textSize)
                context.rotate(90, 0.0, 0.0, 1.0)
                context.text(root.prefix + (root.minimum.x + d.programSize.x).toFixed(root.decimals) + root.suffix, GLView3D.AlignRight)

                if (root.limitMaximum.x < root.maximum.x) {
                    context.color(root.limitColor)
                }
                context.translate(root.textSize/2.0, -textOffset - lineEnding/2.0, 0.0)
                context.scale(root.textSize, root.textSize, root.textSize)
                context.rotate(90, 0.0, 0.0, 1.0)
                context.text(root.prefix + root.minimum.x.toFixed(root.decimals) + root.suffix, GLView3D.AlignRight)
            context.endUnion()

            if (axes > 1)
            {
                context.rotate(90, 0.0, 0.0, 1.0)
                context.translate(root.minimum.y, lineOffset - root.minimum.x, 0.0)
                context.beginUnion()
                    context.translate(0.0, lineEnding/2.0, 0.0)
                    context.line(0.0, -lineEnding, 0.0)
                    context.line(d.programSize.y, 0.0, 0.0)
                    context.translate(d.programSize.y, lineEnding/2.0, 0.0)
                    context.line(0.0, -lineEnding, 0.0)

                    context.translate(d.programSize.y/2.0, textOffset + root.textSize, 0.0)
                    context.rotate(180, 0.0, 0.0, 1.0)
                    context.scale(root.textSize, root.textSize, root.textSize)
                    context.text(root.prefix + d.programSize.y.toFixed(root.decimals) + root.suffix, GLView3D.AlignCenter)

                    if (root.limitMinimum.y > root.minimum.y) {
                        context.color(root.limitColor)
                    }
                    context.translate(d.programSize.y - root.textSize/2.0, textOffset + lineEnding/2.0, 0.0)
                    context.scale(root.textSize, root.textSize, root.textSize)
                    context.rotate(-90, 0.0, 0.0, 1.0)
                    context.text(root.prefix + (root.minimum.y + d.programSize.y).toFixed(root.decimals) + root.suffix, GLView3D.AlignRight)

                    if (root.limitMaximum.y < root.maximum.y) {
                        context.color(root.limitColor)
                    }
                    context.translate(-root.textSize/2.0, textOffset + lineEnding/2.0, 0.0)
                    context.scale(root.textSize, root.textSize, root.textSize)
                    context.rotate(-90, 0.0, 0.0, 1.0)
                    context.text(root.prefix + root.minimum.y.toFixed(root.decimals) + root.suffix, GLView3D.AlignRight)
                context.endUnion()
            }

            if (axes > 2)
            {
                context.rotate(-90, 0.0, 1.0, 0.0)
                context.translate(root.minimum.z, -lineOffset + root.minimum.y, -root.minimum.x)
                context.beginUnion()
                    context.translate(0.0, lineEnding/2.0, 0.0)
                    context.line(0.0, -lineEnding, 0.0)
                    context.line(d.programSize.z, 0.0, 0.0)
                    context.translate(d.programSize.z, lineEnding/2.0, 0.0)
                    context.line(0.0, -lineEnding, 0.0)

                    context.translate(d.programSize.z/2.0, -textOffset - root.textSize, 0.0)
                    context.scale(root.textSize, root.textSize, root.textSize)
                    context.text(root.prefix + d.programSize.z.toFixed(root.decimals) + root.suffix, GLView3D.AlignCenter)

                    if (root.limitMinimum.z > root.minimum.z) {
                        context.color(root.limitColor)
                    }
                    context.translate(d.programSize.z - root.textSize/2.0, -textOffset - lineEnding/2.0, 0.0)
                    context.scale(root.textSize, root.textSize, root.textSize)
                    context.rotate(-90, 0.0, 0.0, 1.0)
                    context.text(root.prefix + (root.minimum.z + d.programSize.z).toFixed(root.decimals) + root.suffix, GLView3D.AlignLeft)

                    if (root.limitMaximum.z < root.maximum.z) {
                        context.color(root.limitColor)
                    }
                    context.translate(-root.textSize/2.0, -textOffset - lineEnding/2.0, 0.0)
                    context.scale(root.textSize, root.textSize, root.textSize)
                    context.rotate(-90, 0.0, 0.0, 1.0)
                    context.text(root.prefix + root.minimum.z.toFixed(root.decimals) + root.suffix, GLView3D.AlignLeft)
                context.endUnion()
            }

        context.endUnion()
    }

    Component.onCompleted: {
        onAxesChanged.connect(needsUpdate)
        onMaximumChanged.connect(needsUpdate)
        onMinimumChanged.connect(needsUpdate)
        onColorChanged.connect(needsUpdate)
        onTextSizeChanged.connect(needsUpdate)
        onPrefixChanged.connect(needsUpdate)
        onSuffixChanged.connect(needsUpdate)
        onDecimalsChanged.connect(needsUpdate)
    }
}
