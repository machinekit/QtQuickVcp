/****************************************************************************
**
** Copyright (C) 2014 Alexander Rössler
** License: LGPL version 2.1
**
** This file is part of QtQuickVcp.
**
** All rights reserved. This program and the accompanying materials
** are made available under the terms of the GNU Lesser General Public License
** (LGPL) version 2.1 which accompanies this distribution, and is available at
** http://www.gnu.org/licenses/lgpl-2.1.html
**
** This library is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
** Lesser General Public License for more details.
**
** Contributors:
** Alexander Rössler @ The Cool Tool GmbH <mail DOT aroessler AT gmail DOT com>
**
****************************************************************************/

import QtQuick 2.0
import QtQuick.Controls 1.2
import QtQuick.Window 2.0

Rectangle {
    default property alias data: container.data

    /*! This property holds the title of the service window.
    */
    property string title: "Window"

    /*! \qmlproperty bool ready

        This property holds wheter the service window is ready or not.
        Per default this property is set using the internal services
        if you want to overwrite the services set this property to \c true.
    */
    property bool ready: false

    /*! \qmlproperty list<Service> services

        This property holds the services used by the application.
    */
    property var services: []

    /*!
        \qmlproperty Item toolBar

        This property holds the toolbar \l Item.

        It can be set to any Item type, but is generally used with \l ToolBar.

        By default, this value is not set. When you set the toolbar item, it will
        be anchored automatically into the application window.
    */
    property Item toolBar

    /*!
        \qmlproperty Item statusBar

        This property holds the status bar \l Item.

        It can be set to any Item type, but is generally used with \l StatusBar.

        By default, this value is not set. When you set the status bar item, it
        will be anchored automatically into the application window.
    */
    property Item statusBar

    /*!
        \qmlproperty MenuBar menuBar

        This property holds the \l MenuBar.

        By default, this value is not set.
    */
    property MenuBar menuBar

    /*!
        \qmlproperty Item background

        This property holds the background \l Item.

        The background item is shown when service discovery is in progress.

        By default, this value references the default loading screen. When you set
        the background item it will be reparented to the ServiceWindow automatically.
    */
    property Item background: discoveryPage

    /*! \internal */
    property var _requiredServices: {
        var required = [];
        var newReady = true;
        for (var i = 0; i < services.length; ++i) {
            if (services[i].required) {
                required.push(services[i]);
                newReady = newReady && services[i].ready;
                services[i].onReadyChanged.connect(_evaluateReady);
            }
        }

        ready = newReady;  // if no required service we are ready

        return required;
    }

    /*!
        Updates the services of this window.
    */
    function updateServices() {
        var list = [];
        var nestedList = _recurseObjects(root.data, "Service");
        if (nestedList.length > 0) {
            list = list.concat(nestedList);
        }
        root.services = list;
    }

    /*! \internal */
    function _evaluateReady() {
        for (var i = 0; i < _requiredServices.length; ++i) {
            if (!_requiredServices[i].ready) {
                ready = false;
                return;
            }
        }

        ready = true;
        return;
    }

    /*! \internal */
    function _recurseObjects(objects, name)
    {
        var list = [];

        if (objects !== undefined) {
            for (var i = 0; i < objects.length; ++i)
            {
                if (objects[i].objectName === name) {
                    list.push(objects[i]);
                }
                var nestedList = _recurseObjects(objects[i].data, name);
                if (nestedList.length > 0) {
                    list = list.concat(nestedList);
                }
            }
        }

        return list;
    }

    /*! \qmlsignal disconnect

        Disconnects the service window if connected.
      */
    signal disconnect()
    /*! \qmlsignal disconnect

        Signals that the application is beeing shutdown.
      */
    signal shutdown()

    Component.onCompleted: {
       updateServices();
    }

    id: root
    color: systemPalette.window

    onBackgroundChanged: {
        if (background !== discoveryPage) {
            background.parent = root;
            background.z = 1000;
        }
    }

    SystemPalette {
        id: systemPalette;
        colorGroup: enabled ? SystemPalette.Active : SystemPalette.Disabled
    }

    Label {
        id: dummyText
    }

    Item {
        id: container
        anchors.fill: parent
    }

    /* loads the default discovery page if necessary */
    Loader {
        id: discoveryPage
        anchors.fill: parent
        visible: false
        z: 100
        sourceComponent: defaultDiscoveryPage
        active: background === discoveryPage

        Behavior on opacity {
            enabled: opacity === 1.0
            PropertyAnimation {
                duration: 500;
                properties: "opacity";
                easing.type: Easing.InCubic;
            }
        }

        /* This timer is a workaround to make the discoveryPage invisible in QML designer */
        Timer {
            interval: 10
            repeat: false
            running: discoveryPage.active
            onTriggered: {
                discoveryPage.visible = true;
            }
        }
    }

    /* the default discovery page, overlays the window as long as Machinetalk is not ready */
    Component {
        id: defaultDiscoveryPage

        Rectangle {
            id: discoveryPage
            color: systemPalette.window

            Button {
                anchors.right: parent.right
                anchors.top: parent.top
                anchors.margins: Screen.pixelDensity
                text: qsTr("Back")
                onClicked: root.disconnect()
            }

            Label {
                id: connectingLabel

                anchors.horizontalCenter: parent.horizontalCenter
                anchors.bottom: connectingIndicator.top
                anchors.bottomMargin: Screen.pixelDensity
                font.pointSize: dummyText.font.pointSize * 1.3
                text: qsTr("Waiting for services to appear...")
            }

            BusyIndicator {
                id: connectingIndicator

                anchors.centerIn: parent
                running: true
                height: Math.min(root.width, root.height) * 0.15
                width: height
            }

            Column {
                id: serviceCheckColumn
                anchors.horizontalCenter: parent.horizontalCenter
                anchors.top: connectingIndicator.bottom
                anchors.topMargin: Screen.pixelDensity

                Repeater {
                    model: root._requiredServices.length

                    CheckBox {
                        id: checkBox
                        text: qsTr("%1 service").arg(capitalizeFirstLetter(_requiredServices[index].type))
                        checked: _requiredServices[index].ready

                        function capitalizeFirstLetter(string) {
                            return string.charAt(0).toUpperCase() + string.slice(1);
                        }
                    }
                }
            }

            MouseArea {
                // steal the clicks from the check boxes
                anchors.fill: serviceCheckColumn
            }
        }
    }

    state: ready ? "connected" : "disconnected"

    states: [
        State {
            name: "disconnected"
            PropertyChanges { target: background; opacity: 1.0; enabled: true }
            PropertyChanges { target: container; visible: false; enabled: false }
        },
        State {
            name: "connected"
            PropertyChanges { target: background; opacity: 0.0; enabled: false }
            PropertyChanges { target: container; visible: true; enabled: true }
        }
    ]
}
