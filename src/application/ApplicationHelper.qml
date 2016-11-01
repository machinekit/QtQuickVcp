/****************************************************************************
**
** Copyright (C) 2016 Alexander Rössler
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
****************************************************************************/
import QtQuick 2.0
import Machinekit.Application 1.0

Item {
    property var status: { "synced": false }
    property bool ready: true

    property var axisNamesUpper: getUpperAxisNames(axisNames)
    property var axisNames: getAxisNames()
    property string timeUnits: getTimeUnits()
    property string distanceUnits: getDistanceUnits()
    property double timeFactor: (timeUnits === "min") ? 60 : 1
    property double distanceFactor: (distanceUnits === "mm") ? 1.0 : ((distanceUnits === "in") ? 0.03937 : 0.1)

    function getTimeUnits() {
        if (status.synced) {
            switch (status.config.timeUnits) {
            case ApplicationStatus.TimeUnitsMinute:
                return "min";
            case ApplicationStatus.TimeUnitsSecond:
                return "s";
            }
        }

        return "min";
    }

    function getDistanceUnits() {
        if (status.synced) {
            switch (status.interp.programUnits) {
            case ApplicationStatus.CanonUnitsInch:
                return "in";
            case ApplicationStatus.CanonUnitsMm:
                return "mm";
            case ApplicationStatus.CanonUnitsCm:
                return "cm";
            }
        }

        return "mm";
    }

    function getAxisNames() {
        if (status.synced) {
            var mask = status.config.axisMask;
            var labels = [];
            if (mask & 1) labels.push("x");
            if (mask & 2) labels.push("y");
            if (mask & 4) labels.push("z");
            if (mask & 8) labels.push("a");
            if (mask & 16) labels.push("b");
            if (mask & 32) labels.push("c");
            if (mask & 64) labels.push("u");
            if (mask & 128) labels.push("v");
            if (mask & 256) labels.push("w");
            return labels;
        }

        return ["x", "y", "z", "a", "b", "c", "u", "v", "w"];
    }

    function getUpperAxisNames(names) {
        if (status.synced) {
            var mask = status.config.axisMask;
            var labels = [];
            if (mask & 1) labels.push("X");
            if (mask & 2) labels.push("Y");
            if (mask & 4) labels.push("Z");
            if (mask & 8) labels.push("A");
            if (mask & 16) labels.push("B");
            if (mask & 32) labels.push("C");
            if (mask & 64) labels.push("U");
            if (mask & 128) labels.push("V");
            if (mask & 256) labels.push("W");
            return labels;
        }

        return ["X", "Y", "Z", "A", "B", "C", "U", "V", "W"];
    }
}
