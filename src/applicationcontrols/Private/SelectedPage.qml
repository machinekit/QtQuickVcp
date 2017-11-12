import QtQuick 2.1
import QtQuick.Controls 1.1
import QtQuick.Layouts 1.1
import QtQuick.Window 2.0

Item {
    property var launcher: undefined
    property QtObject applicationLog: QtObject { property bool connected: false }

    signal goBack()

    id: root
    width: 600
    height: 500

    QtObject {
        id: d
        readonly property string stdoutText: (launcher !== undefined) && (launcher.output !== undefined) ?
                                        launcher.output.join("") : ""
        readonly property string launcherName: (launcher !== undefined) ? launcher.name : ""
        readonly property string titleText: {
            if (isRunning) {
                return qsTr("Starting %1...").arg(launcherName);
            }
            else if (hasError) {
                return qsTr("Error starting %1").arg(launcherName);
            }
            else {
                return qsTr("%1 exited").arg(launcherName);
            }
        }
        readonly property var image: (launcher !== undefined) && (launcher.image !== undefined) ? launcher.image : undefined
        readonly property bool isRunning: (launcher !== undefined) && launcher.running
        readonly property bool hasError: (launcher !== undefined) && (!launcher.running) && (launcher.returncode !== 0)
        readonly property int returncode: (launcher !== undefined) ? launcher.returncode : 0

        property string logText: ""

        function addLogEntry(message) {
            logText += message.timestamp + " - [" + message.tag + "]: " + message.text + "\n";
        }

        function clearLog() {
            logText = "";
        }
    }

    /*! /internal
        Cannot directly connect to slots since the file property is var and not a QObject.
    */
    onApplicationLogChanged: {
        if (root.applicationLog.onMessageReceived) {
            root.applicationLog.onMessageReceived.connect(d.addLogEntry)
        }
    }

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
            text: d.titleText
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
            Layout.preferredHeight: root.height * 0.6
            fillMode: Image.PreserveAspectFit
            source: d.image !== undefined ? d.image.url : "qrc:Machinekit/Application/Controls/icons/machinekit"
            visible: !d.hasError
        }

        Label {
            Layout.fillWidth: true
            font.pointSize: dummyText.font.pointSize * 1.2
            font.bold: true
            horizontalAlignment: Text.AlignHCenter
            visible: d.hasError
            wrapMode: Text.WordWrap
            text: visible ? qsTr("Process exited with return code %1. See the log for details.").arg(d.returncode) : ""
        }

        TabView {
            Layout.fillHeight: true
            Layout.fillWidth: true

            Tab {
                active: true
                title: qsTr("Application Output")
                TextArea {
                    id: stdoutTextArea
                    text: d.stdoutText
                    readOnly: true
                }
            }

            Tab {
                active: true
                title: qsTr("Machinekit Log")
                TextArea {
                    id: logTextArea
                    text: d.logText
                    readOnly: true
                }
            }
        }
    }
}

