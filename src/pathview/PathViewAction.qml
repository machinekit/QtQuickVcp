/****************************************************************************
**
** Copyright (C) 2017 Alexander Rössler
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
** Alexander Rössler <alexander AT roessler DOT systems>
**
****************************************************************************/

import QtQuick 2.0
import QtQuick.Controls 1.2

Action {
    property var core: null
    property var gcodeProgramLoader: core === null ? {} : core.gcodeProgramLoader
    property var gcodeProgramModel: core === null ? null : core.gcodeProgramModel
    property var previewClient: core === null ? { connected: false } : core.previewClient

    Component.onCompleted: {
        if (core === null)
        {
            try {
                var x = pathViewCore;
                core = Qt.binding(function() { return x; });
            }
            catch (err) {
            }
        }
    }
}
