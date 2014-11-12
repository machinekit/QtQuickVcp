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
import QtQuick.Window 2.0

ApplicationWindow {
    id: appWindow

    visibility: (Qt.platform.os == "android") ? "FullScreen" : "AutomaticVisibility"
    visible: true
    width: (Qt.platform.os == "android") ? Screen.width : Screen.width * 0.7
    height: (Qt.platform.os == "android") ? Screen.height : Screen.height * 0.7
    title: (mainAppLoader.item != undefined) ? mainAppLoader.item.title : "Loading"
    toolBar: (mainAppLoader.item != undefined) ? mainAppLoader.item.toolBar : null
    statusBar: (mainAppLoader.item != undefined) ? mainAppLoader.item.statusBar : null
    menuBar: (mainAppLoader.item != undefined) ? mainAppLoader.item.menuBar : null

    // Loaders for the main application and the splash screen.
    Loader {
        id: mainAppLoader

        anchors.fill: parent
        onLoaded: {
            focus = true
            console.debug("Main application loaded.");
        }
        focus: true
         Keys.onPressed: {
             console.log("Loaded item captured:", event.text);
             event.accepted = true;
         }
    }

    Loader {
        id: splashScreenLoader
        source: Qt.resolvedUrl("SplashScreen.qml");
        width: parent.width
        height: parent.height
    }

    // Timers for starting to load the main application and eventually deleting
    // the splash screen.
    Timer {
        id: firstPhaseTimer
        property int phase: 0
        interval: 50
        running: true
        repeat: false

        onTriggered: {
            if (!mainAppLoader.Loading) {
                console.debug("Starting to load the main application.");
                mainAppLoader.source = Qt.resolvedUrl("main.qml");
                secondPhaseTimer.start();
            }
        }
    }
    Timer {
        id: secondPhaseTimer
        property int phase: 0
        interval: 200
        running: false
        repeat: true

        onTriggered: {
            if (phase == 0) {
                if (mainAppLoader.Loading) {
                    console.debug("The main application is not loaded yet.");
                    return;
                }

                console.debug("Finishing the splash screen progress bar.");

                if (splashScreenLoader.item) {
                    splashScreenLoader.item.loadingProgress = 1;
                }

                // Set the phase for deletion.
                phase += 1;
            }
            else if (phase == 1) {
                // Hide the splash screen.
                console.debug("Hiding the splash screen.");

                if (splashScreenLoader.item) {
                    splashScreenLoader.item.opacity = 0;
                }

                phase += 1;
            }
            else {
                // Delete the splash screen.
                console.debug("Deleting the splash screen.");

                // By setting the source property to an empty string destroys
                // the loaded item.
                splashScreenLoader.source = "";

                secondPhaseTimer.stop();
            }
        }
    }
}
