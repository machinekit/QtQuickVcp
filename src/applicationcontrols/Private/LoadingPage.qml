import QtQuick 2.1
import QtQuick.Controls 1.1
import QtQuick.Layouts 1.1
import QtQuick.Window 2.0

Item {
    property var applicationConfig: {"selectedConfig": {"name": "TestConfig"}}
    property string loadingName: applicationConfig.selectedConfig.name

    id: root
    width: 700
    height: 600

    Label {
        id: dummyText
        visible: false
    }

    Label {
        id: connectingLabel

        anchors.horizontalCenter: parent.horizontalCenter
        anchors.bottom: connectingIndicator.top
        anchors.bottomMargin: Screen.pixelDensity
        font.pointSize: dummyText.font.pointSize * 1.3
        text: qsTr("Loading ") + (root.loadingName != "" ? root.loadingName : "user interface") + "..."
    }

    BusyIndicator {
        id: connectingIndicator

        anchors.centerIn: parent
        running: true
        height: (parent.height > parent.width) ? parent.height * 0.10 : parent.width * 0.10
        width: height
    }
}

