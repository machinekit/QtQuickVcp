import QtQuick 2.0
import Machinekit.PathView 1.0

Canvas3D {
    property vector3d programSize: Qt.vector3d(2.0, 3.0, 2.5)
    property vector3d programPosition: Qt.vector3d(1.0, 0.8, 0.0)
    property color color: "magenta"
    property real textSize: 0.5
    property string prefix: ""
    property string suffix: ""
    property int decimals: 2

    id: root
    onPaint: {
        var lineEnding = root.textSize
        var lineOffset = root.textSize
        var textOffset = root.textSize/4.0

        context.reset()

        context.color(root.color)
        context.beginUnion()

            context.translate(root.programPosition.x, -lineOffset, 0.0)
            context.beginUnion()
                context.translate(0.0, lineEnding/2.0, 0.0)
                context.line(0.0, -lineEnding, 0.0)
                context.line(root.programSize.x, 0.0, 0.0)
                context.translate(root.programSize.x, lineEnding/2.0, 0.0)
                context.line(0.0, -lineEnding, 0.0)

                context.translate(root.programSize.x/2.0, -textOffset - root.textSize, 0.0)
                context.scale(root.textSize, root.textSize, root.textSize)
                context.text(root.prefix + root.programSize.x.toFixed(root.decimals) + root.suffix, GLView3D.AlignCenter)

                context.translate(root.programSize.x + root.textSize/2.0, -textOffset - lineEnding/2.0, 0.0)
                context.scale(root.textSize, root.textSize, root.textSize)
                context.rotate(90, 0.0, 0.0, 1.0)
                context.text(root.prefix + (root.programPosition.x + root.programSize.x).toFixed(root.decimals) + root.suffix, GLView3D.AlignRight)

                context.translate(root.textSize/2.0, -textOffset - lineEnding/2.0, 0.0)
                context.scale(root.textSize, root.textSize, root.textSize)
                context.rotate(90, 0.0, 0.0, 1.0)
                context.text(root.prefix + root.programPosition.x.toFixed(root.decimals) + root.suffix, GLView3D.AlignRight)
            context.endUnion()

            context.rotate(90, 0.0, 0.0, 1.0)
            context.translate(root.programPosition.y, lineOffset, 0.0)
            context.beginUnion()
                context.translate(0.0, lineEnding/2.0, 0.0)
                context.line(0.0, -lineEnding, 0.0)
                context.line(root.programSize.y, 0.0, 0.0)
                context.translate(root.programSize.y, lineEnding/2.0, 0.0)
                context.line(0.0, -lineEnding, 0.0)

                context.translate(root.programSize.y/2.0, textOffset + root.textSize, 0.0)
                context.rotate(180, 0.0, 0.0, 1.0)
                context.scale(root.textSize, root.textSize, root.textSize)
                context.text(root.prefix + root.programSize.y.toFixed(root.decimals) + root.suffix, GLView3D.AlignCenter)

                context.translate(root.programSize.y - root.textSize/2.0, textOffset + lineEnding/2.0, 0.0)
                context.scale(root.textSize, root.textSize, root.textSize)
                context.rotate(-90, 0.0, 0.0, 1.0)
                context.text(root.prefix + (root.programPosition.y + root.programSize.y).toFixed(root.decimals) + root.suffix, GLView3D.AlignRight)

                context.translate(-root.textSize/2.0, textOffset + lineEnding/2.0, 0.0)
                context.scale(root.textSize, root.textSize, root.textSize)
                context.rotate(-90, 0.0, 0.0, 1.0)
                context.text(root.prefix + root.programPosition.y.toFixed(root.decimals) + root.suffix, GLView3D.AlignRight)
            context.endUnion()

            context.rotate(-90, 0.0, 1.0, 0.0)
            context.translate(root.programPosition.z, -lineOffset, 0.0)
            context.beginUnion()
                context.translate(0.0, lineEnding/2.0, 0.0)
                context.line(0.0, -lineEnding, 0.0)
                context.line(root.programSize.z, 0.0, 0.0)
                context.translate(root.programSize.z, lineEnding/2.0, 0.0)
                context.line(0.0, -lineEnding, 0.0)

                context.translate(root.programSize.z/2.0, -textOffset - root.textSize, 0.0)
                context.scale(root.textSize, root.textSize, root.textSize)
                context.text(root.prefix + root.programSize.z.toFixed(root.decimals) + root.suffix, GLView3D.AlignCenter)

                context.translate(root.programSize.z - root.textSize/2.0, -textOffset - lineEnding/2.0, 0.0)
                context.scale(root.textSize, root.textSize, root.textSize)
                context.rotate(-90, 0.0, 0.0, 1.0)
                context.text(root.prefix + (root.programPosition.z + root.programSize.z).toFixed(root.decimals) + root.suffix, GLView3D.AlignLeft)

                context.translate(-root.textSize/2.0, -textOffset - lineEnding/2.0, 0.0)
                context.scale(root.textSize, root.textSize, root.textSize)
                context.rotate(-90, 0.0, 0.0, 1.0)
                context.text(root.prefix + root.programPosition.z.toFixed(root.decimals) + root.suffix, GLView3D.AlignLeft)
            context.endUnion()

        context.endUnion()
    }

    Component.onCompleted: {
        onProgramSizeChanged.connect(needsUpdate)
        onProgramPositionChanged.connect(needsUpdate)
        onColorChanged.connect(needsUpdate)
        onTextSizeChanged.connect(needsUpdate)
        onPrefixChanged.connect(needsUpdate)
        onSuffixChanged.connect(needsUpdate)
        onDecimalsChanged.connect(needsUpdate)
    }
}
