import QtQuick 2.0
import Machinekit.Application 1.0
import Machinekit.Application.Controls 1.0
import Machinekit.Controls 1.0
import Machinekit.PathView 1.0

ServiceWindow {
    default property alias data: container.data
    property alias applicationCore: applicationCore
    property alias pathViewCore: pathViewCore

    id: root
    width: 900
    height: 700

    ApplicationCore {
        id: applicationCore
    }

    PathViewCore {
        id: pathViewCore
    }

    TestBackground {
        id: container
        anchors.fill: parent
    }

    Led {
        anchors.top: parent.top
        anchors.right: parent.right
        color: "lightgreen"
        value: applicationCore.status.synced
    }
}
