import QtQuick 2.0

Text {  // using icon fonts is an alternative
    font.family: "Material Icons"
    anchors.fill: parent
    font.pixelSize: parent.width * 0.95
    verticalAlignment: Text.AlignVCenter
    horizontalAlignment: Text.AlignHCenter
    opacity: enabled ? 1.0 : 0.4
}
