import QtQuick 2.5
import Machinekit.Application 1.0
import QtQuick.Controls 2.2
import "../src/pathview"

ConnectedTestBase {
    id: root

    PathView3D {
        id: pathView
        anchors.fill: parent
        enabled: true
        viewMode: "Lathe"
    }

    TextArea {
        anchors.fill: parent
        text: "y min limit: " + appObject.status.config.axis[1].minPositionLimit +
            "\nindices: " + appObject.helper.axisIndices +
            "\naxisNames: " + appObject.helper.axisNames +
            "\naxes: " + appObject.status.config.axis.length +
            "\nlathe: " + appObject.status.config.lathe
        color: "white"
    }

    ApplicationObject {
        id: appObject
    }

    ApplicationHelper {
        id: appHelper
    }
}
