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
// import all available plugins
import QtQuick 2.3
import QtQuick.Controls 1.2
import QtQuick.Controls.Styles 1.2
import QtQuick.Dialogs 1.2
import QtQuick.Window 2.0
import QtQuick.Layouts 1.1
import QtQuick.Controls.Styles 1.2
import QtQuick.Particles 2.0
import QtQuick.XmlListModel 2.0
import Machinekit.Controls 1.0
import Machinekit.HalRemote 1.0
import Machinekit.HalRemote.Controls 1.0
import Machinekit.VideoView 1.0
import Machinekit.PathView 1.0
import Machinekit.Application 1.0
import Machinekit.Application.Controls 1.0
import Machinekit.Service 1.0

Item {
    id: applicationWindow

    width: 500
    height: 700
    property string title: connectionWindow.title
    property Item toolBar: connectionWindow.toolBar
    property Item statusBar: connectionWindow.statusBar
    property MenuBar menuBar: connectionWindow.menuBar

    ConnectionWindow {
        id: connectionWindow

        anchors.fill: parent
        defaultTitle: qsTr("Machinekit Client") + " - " + Revision.name
        mode: "remote"
        remoteVisible: true
        localVisible: true
        autoSelectInstance: false
        autoSelectApplication: false
        instanceFilter { name: "" }
        applicationFilter { name: "" }

        ApplicationDescription {
            sourceDir: "./MachinekitClient.ServiceDisplay"
        }

        ApplicationDescription {
            sourceDir: "./MachinekitClient.LiveCoding"
        }
    }

    Component.onCompleted: {
        parseArguments(Qt.application.arguments);
    }

    function parseArguments(arguments)
    {
        var toggle = false;
        var option;
        var argument;
        var args = arguments.slice();
        args.shift();

        while (args.length > 0)
        {
            if (!toggle) {
                option = args[0];
            }
            else {
                argument = args[0];
                processOption(option, argument);
            }
            toggle = !toggle;
            args.shift();
        }
    }

    function processOption(option, argument)
    {
        if (option === '--config')
        {
            connectionWindow.configurationFilePath = argument;
        }
        else if (option === '--test-deployment') {
            if (argument === 'true') {
                Qt.quit();
            }
        }
    }
}
