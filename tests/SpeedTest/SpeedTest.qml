import QtQuick 2.0
import QtQuick.Controls 1.1
import QtQuick.Layouts 1.1
import QtQuick.Window 2.0
import Machinekit.HalRemote 1.0
import Machinekit.HalRemote.Controls 1.0

Rectangle {
    property bool testRunning: false
    property int testMode: testModeCombo.currentIndex
    property int minTimeout: minTimeoutSpin.value
    property int maxTimeout: maxTimeoutSpin.value
    property int n: numberSpin.value
    property int i: 0
    property double timestamp: 0
    property var results: []

    property list<Service> services: [
        Service {
            id: halrcompService
            type: "halrcomp"
        },
        Service {
            id: halrcmdService
            type: "halrcmd"
        }
    ]

    property string title: qsTr("SpeedTest")


    id: main

    color: systemPalette.window

    function startTest() {
        i = 0
        results = []
        testRunning = true
        testTimer.start()
    }

    function stopTest() {
        var outputText = ""

        testRunning = false

        for (var j = 0; j < results.length; j += 2)
        {
            outputText += (results[j]+1) + "\t" + results[j+1] + "\n"
        }

        outputTextArea.text = outputText
    }

    SystemPalette {
        id: systemPalette;
        colorGroup: enabled ? SystemPalette.Active : SystemPalette.Disabled
    }

    Item {
        id: containerItem1

        HalPin {
            id: testPin1
            name: "test"
            type: HalPin.Bit
            direction: HalPin.Out

            onSyncedChanged: {
                if (!(testRunning && (testMode == 0)))
                    return

                if (!synced) {
                    timestamp = Date.now()
                }
                else
                {
                    var newTimestamp = Date.now()
                    var diff = newTimestamp - timestamp

                    results.push(i)
                    results.push(diff)

                    i++
                    if (i != n) {
                        testTimer.interval = Math.random() * (maxTimeout-minTimeout) + minTimeout
                        testTimer.restart()
                    }
                    else
                        stopTest()
                }
            }

            onValueChanged: {
                if (!(testRunning && (testMode == 1)))
                    return

                timestamp = Date.now()
            }
        }
    }

    Item {
        id: containerItem2

        HalPin {
            id: testPin2
            name: "test"
            type: HalPin.Bit
            direction: HalPin.In

            onValueChanged: {
                if (!(testRunning && (testMode == 1)))
                    return

                var newTimestamp = Date.now()
                var diff = newTimestamp - timestamp

                results.push(i)
                results.push(diff)

                i++
                if (i != n) {
                    testTimer.interval = Math.random() * (maxTimeout-minTimeout) + minTimeout
                    testTimer.restart()
                }
                else
                    stopTest()
            }
        }
    }

    HalRemoteComponent {
        id: remoteComponent1

        name: "speedtest1"
        halrcmdUri: halrcmdService.uri
        halrcompUri: halrcompService.uri
        heartbeatPeriod: 3000
        ready: (halrcompService.ready && halrcmdService.ready) || (state === HalRemoteComponent.Connected)
        containerItem: containerItem1
    }

    HalRemoteComponent {
        id: remoteComponent2

        name: "speedtest2"
        halrcmdUri: halrcmdService.uri
        halrcompUri: halrcompService.uri
        heartbeatPeriod: 3000
        ready: (halrcompService.ready && halrcmdService.ready) || (state === HalRemoteComponent.Connected)
        containerItem: containerItem2
    }

    Timer {
        id: testTimer

        interval: maxTimeout
        repeat: false
        running: false

        onTriggered: testPin1.value = !testPin1.value
    }

    ColumnLayout {
        anchors.fill: parent
        anchors.margins: Screen.logicalPixelDensity * 2
        visible: halrcompService.ready && halrcmdService.ready

        RowLayout {
            Layout.fillWidth: true

            Label {
                text: qsTr("Progress:")
            }

            ProgressBar {
                Layout.fillWidth: true

                minimumValue: 0
                maximumValue: n
                value: i

                Label {
                    anchors.fill: parent
                    horizontalAlignment: "AlignHCenter"
                    verticalAlignment: "AlignVCenter"
                    text: i + "/" + n
                }
            }
        }

        TextArea {
            id: outputTextArea

            Layout.fillWidth: true
            Layout.fillHeight: true
            text: qsTr("Test 1 tests the delay time between a pin change and pin update \n" +
                       "Test 2 tests the time between a pin change on one component and the pin change on another component")
        }

        RowLayout {
            Layout.fillWidth: true

            Label {
                Layout.fillWidth: true
                text: qsTr("Type:")
            }

            ComboBox  {
                id: testModeCombo

                model: [ "Test1", "Test2" ]
            }
        }

        RowLayout {
            Layout.fillWidth: true

            Label {
                Layout.fillWidth: true
                text: qsTr("Number of measurements")
            }

            SpinBox {
                id: numberSpin

                minimumValue: 1
                maximumValue: 999999
                value: 10
            }
        }

        RowLayout {
            Layout.fillWidth: true

            Label {
                Layout.fillWidth: true
                text: qsTr("Minimum timeout:")
            }

            SpinBox {
                id: minTimeoutSpin

                minimumValue: 1
                maximumValue: 99999
                value: 10
            }
        }

        RowLayout {
            Layout.fillWidth: true

            Label {
                Layout.fillWidth: true
                text: qsTr("Maximum timeout:")
            }

            SpinBox {
                id: maxTimeoutSpin

                minimumValue: 1
                maximumValue: 99999
                value: 100
            }
        }

        Button {
            id: startButton

            Layout.fillWidth: true
            text: qsTr("start")
            onClicked: startTest()
        }
    }
}
