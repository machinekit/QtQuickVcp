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
import Machinekit.Application 1.0

ApplicationItem {
    property int decimals: (_distanceUnits === "mm") ? 3 : 4
    property int digits: (_distanceUnits === "mm") ? 4 : 3
    property string prefix: ""
    property string suffix: ""
    property int axes: axisNames.length
    property var axisHomed: _ready ? status.motion.axis : [{"homed":false}, {"homed":false}, {"homed":false}, {"homed":false}]
    property var axisNames: helper.ready ? helper.axisNamesUpper : ["X", "Y", "Z", "A"]
    property var g5xNames: ["G54", "G55", "G56", "G57", "G58", "G59", "G59.1", "G59.2", "G59.3"]
    property int g5xIndex: _ready ? status.motion.g5xIndex : 1
    property var position: _getPosition()
    property var dtg: _ready ? _scalePosition(status.motion.dtg) : {"x":0.0, "y":0.0, "z":0.0, "a":0.0, "b":0.0, "c":0.0, "u":0.0, "v":0.0, "w":0.0}
    property var g5xOffset: _ready ? _scalePosition(status.motion.g5xOffset) : {"x":0.0, "y":0.0, "z":0.0, "a":0.0, "b":0.0, "c":0.0, "u":0.0, "v":0.0, "w":0.0}
    property var g92Offset: _ready ? _scalePosition(status.motion.g92Offset) : {"x":0.0, "y":0.0, "z":0.0, "a":0.0, "b":0.0, "c":0.0, "u":0.0, "v":0.0, "w":0.0}
    property var toolOffset: _ready ? _scalePosition(status.io.toolOffset) : {"x":0.0, "y":0.0, "z":0.0, "a":0.0, "b":0.0, "c":0.0, "u":0.0, "v":0.0, "w":0.0}
    property double rotationXy: _ready ? status.motion.rotationXy : 0.0
    property bool lathe: _ready ? status.config.lathe : false
    property double velocity: _ready ? status.motion.currentVel * _timeFactor * _distanceFactor : 0.0
    property double distanceToGo: _ready ? status.motion.distanceToGo * _distanceFactor : 0.0
    property bool offsetsVisible: settings.initialized && settings.values.dro.showOffsets
    property bool velocityVisible: settings.initialized && settings.values.dro.showVelocity
    property bool distanceToGoVisible: settings.initialized && settings.values.dro.showDistanceToGo
    property int positionFeedback: _ready ? status.config.positionFeedback : ApplicationStatus.ActualPositionFeedback
    property int positionOffset: _ready ? status.config.positionOffset : ApplicationStatus.RelativePositionOffset

    readonly property bool _ready: status.synced
    readonly property var _axisNames: helper.ready ? helper.axisNames : ["x", "y", "z", "a"]
    readonly property var _axisIndices: helper.ready ? helper.axisIndices : [0, 1, 2, 3]
    readonly property double _timeFactor: helper.ready ? helper.timeFactor : 1
    readonly property double _distanceFactor: helper.ready ? helper.distanceFactor : 1
    readonly property string _distanceUnits: helper.ready ? helper.distanceUnits: "mm"

    function _scalePosition(pos)
    {
        var newPos = {};
        for (var i = 0; i < axes; ++i) {
            var axisName = _axisNames[i];
            newPos[axisName] = pos[axisName] * _distanceFactor;
        }
        return newPos;
    }

    function _getPosition() {
        var basePosition;
        if (_ready) {
            basePosition = (positionFeedback === ApplicationStatus.ActualPositionFeedback) ? status.motion.actualPosition : status.motion.position;
            basePosition = _scalePosition(basePosition);
        }
        else {
            basePosition = { "x":0.0, "y":0.0, "z":0.0, "a":0.0, "b":0.0, "c":0.0, "u":0.0, "v":0.0, "w":0.0 };
        }

        if (positionOffset === ApplicationStatus.RelativePositionOffset) {
            for (var i = 0; i < axes; ++i) {
                var axisName = _axisNames[i];
                basePosition[axisName] -= g5xOffset[axisName] + g92Offset[axisName] + toolOffset[axisName];
            }
        }

        return basePosition;
    }

    id: root
}
