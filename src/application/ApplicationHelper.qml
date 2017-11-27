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
    readonly property bool ready: true

    readonly property var axisNamesUpper: getUpperAxisNames(axisNames)
    readonly property var axisNames: getAxisNames()
    readonly property var axisIndices: getAxisIndices()
    readonly property string timeUnits: getTimeUnits()
    readonly property string distanceUnits: getDistanceUnits()
    readonly property string machineUnits: getMachineUnits()
    readonly property double timeFactor: (timeUnits === "min") ? 60 : 1
    readonly property double distanceFactor: getDistanceFactor()

    function getDistanceFactor() {
        if(machineUnits === "mm") {
            return (distanceUnits === "mm") ? 1.0 : ((distanceUnits === "in") ? 0.0393700787 : 0.1);
        }
        else if (machineUnits == "in"){
            return (distanceUnits === "mm") ? 25.4 : ((distanceUnits === "in") ? 1.0 : 2.54);
        }
        else {
            return (distanceUnits === "mm") ? 10.0 : ((distanceUnits === "in") ? 0.393700787 : 1.0);
        }
    }

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

    function getMachineUnits() {
        if (status.synced) {
            switch (status.config.linearUnits) {
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

    function getAxisIndices() {
        if (status.synced) {
            var mask = status.config.axisMask;
            var indices = [];
            if (mask & 1) indices.push(0);
            if (mask & 2) indices.push(1);
            if (mask & 4) indices.push(2);
            if (mask & 8) indices.push(3);
            if (mask & 16) indices.push(4);
            if (mask & 32) indices.push(5);
            if (mask & 64) indices.push(6);
            if (mask & 128) indices.push(7);
            if (mask & 256) indices.push(8);
            return indices;
        }

        return [0, 1, 2, 3, 4, 5, 6, 7, 8];
    }
}
