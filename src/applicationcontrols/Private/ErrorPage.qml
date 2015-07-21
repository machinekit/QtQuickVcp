import QtQuick 2.1
import QtQuick.Controls 1.1
import QtQuick.Layouts 1.1
import QtQuick.Window 2.0
import Machinekit.Controls 1.0

Item {
    property string errorType: "Error Type"
    property string errorText: "Error Text"

    id: root
    width: 700
    height: 600

    Label {
        id: dummyText
        visible: false
    }

    Label {
        id: errorLabel
        anchors.fill: parent
        anchors.margins: Screen.pixelDensity
        horizontalAlignment: Text.AlignHCenter
        verticalAlignment: Text.AlignVCenter
        wrapMode: Text.WordWrap
        font.pointSize: dummyText.font.pointSize * 1.1
        text: errorType + "\n" + errorText
    }
}

