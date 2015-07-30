import QtQuick 2.1
import QtQuick.Controls 1.1
import QtQuick.Layouts 1.1
import QtQuick.Window 2.0

Item {
    property var launcher: {"name": "Test-config",
                            "output": ["First line", "second line", "third line"]}
    property string stdoutText: (launcher !== undefined) && (launcher.output !== undefined) ?
                                    launcher.output.join("") : ""
    property string launcherName: launcher != undefined ? launcher.name : ""
    property string titleText: {
        if (isRunning) {
            return qsTr("Starting ") + launcherName + "..."
        }
        else if (hasError) {
            return qsTr("Error starting ") + launcherName
        }
        else {
            return launcherName + qsTr(" exited")
        }
    }
    property var image: (launcher !== undefined) && (launcher.image !== undefined) ? launcher.image: undefined
    property bool isRunning: (launcher !== undefined) && launcher.running
    property bool hasError: (launcher !== undefined) && (!launcher.running) && (launcher.returncode !== 0)
    property int returncode: (launcher !== undefined) ? launcher.returncode : 0

    signal goBack()

    id: root
    width: 600
    height: 500

    Label {
        id: dummyText
        visible: false
    }

    Button {
        id: dummyButton
        visible: false
    }

    ColumnLayout {
        anchors.fill: parent
        anchors.margins: Screen.pixelDensity
        spacing: Screen.pixelDensity

        Label {
            id: pageTitleText

            Layout.fillWidth: true
            Layout.preferredHeight: Math.max(dummyButton.height, implicitHeight)
            text: titleText
            font.pointSize: dummyText.font.pointSize * 1.3
            font.bold: true
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter
            wrapMode: Text.WordWrap

            RowLayout {
                anchors.fill: parent

                Item { Layout.fillWidth: true }
                Button {
                    text: qsTr("Back")
                    onClicked: goBack()
                }
            }
        }

        Image {
            Layout.fillWidth: true
            Layout.fillHeight: true
            fillMode: Image.PreserveAspectFit
            source: root.image !== undefined ? root.image.url : "qrc:Machinekit/Application/Controls/icons/machinekit"
            visible: !hasError
        }

        Label {
            Layout.fillWidth: true
            font.pointSize: dummyText.font.pointSize * 1.2
            font.bold: true
            horizontalAlignment: Text.AlignHCenter
            visible: hasError
            wrapMode: Text.WordWrap
            text: visible ? qsTr("Process exited with return code ") + returncode + qsTr(". See the log for details.") : ""
        }

        TextArea {
            Layout.fillHeight: true
            Layout.fillWidth: true
            text: stdoutText
            visible: true
            readOnly: true
        }
    }
}

