import QtQuick 2.1
import QtQuick.Controls 1.1
import QtQuick.Layouts 1.1
import QtQuick.Window 2.0
import Machinekit.Controls 1.0
import Machinekit.Service 1.0

Item {
    property bool autoSelectInstance: false
    property var instances: []
    property var serviceDiscovery: {
        "lookupMode": ServiceDiscovery.MulticastDNS,
        "networkReady": true
    }
    readonly property bool networkReady: serviceDiscovery.networkReady

    signal nameServersChanged()
    signal instanceSelected(string uuid)

    onInstancesChanged: {
        if (!d.evaluateAutoSelection()) {
            d.assignInstances();
        }
    }

    id: root
    width: 1000
    height: 800

    QtObject {
        id: d
        property var localInstances: []

        function evaluateAutoSelection() {
            if (visible && (autoSelectInstance === true) && (instances.length > 0))
            {
                var uuid = instances[0].uuid;
                if (uuid !== "") {
                    autoSelectionTimer.stop();
                    autoSelectionTimer.uuid = uuid;
                    autoSelectionTimer.start();
                    return true;
                }
            }
            return false;
        }

        function assignInstances() {
            d.localInstances = root.instances;
        }

    }

    Timer {
        property string uuid: ""
        id: autoSelectionTimer
        interval: 10
        repeat: false
        onTriggered: instanceSelected(uuid)
    }

    Button {
        id: dummyButton
        visible: false
    }

    Label {
        id: dummyText
        visible: false
    }

    Component {
        id: instanceListView

        ListView {
            id: listView
            spacing: Screen.pixelDensity
            clip: true

            model: d.localInstances
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
                        text: hostName + " - " + hostAddress
                        color: systemPalette.dark
                        horizontalAlignment: Text.AlignHCenter
                        wrapMode: Text.WordWrap
                    }
                }

                onClicked: instanceSelected(root.instances[index].uuid)
            }

            Item {
                visible: d.localInstances.length === 0
                anchors.fill: parent

                BusyIndicator {
                    anchors.centerIn: parent
                    running: true
                    height: Math.min(root.width, root.height) * 0.15
                    width: height
                }

                MouseArea {
                    anchors.fill: parent
                    onClicked: listView.forceActiveFocus()
                }
            }
        }
    }

    SlideView {
        id: discoveryView
        anchors.fill: parent

        onCurrentIndexChanged: {
            if (currentIndex === 0) {
                serviceDiscovery.lookupMode = ServiceDiscovery.MulticastDNS;
            }
            else {
                serviceDiscovery.lookupMode = ServiceDiscovery.UnicastDNS;
            }
        }

        Binding {
            target: discoveryView; property: "currentIndex";
            value: (serviceDiscovery.lookupMode === ServiceDiscovery.MulticastDNS) ? 0 : 1
        }

        SlidePage {
            title: qsTr("Multicast")

            ColumnLayout {
                anchors.fill: parent
                anchors.margins: Screen.pixelDensity
                spacing: Screen.pixelDensity

                Label {
                    id: pageTitleText2

                    Layout.fillWidth: true
                    Layout.preferredHeight: Math.max(dummyButton.height, implicitHeight)
                    visible: root.networkReady
                    text: qsTr("Available Instances:")
                    font.pointSize: dummyText.font.pointSize * 1.3
                    font.bold: true
                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignVCenter
                    wrapMode: Text.WordWrap
                }

                Loader {
                    Layout.fillHeight: true
                    Layout.fillWidth: true
                    visible: root.networkReady
                    sourceComponent: instanceListView
                    active: true
                }

                Item {
                    Layout.fillHeight: true
                    Layout.fillWidth: true
                    visible: !root.networkReady

                    Label {
                        anchors.centerIn: parent
                        wrapMode: Text.WordWrap
                        horizontalAlignment: Text.AlignHCenter
                        font.pointSize: dummyText.font.pointSize * 1.1
                        text: qsTr("Warning!<br>No network connection found, service discovery unavailable. Please check your network connection.")
                    }
                }
            }
        }

        SlidePage {
            id: unicastPage
            title: qsTr("Unicast")

            ColumnLayout {
                anchors.fill: parent
                anchors.margins: Screen.pixelDensity
                spacing: Screen.pixelDensity

                Label {
                    Layout.fillWidth: true
                    Layout.preferredHeight: Math.max(dummyButton.height, implicitHeight)
                    text: qsTr("Available Instances:")
                    font.pointSize: dummyText.font.pointSize * 1.3
                    font.bold: true
                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignVCenter
                    wrapMode: Text.WordWrap
                }

                Loader {
                    Layout.fillHeight: true
                    Layout.fillWidth: true
                    sourceComponent: instanceListView
                    active: true
                }

                Label {
                    Layout.fillWidth: true
                    text: qsTr("Machinekit Instances:")
                    font.pointSize: dummyText.font.pointSize * 1.3
                    font.bold: true
                    horizontalAlignment: Text.AlignHCenter
                    wrapMode: Text.WordWrap
                }

                ListView {
                    id: dnsServerView
                    Layout.fillWidth: true
                    Layout.preferredHeight: dummyButton.height * 1.5 * count + Screen.pixelDensity * 1.5 * Math.max(count-1, 0)
                    spacing: Screen.pixelDensity

                    model: serviceDiscovery.nameServers

                    delegate: RowLayout {
                                id: viewItem
                                anchors.left: parent.left
                                anchors.right: parent.right
                                height: dummyButton.height * 1.5

                                Label {
                                    text: qsTr("Instance %1:").arg(index + 1)
                                    font.pointSize: dummyText.font.pointSize * 1.2
                                }

                                TextField {
                                    id: dnsServerTextField
                                    Layout.fillWidth: true
                                    Layout.fillHeight: true
                                    font.pointSize: dummyText.font.pointSize * 1.2
                                    placeholderText: qsTr("IP address or hostname")
                                    onEditingFinished: {
                                        dnsServerView.model[index].hostName = text;
                                        serviceDiscovery.updateNameServers();
                                        root.nameServersChanged();

                                        root.forceActiveFocus();   // remove the focus
                                    }

                                    Binding {
                                        target: dnsServerTextField;
                                        property: "text";
                                        value: (dnsServerView.model[index] !== null) ? dnsServerView.model[index].hostName : ""
                                    }
                                }

                                Button {
                                    Layout.fillHeight: true
                                    text: (dnsServerTextField.text !== "") ? "+" : "-"
                                    visible: (index === (dnsServerView.model.length - 1)) && (index < 2)   // last item, limited to 3 items due to bug => TODO
                                    onClicked: {
                                        root.forceActiveFocus();   // accept changes on text edit

                                        if (dnsServerTextField.text && dnsServerView.model[index].hostName)
                                        {
                                            var nameServerObject = nameServerComponent.createObject(root, {});
                                            serviceDiscovery.addNameServer(nameServerObject);
                                        }
                                    }

                                    Label {
                                        anchors.fill: parent
                                        font.pointSize: dummyText.font.pointSize*1.2
                                        font.bold: true
                                        horizontalAlignment: Text.AlignHCenter
                                        verticalAlignment: Text.AlignVCenter
                                        text: "+"
                                    }
                                }
                            }
                }

                Button {
                    Layout.fillWidth: true
                    Layout.preferredHeight: dummyButton.height * 1.5
                    visible: dnsServerView.count === 0
                    onClicked: {
                        serviceDiscovery.addNameServer(nameServerComponent.createObject(root, {}))
                    }

                    Label {
                        anchors.fill: parent
                        font.pointSize: dummyText.font.pointSize*1.2
                        font.bold: true
                        horizontalAlignment: Text.AlignHCenter
                        verticalAlignment: Text.AlignVCenter
                        text: "+"
                    }
                }
            }
        }
    }
}

