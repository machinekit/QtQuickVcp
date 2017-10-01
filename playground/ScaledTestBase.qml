import QtQuick 2.0

import "../src/controls"

ScaleContainer {

    default property alias data: container.data

    width: 900
    height: 700
    scale: 0.6

    TestBackground {
        id: container
        anchors.fill: parent
    }
}
