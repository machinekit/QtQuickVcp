/****************************************************************************
**
** Copyright (C) 2015 Alexander Rössler
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

ApplicationObject {
    property int axis: 0
    property bool continuousVisible: true
    property string continuousText: qsTr("Continuous")
    property var incrementsModel: continuousVisible ? [continuousText].concat(_incrementsModelBase) : _incrementsModelBase
    property var incrementsModelReverse: incrementsModel.slice(0).reverse()
    property var distanceModel: continuousVisible ? [0.0].concat(_incrementsModelBase) : _incrementsModelBase
    property var distanceModelReverse: distanceModel.slice(0).reverse()
    property var _incrementsModelBase: root.status.synced ? _parseIncrements(axis, status) : []

    id: root

    function _compareNumbers(a, b)
    {
        return a - b;
    }

    function _parseIncrements(axis, status) {
        var axisIncrements = undefined;
        if ((axis >= 0) && (axis < status.config.axes)) {
            axisIncrements = status.config.axis[axis].increments;
        }
        if ((axisIncrements === undefined) || (axisIncrements === "")) {
            axisIncrements = status.config.increments;
        }
        return axisIncrements.split(" ").sort(_compareNumbers);
    }
}

