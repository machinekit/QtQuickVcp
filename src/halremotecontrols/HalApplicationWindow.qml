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
import Machinekit.HalRemote 1.0
import Machinekit.HalRemote.Controls 1.0

/*!
    \qmltype HalApplicationWindow
    \inqmlmodule Machinekit.HalRemote.Controls
    \brief Provides a application window to simplify HAL application deployment.
    \ingroup halremotecontrols

    In order to create working HAL remote UI have to set \l title and \l name
    of the application window accordingly.

    \qml
    HalApplicationWindow {
        id: halApplicationWindow
        title: "My HAL application"
        name: "myhalapp"
    }
    \endqml
*/

Rectangle {
    /*! This property holds the title of the application window.
    */
    property string title: "HAL Application Template"

    /*! \qmlproperty string name

        This property holds the name of the HAL remote component that will
        contain all HAL pins created inside the window.
    */
    property alias name: remoteComponent.name

    /*! \qmlproperty string cmdUri

        This property holds the ZeroMQ command uri. It needs to be set in
        case \l autoDiscover is disabled.
    */
    property alias cmdUri: remoteComponent.cmdUri

    /*! \qmlproperty string updateUri

        This property holds the ZeroMQ update uri. It needs to be set in
        case \l autoDiscover is disabled.
    */
    property alias updateUri: remoteComponent.updateUri

    /*! \qmlproperty int heartbeadPeriod

        This property holds the period time of the heartbeat timer in ms.

        The default value is \c{3000}.
    */
    property alias heartbeatPeriod: remoteComponent.heartbeatPeriod

    /*! This property holds wether auto discovery of the ZeroMQ uri should be enabled or not.

        The default value is \c{true}.
    */
    property bool autoDiscover: true

    /*! This property holds wheter the application window is ready or not. If the
        property is set to true auto discovery and will automaticall start.

        The default value is \c{true}.
    */
    property bool ready: true

    /*! This property holds the services used by the application.
    */
    property list<Service> services: [
        Service {
            id: halrcompService
            type: Service.ST_STP_HALRCOMP
            minVersion: 0
        },
        Service {
            id: rcommandService
            type: Service.ST_HAL_RCOMMAND
            minVersion: 0
        }
    ]

    id: main

    width: 500
    height: 800
    color: systemPalette.window

    SystemPalette {
        id: systemPalette;
        colorGroup: enabled ? SystemPalette.Active : SystemPalette.Disabled
    }

    HalDiscoveryPage {
        id: discoveryPage

        anchors.fill: parent
        visible: false
        z: 100
        remoteComponent: remoteComponent
        halrcompService: halrcompService
        rcommandService: rcommandService
    }

    /* This timer is a workaround to make the discoveryPage invisible in QML designer */
    Timer {
        interval: 10
        repeat: false
        running: true
        onTriggered: discoveryPage.visible = true
    }

    state: remoteComponent.ready?"connected":"discovery"

    states: [
        State {
            name: "discovery"
            PropertyChanges { target: discoveryPage; opacity: 1.0 }
        },
        State {
            name: "connected"
            PropertyChanges { target: discoveryPage; opacity: 0.0 }
        }
    ]

    transitions: Transition {
            PropertyAnimation { duration: 500; properties: "opacity"; easing.type: Easing.InCubic }
        }

    HalRemoteComponent {
        id: remoteComponent

        name: main.name
        cmdUri: rcommandService.uri
        updateUri: halrcompService.uri
        heartbeatPeriod: 3000
        ready: main.autoDiscover?(halrcompService.ready && rcommandService.ready):main.ready
        containerItem: parent
    }
}
