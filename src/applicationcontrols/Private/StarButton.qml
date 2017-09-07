import QtQuick 2.0

Image {
    property bool liked: false
    readonly property string imageSource: liked ? "qrc:Machinekit/Application/Controls/icons/star-black" : "qrc:Machinekit/Application/Controls/icons/star-border-black"

    signal clicked()

    id: root
    fillMode: Image.PreserveAspectFit
    source: liked || starMouseArea.containsMouse ? imageSource : ""

    MouseArea {
        id: starMouseArea
        anchors.fill: parent
        cursorShape: Qt.PointingHandCursor
        hoverEnabled: true
        onClicked: root.clicked()
    }
}
