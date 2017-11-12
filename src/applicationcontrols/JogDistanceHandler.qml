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
    property var incrementsModel: continuousVisible ? _incrementsModelBase.increments.concat([continuousText]) : _incrementsModelBase
    property var incrementsModelReverse: incrementsModel.slice(0).reverse()
    property var distanceModel: continuousVisible ? _incrementsModelBase.distances.concat([0.0]) : _incrementsModelBase
    property var distanceModelReverse: distanceModel.slice(0).reverse()
    property var _incrementsModelBase: _ready ? _parseIncrements(axis, status) : { increments: [], distances: [] }
    property double _linearUnits: _ready ? status.config.linearUnits : 1.0
    property bool _ready: status.synced

    id: root

    function _compareNumbers(a, b)
    {
        return a - b;
    }

    function _fromInternalLinearUnit(scale) {
        var units = root._linearUnits * 25.4
        return units * scale;
    }

    function _parseIncrementItem(increment) {
        var scale;
        if (increment.endsWith("mm")) {
            scale = _fromInternalLinearUnit(1.0 / 25.4);
            increment = increment.slice(0, increment.length - 2);
        }
        else if (increment.endsWith("cm")) {
            scale = _fromInternalLinearUnit(10.0 / 25.4);
            increment = increment.slice(0, increment.length - 2);
        }
        else if (increment.endsWith("um")) {
            scale = _fromInternalLinearUnit(0.001 / 25.4);
            increment = increment.slice(0, increment.length - 2);
        }
        else if (increment.endsWith("in")) {
            scale = _fromInternalLinearUnit(1.0);
            increment = increment.slice(0, increment.length - 2);
        }
        else if (increment.endsWith("inch")) {
            scale = _fromInternalLinearUnit(1.0);
            increment = increment.slice(0, increment.length - 4);
        }
        else if (increment.endsWith("mil")) {
            scale = _fromInternalLinearUnit(0.001);
            increment = increment.slice(0, increment.length - 3);
        }
        else {
            scale = 1.0;
        }

        increment = increment.trim();
        if (increment.indexOf("/") !== -1) {
            var items = increment.split("/");
            increment = Number(items[0].trim()) / Number(items[1].trim());
        }
        else {
            increment = Number(increment);
        }

        return increment * scale;
    }

    function _parseIncrements(axis, status) {
        /* load per axis increments or generic increments */
        var increments = undefined;
        if ((axis >= 0) && (axis < status.config.axes)) {
            increments = status.config.axis[axis].increments;
        }
        if ((increments === undefined) || (increments === "")) {
            increments = status.config.increments;
        }

        /* axis increments can have the form "1 2 3 4" or "1cm, 3mm, 4in" */
        if (increments.indexOf(",") !== -1) {
            increments = increments.split(",");
            increments = increments.map(function (item) { return item.trim(); });
        }
        else {
            increments = increments.split(" ");
        }

        /* convert the elements to numbers */
        var distances = increments.map(_parseIncrementItem);

        return { increments: increments, distances: distances };
    }
}

