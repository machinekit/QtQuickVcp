import QtQuick 2.1
import QtQuick.Controls 1.1
import QtQuick.Layouts 1.1
import QtQuick.Window 2.0

Item {
    property var applicationLauncher: {"launchers": []}
    property int selectedLauncher: 0

    signal launcherSelected(int index)
    signal goBack()

    id: root
    width: 600
    height: 400

    Label {
        id: dummyText
        visible: false
    }

    ColumnLayout {
        anchors.fill: parent
        anchors.margins: Screen.logicalPixelDensity*3
        spacing: Screen.logicalPixelDensity*3

        Label {
            id: pageTitleText

            Layout.fillWidth: true
            text: qsTr("Available Launchers:")
            font.pointSize: dummyText.font.pointSize * 1.3
            font.bold: true
            horizontalAlignment: Text.AlignHCenter
            wrapMode: Text.WordWrap

            RowLayout {
                anchors.fill: parent
                Button {
                    text: qsTr("List")
                    onClicked: launcherListView.viewMode = "list"
                }
                Button {
                    text: qsTr("Small")
                    onClicked: launcherListView.viewMode = "small"
                }
                Button {
                    text: qsTr("Big")
                    onClicked: launcherListView.viewMode = "big"
                }
                Item { Layout.fillWidth: true }
                Button {
                    text: qsTr("Back")
                    onClicked: goBack()
                }
            }
        }

        GridView {
            property string viewMode: "small"

            id: launcherListView
            Layout.fillHeight: true
            Layout.fillWidth: true
            cellWidth: width * (viewMode === "big" ? 0.333 : (viewMode === "small" ? 0.2 : 1.0))
            cellHeight: viewMode === "list" ? dummyButton.height * 3 : cellWidth
            clip: true
            model: applicationLauncher.launchers
            delegate: Item {
                property var item: launcherListView.model[index]

                width: launcherListView.cellWidth
                height: launcherListView.cellHeight

                Button {
                    anchors.fill: parent
                    anchors.margins: Screen.logicalPixelDensity*1.5

                    ColumnLayout {
                        anchors.fill: parent
                        anchors.margins: Screen.pixelDensity * 3

                        Item {
                            Layout.fillHeight: true
                            Layout.fillWidth: true

                            Image {
                                anchors.fill: parent
                                fillMode: Image.PreserveAspectFit
                                source: item.image !== undefined ? item.image.url : "qrc:Machinekit/Application/Controls/icons/machinekit"
                                visible: launcherListView.viewMode === "list" ? false : true
                                opacity: item.running ? 0.05 : 1.0
                            }

                            Label {
                                anchors.centerIn: parent
                                text:  item.terminating ? qsTr("Terminating...") : qsTr("Running...")
                                visible: item.running
                            }

                            Button {
                                anchors.right: parent.right
                                anchors.top: parent.top
                                text:  item.terminating ? qsTr("Kill") : qsTr("Terminate")
                                visible: item.running

                                onClicked: {
                                    if (item.terminating) {
                                       applicationLauncher.kill(index)
                                    }
                                    else {
                                       applicationLauncher.terminate(index)
                                    }
                                }
                            }
                        }

                        Label {
                            id: titleText2

                            Layout.fillWidth: true
                            font.pointSize: dummyText.font.pointSize * (launcherListView.viewMode === "small" ? 1.0 : 1.3)
                            font.bold: true
                            horizontalAlignment: Text.AlignHCenter
                            verticalAlignment: Text.AlignVCenter
                            text: item.name
                            elide: Text.ElideRight
                        }
                    }

                    onClicked: {
                        if (!item.running) {
                            applicationLauncher.start(index)
                        }
                        selectedLauncher = index
                        launcherSelected(index)
                    }
                }
            }

            BusyIndicator {
                anchors.centerIn: parent
                running: true
                visible: applicationLauncher.launchers.length === 0
                height: parent.height * 0.15
                width: height
            }
        }
    }
}

