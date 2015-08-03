import QtQuick 2.1
import QtQuick.Controls 1.1
import QtQuick.Layouts 1.1
import QtQuick.Window 2.0

Item {
    property var applicationConfig: {"selectedConfig": {"name": "TestConfig"}}
    property string loadingName: applicationConfig.selectedConfig.name

    signal goBack()

    id: root
    width: 700
    height: 600

    Label {
        id: dummyText
        visible: false
    }

    Button {
        anchors.top: parent.top
        anchors.right: parent.right
        anchors.margins: Screen.pixelDensity
        text: qsTr("Back")
        onClicked: goBack()
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
        height: Math.min(root.width, root.height) * 0.15
        width: height
    }
}

