import QtQuick 2.1
import QtQuick.Controls 1.1
import QtQuick.Layouts 1.1
import QtQuick.Window 2.0
import Machinekit.Controls 1.0

Item {
    property bool instanceSelected: false
    property bool autoSelectApplication: false
    property bool localVisible: true
    property bool remoteVisible: true
    property string mode: "local"
    property var configService: QtObject { property string name: "Test on XYZ" }
    property var applications: []

    property var _listModel: root.instanceSelected ? root.applications : 0

    signal applicationSelected(int index)
    signal goBack()

    function _evaluateAutoSelection() {
        if ((autoSelectApplication === true) && (_listModel.length > 0) && configService.ready)
        {
            applicationSelected(0);
        }
    }

    id: root

    Connections {
        target: configService
        ignoreUnknownSignals: true
        onReadyChanged: _evaluateAutoSelection()
    }

    Label {
        id: dummyText
        visible: false
    }
    Button {
        id: dummyButton
        visible: false
    }

    Component {
        id: localRemoteContent

        ColumnLayout {
            spacing: Screen.pixelDensity

            Label {
                id: pageTitleText

                Layout.fillWidth: true
                Layout.preferredHeight: Math.max(dummyButton.height, implicitHeight)
                text: configService.name
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

            ListView {
                Layout.fillWidth: true
                Layout.fillHeight: true
                spacing: Screen.pixelDensity
                clip: true

                model: _listModel
                delegate: Button {
                    anchors.left: parent.left
                    anchors.right: parent.right
                    height: dummyButton.height * 3

                    ColumnLayout {
                        anchors.left: parent.left
                        anchors.right: parent.right
                        anchors.verticalCenter: parent.verticalCenter

                        Label {
                            id: titleText

                            Layout.fillWidth: true
                            font.pointSize: dummyText.font.pointSize*1.3
                            font.bold: true
                            text: name
                            horizontalAlignment: Text.AlignHCenter
                            elide: Text.ElideRight
                        }
                        Label {
                            id: descriptionText

                            Layout.fillWidth: true
                            text: description
                            color: systemPalette.dark
                            horizontalAlignment: Text.AlignHCenter
                            wrapMode: Text.WordWrap
                        }
                    }

                    onClicked: applicationSelected(index)
                }

                onCountChanged: _evaluateAutoSelection()

                BusyIndicator {
                    anchors.centerIn: parent
                    running: true
                    visible: (root.mode === "remote") && !applicationConfig.synced
                    height: Math.min(root.width, root.height) * 0.15
                    width: height
                }
            }
        }
    }

    SlideView {
        id: appView
        anchors.fill: parent

        onCurrentIndexChanged: {
            if (currentIndex === 0)
                root.mode = "remote";
            else
                root.mode = "local";
        }

        Binding {
            target: appView; property: "currentIndex";
            value: ((root.mode === "remote") ? 0 : 1)
        }

        SlidePage {
            anchors.fill: parent
            anchors.margins: Screen.pixelDensity
            title: qsTr("Remote UIs")
            visible: root.remoteVisible

            Loader {
                anchors.fill: parent
                sourceComponent: localRemoteContent
                active: true
            }
        }

        SlidePage {
            anchors.fill: parent
            anchors.margins: Screen.pixelDensity
            title: qsTr("Local UIs")
            visible: root.localVisible

            Loader {
                anchors.fill: parent
                sourceComponent: localRemoteContent
                active: true
            }
        }
    }
}

