import QtQuick 2.0

Item {
    default property alias data: container.data
    property double referenceWidth: 2560
    property double referenceHeight: 1600
    property double scale: (d.scale1 + d.scale2) / 2

    id: root

    QtObject {
        id: d
        readonly property int contentWidth: Math.max(root.referenceWidth, root.width / root.scale)
        readonly property int contentHeight: Math.min(root.referenceHeight, root.height / root.scale)
        readonly property double scale1: root.height / root.referenceHeight
        readonly property double scale2: root.width / root.referenceWidth
    }

    Item {
        id: container
        width: d.contentWidth
        height: d.contentHeight
        anchors.centerIn: root
        scale: root.scale
    }
}
