import QtQuick 2.1
import QtQuick.Controls 1.0
import Machinekit.HalRemote 1.0

ApplicationWindow {
    id: mainWindow

    visible: true
    title: appLoader.active ? appLoader.item.title : qsTr("MachineKit App Discover")
    width: 500
    height: 700

    Item {
        id: pageStack

        anchors.fill: parent

        Item {
            id: startPage

            anchors.fill: parent

            Text {
                id: dummyText
            }

            Text {
                id: pageTitleText

                anchors.top: parent.top
                anchors.horizontalCenter: parent.horizontalCenter
                anchors.topMargin: 10
                text: qsTr("Available Apps:")
                font.pointSize: dummyText.font.pointSize * 1.6
                font.bold: true
            }

            ListView {
                anchors.top: pageTitleText.bottom
                anchors.left: parent.left
                anchors.right: parent.right
                anchors.bottom: parent.bottom
                anchors.margins: 10
                spacing: 10

                model: appConfig.appConfigs
                delegate: Button {
                    anchors.left: parent.left
                    anchors.right: parent.right
                    height: mainWindow.height*0.1
                    Text {
                        id: titleText

                        anchors.centerIn: parent
                        anchors.verticalCenterOffset: -5
                        font.pointSize: descriptionText.font.pointSize*1.6
                        font.bold: true
                        text: name
                    }
                    Text {
                        id: descriptionText

                        anchors.horizontalCenter: parent.horizontalCenter
                        anchors.top: titleText.bottom
                        anchors.margins: 2
                        text: description
                    }

                    onClicked: appConfig.selectAppConfig(name)
                }
            }
        }

        Item {
            id: viewPage

            anchors.fill: parent

            Loader {
                id: appLoader

                anchors.fill: parent
                active: selectedConfig.loaded
                source: selectedConfig.mainFile

                onSourceChanged: {
                    console.log(source)
                    console.log(active)
                }

                onLoaded: console.log("loaded")
            }
        }

        state: (appLoader.active) ?"loaded":"discovery"

        states: [
            State {
                name: "discovery"
                PropertyChanges { target: startPage; opacity: 1.0 }
                PropertyChanges { target: viewPage; opacity: 0.0 }
            },
            State {
                name: "loaded"
                PropertyChanges { target: startPage; opacity: 0.0 }
                PropertyChanges { target: viewPage; opacity: 1.0 }
            }
        ]

        transitions: Transition {
                PropertyAnimation { duration: 500; properties: "opacity"; easing.type: Easing.InCubic }
            }

        // Capture the Android Back key and backspace key
        // on the desktop tp go back in the application
        // focus needs to be true to capture key events
        focus: true
        Keys.onReleased: {
                if ((event.key == Qt.Key_Back) ||
                        (event.key == Qt.Key_Backspace)) {
                    if (pageStack.state == "discovery")
                    {
                        Qt.quit()
                    }
                    else if (pageStack.state == "loaded")
                    {
                        appConfig.unselectAppConfig()
                    }

                    event.accepted = true
                }
            }
    }

    AppConfig {
        id: appConfig

        uri: "tcp://192.168.1.18:5590"
        timeout: 3000
        ready: true
        filters: [ AppConfigFilter { type: AppConfigItem.QT5_QML },
                   AppConfigFilter { type: AppConfigItem.GLADEVCP } ]
        selectedConfig: AppConfigItem {
            id: selectedConfig
        }

    }
}
