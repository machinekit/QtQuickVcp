import QtQuick 2.1
import QtQuick.Controls 1.1
import QtQuick.Layouts 1.1
import QtQuick.Window 2.0

Item {
    id: root

    Label {
        id: dummyText
        visible: false
    }

    Label {
        anchors.fill: parent
        anchors.verticalCenter: parent.verticalCenter
        anchors.margins: Screen.pixelDensity
        horizontalAlignment: Text.AlignHCenter
        verticalAlignment: Text.AlignVCenter
        wrapMode: Text.WordWrap
        font.pointSize: dummyText.font.pointSize * 1.1
        text: qsTr("Warning!<br>No network connection found, service discovery unavailable. Please check your network connection.")
    }
}
