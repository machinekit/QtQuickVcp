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
import QtQuick.Controls 1.1
import Machinekit.HalRemote 1.0
import Machinekit.HalRemote.Controls 1.0

ApplicationWindow {
    id: applicationWindow

    visible: true
    width: 500
    height: 700
    title: connectionWindow.title

    ConnectionWindow {
        id: connectionWindow

        anchors.fill: parent
        autoSelectInstance: false
        applicationSource: "qrc:/qml/PidctrlDemo.qml"
        instanceFilter: ServiceDiscoveryFilter{ name: "" }
    }
}
