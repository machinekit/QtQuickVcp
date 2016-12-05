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

/*!
    \qmltype ValueModel
    \inqmlmodule Machinekit.Controls
    \brief Provides a model to store values in combination with timestamps.
    \ingroup machinekitcontrols

    The value model may be used in combination with the \l{ValueChart}.

    \qml
    ValueModel {
        id: valueModel
        name: "My value model"
    }
    \endqml

    \sa ValueChart, LogChart
*/
ListModel {
    /*! This property holds the name of the value model.
    */
    property string name: ""

    /*! This property holds the timestamp of the first stored value.
    */
    property real startTimestamp

    /*! This property holds the timestamp of the last stored value.
    */
    property real endTimestamp

    /*! This property holds the highest value in the value model.
    */
    property double highestValue: 0.0

    /*! This property holds the lowest value in the value model.
    */
    property double lowestValue: 0.0

    /*! This property holds the current value of the value model.
    */
    property double currentValue: 0.0

    /*! This property holds the target value of the value model (e.g. for PID loops).

        The default value is \c{0}.
    */
    property double targetValue: 0.0

    /*! This property holds wether the value model is ready or not.
    */
    property bool ready: false

    /*! This property holds how many value entries should be stored as maximum.
        The model will remove the oldest entries if more data is added.
        Tuning this property may increase performance.

        The default value is \c{5000}.
    */
    property int maximumSize: 5000

    /*! This signal is emitted when new data is ready
    */
    signal dataReady

    id: model

    /*! This function return the nearest index in the value model to
        a given timestamp.
    */
    function indexOf(timestamp) {
        var start = model.get(0).timestamp;
        var end = model.get(model.count - 1).timestamp;
        if (end <= timestamp) {
            return model.count -1;
        }

        if (start >= timestamp) {
            return 0;
        }

        for (var i = 0; i < model.count; i++) {
            if (model.get(i).timestamp > timestamp) {
                return i-1;
            }
        }
        return -1;
    }

    /*! \internal */
    function createValue(value) {
        if (highestValue < value) {
            highestValue = value;
        }
        if (lowestValue > value) {
            lowestValue = value;
        }
        if (model.count === 0) {
            startTimestamp = Date.now();
        }
        endTimestamp = Date.now();
        currentValue = value;
        return {
                "timestamp": endTimestamp,
                "value":value
               };
    }

    /*! Clears all the data in the value model.
    */
    function clearData() {
        model.clear();
        model.ready = false;
    }

    /*! Adds one entry to the data model.
    */
    function addData(value)
    {
        model.append(createValue(value));
        if (model.count > maximumSize) {
            model.remove(0);
        }
        model.ready = true;
        model.dataReady(); //emit signal
    }
}
