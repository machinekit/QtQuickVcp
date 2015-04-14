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
    property alias core: object.core
    property alias status: object.status

    property double distance: _distanceModel[root.currentIndex]
    property bool showContinous: true
    property string continousText: qsTr("Continous")

    property var _incrementsModel: showContinous ? [continousText].concat(_incrementsModelBase) : _incrementsModelBase
    property var _incrementsModelBase: status.synced ? status.config.increments.split(" ") : []
    property var _distanceModel: {
        var distanceModel = []
        for (var i = 0; i < _incrementsModel.length; ++i) {
            distanceModel.push(_incrementsModel[i] === continousText ? 0 : _incrementsModel[i])
        }
        return distanceModel
    }

    id: root
    model: _incrementsModel

    ApplicationObject {
        id: object
    }
}
