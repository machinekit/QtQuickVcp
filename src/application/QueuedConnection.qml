import QtQuick 2.5

Timer {
    property bool input: false
    property bool output: false

    id: root
    interval: 1
    repeat: false
    onInputChanged: restart()
    onTriggered: output = input
}
