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
import Machinekit.Application 1.0

LocalSettings {
    property var status: { "synced": false }
    property bool initialized: false
    property bool _ready: status.synced

    id: localSettings

    name: "settings"

    Component.onCompleted: {
        load()
    }
    Component.onDestruction: {
        save()
    }

    on_ReadyChanged: {
        if (_ready && !initialized) {
            for (var i = 0; i < status.config.axes; ++i) {
                setValue("axis" + i + ".jogVelocity", status.config.defaultVelocity, false)
            }
            setValue("dro.showOffsets", false, false)
            setValue("dro.showVelocity", true, false)
            setValue("dro.showDistanceToGo", true, false)
            setValue("preview.enable", false, false)
            setValue("preview.showMachineLimits", true, false)
            setValue("preview.showProgram", true, false)
            setValue("preview.showProgramRapids", true, false)
            setValue("preview.showLivePlot", true, false)
            setValue("preview.showTool", true, false)
            setValue("preview.showProgramExtents", true, false)
            setValue("preview.showMachineLimits", true, false)
            setValue("preview.alphaBlendProgram", false, false)
            setValue("preview.showCoordinate", true, false)
            setValue("mdi.history", [], false)
            initialized = true
        }
    }
}
