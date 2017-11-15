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
import Machinekit.Application 1.0

ComboBox {
    property alias settings: appObject.settings
    property alias axis: handler.axis
    property alias continuousVisible: handler.continuousVisible
    property alias continuousText: handler.continuousText
    readonly property double distance: (currentIndex < handler.distanceModelReverse.length) ? handler.distanceModelReverse[currentIndex] : 0.0

    readonly property bool __ready: handler.settings.initialized

    id: root
    model: handler.incrementsModelReverse

    function __setIndex(index) {
        if (!__ready) {
            return;
        }
        root.settings.setValue("axis" + axis + ".jogIncrementSelection", index);
    }

    function __update() {
        if (!__ready) {
            return;
        }
        var value = root.settings.value("axis" + axis + ".jogIncrementSelection");
        if (value !== undefined) {
            root.currentIndex = value;
        }
    }

    onActivated: __setIndex(index)
    on__ReadyChanged: __update()
    onModelChanged: __update()

    /*! /internal
        Cannot directly connect to slots since the file property is var and not a QObject.
    */
    onSettingsChanged: {
        if (root.settings.onValuesChanged) {
            root.settings.onValuesChanged.connect(__update)
        }
    }

    ApplicationObject {
        id: appObject
    }

    JogDistanceHandler {
        id: handler
    }
}
