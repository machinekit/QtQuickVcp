import QtQuick 2.1
import QtQuick.Controls 1.1
import QtQuick.Layouts 1.1
import QtQuick.Dialogs 1.2
import QtQuick.Window 2.0

Item {
    property var applicationLauncher: {"launchers": []}
    property int selectedLauncher: 0
    property var configService: {"ready": false}

    readonly property bool hasSmallScreen: (root.width / Screen.pixelDensity) < 110.0
    property string viewMode: hasSmallScreen ? "big" : "small"

    signal launcherSelected(int index)
    signal goBack()
    signal systemShutdown()

    id: root
    width: 600
    height: 400

    function __filterLaunchers(launchers) {
        var items = launchers.slice();
        var running = false;

        function moveItemToFront(index, position) {
            var item = items[index];
            items.splice(index, 1);
            items.splice(position, 0, item);
        }

        // move running and important items to the front
        var importantCount = 0;
        for (var i = 0; i < items.length; ++i) {
            items[i].index = i;  // store the original index
            if (items[i].running) {
                running = true;
                moveItemToFront(i, 0);
                importantCount++;
            }
            else if (items[i].importance > 0) {
                moveItemToFront(i, importantCount);
                importantCount++;
            }
        }

        // create a new launcher element if a configserver is running
        // but none of our launchers is running
        if (configService.ready && !running) {
            var launcher = {};
            launcher.name = configService.name;
            launcher.running = true;
            launcher.terminating = false;
            launcher.local = true;
            launcher.index = -1;
            launcher.importance = 0;

            items.unshift(launcher);
        }

        return items;
    }

    Label {
        id: dummyText
        visible: false
    }

    Button {
        id: dummyButton
        visible: false
    }

    MessageDialog {
        id: shutdownDialog
        title: qsTr("System Shutdown")
        text: qsTr("Do you really want to shutdown the Machinekit system?")
        standardButtons: StandardButton.Yes | StandardButton.No

        onYes: {
            systemShutdown();
            goBack();
        }
    }

    ColumnLayout {
        anchors.fill: parent
        anchors.margins: Screen.pixelDensity
        spacing: Screen.pixelDensity

        Label {
            id: pageTitleText

            Layout.fillWidth: true
            Layout.preferredHeight: Math.max(dummyButton.height, implicitHeight)
            text: qsTr("Available Launchers:")
            font.pointSize: dummyText.font.pointSize * 1.3
            font.bold: true
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter
            wrapMode: Text.WordWrap

            RowLayout {
                anchors.fill: parent
                Button {
                    id: listButton
                    text: qsTr("List")
                    checkable: true
                    onClicked: root.viewMode = "list"
                    Binding { target: listButton; property: "checked"; value: root.viewMode === "list" }
                }
                Button {
                    id: smallButton
                    text: qsTr("Small")
                    checkable: true
                    visible: !root.hasSmallScreen
                    onClicked: root.viewMode = "small"
                    Binding { target: smallButton; property: "checked"; value: root.viewMode === "small" }
                }
                Button {
                    id: bigButton
                    text: qsTr("Big")
                    checkable: true
                    onClicked: root.viewMode = "big"
                    Binding { target: bigButton; property: "checked"; value: root.viewMode === "big" }
                }
                Item { Layout.fillWidth: true }
                Button {
                    text: qsTr("Shutdown")
                    onClicked: shutdownDialog.visible = true
                }
                Button {
                    text: qsTr("Back")
                    onClicked: goBack()
                }
            }
        }

        Component {
            id: launcherItem

            Item {
                readonly property var item: launcherListView.model[index]

                width: launcherListView.cellWidth
                height: launcherListView.cellHeight

                Button {
                    anchors.fill: parent
                    anchors.margins: Screen.pixelDensity / 2

                    ColumnLayout {
                        anchors.fill: parent
                        anchors.margins: Screen.pixelDensity * 1.5

                        Item {
                            Layout.fillHeight: true
                            Layout.fillWidth: true

                            Image {
                                anchors.fill: parent
                                fillMode: Image.PreserveAspectFit
                                source: item.image !== undefined ? item.image.url : "qrc:Machinekit/Application/Controls/icons/machinekit"
                                visible: root.viewMode === "list" ? false : true
                                opacity: item.running ? 0.05 : 1.0
                            }

                            Label {
                                anchors.centerIn: parent
                                text:  item.terminating ? qsTr("Terminating...") : qsTr("Running...")
                                visible: item.running
                            }

                            StarButton {
                                anchors.left: parent.left
                                anchors.top: parent.top
                                width: root.viewMode === "list" ? parent.height : parent.width * 0.2
                                height: width
                                liked: item.importance !== 0
                                onClicked: applicationLauncher.setImportance(item.index, !item.importance)
                            }

                            Button {
                                anchors.right: parent.right
                                anchors.top: parent.top
                                text:  item.terminating ? qsTr("Kill") : qsTr("Terminate")
                                visible: item.running && (item.local === undefined)

                                onClicked: {
                                    if (item.terminating) {
                                       applicationLauncher.kill(item.index);
                                    }
                                    else {
                                       applicationLauncher.terminate(item.index);
                                    }
                                }
                            }
                        }

                        Label {
                            Layout.fillWidth: true
                            font.pointSize: dummyText.font.pointSize * (root.viewMode === "small" ? 1.0 : 1.3)
                            font.bold: true
                            horizontalAlignment: Text.AlignHCenter
                            verticalAlignment: Text.AlignVCenter
                            text: item.name
                            elide: Text.ElideRight
                        }
                    }

                    onClicked: {
                        if (!item.running) {
                            applicationLauncher.start(item.index);
                        }
                        selectedLauncher = item.index;
                        launcherSelected(index);
                    }
                }
            }
        }

        ScrollView {
            Layout.fillHeight: true
            Layout.fillWidth: true
            GridView {
                readonly property var launchers: root.__filterLaunchers(applicationLauncher.launchers)

                id: launcherListView
                cellWidth: width * (root.viewMode === "big" ? 0.333 : (root.viewMode === "small" ? 0.199 : 1.0))
                cellHeight: root.viewMode === "list" ? dummyButton.height * 3 : cellWidth
                clip: true
                model: launchers
                delegate: launcherItem

                BusyIndicator {
                    anchors.centerIn: parent
                    running: true
                    visible: (launcherListView.model.length === 0) || !applicationLauncher.synced
                    height: Math.min(root.width, root.height) * 0.15
                    width: height
                }
            }
        }
    }
}

