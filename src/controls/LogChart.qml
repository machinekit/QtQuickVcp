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
    \qmltype LogChart
    \inqmlmodule Machinekit.Controls
    \brief Provides chart for logging data.
    \ingroup machinekitcontrols

    The log chart combines \l ValueChart and \l ValueModel into one component.
    It is specially designed to log data from a data source providing only
    information about the current value.

    \qml
    LogChart {
        id: logChart
        value: myValueSource
    }
    \endqml

    \sa ValueChart, ValueModel
*/

ValueChart {
    /*! \qmlproperty double targetValue

        This property holds the target value that should be displayed in the chart.
    */
    property alias targetValue: valueModel.targetValue

    /*! \qmlproperty int maximumLogSize

        This property holds how many value entries should be stored as maximum.
        The model will remove the oldest entries if more data is added.
        Tuning this property may increase performance.

        The default value is \c{5000}.
    */
    property alias maximumLogSize: valueModel.maximumSize

    /*! \qmlproperty int sampleInterval

        This property holds the interval that the value pin should be sampled with in ms.

        The default value is \c{10}.
    */
    property alias sampleInterval: sampleTimer.interval

    /*! \qmlproperty int updateInterval

        This property holds the interval the chart should be update with in ms.

        The default value is \c{100}.
    */
    property alias updateInterval: updateTimer.interval

    /*! \qmlproperty bool autoUpdateEnabled

        This property holds wether the chart should be automatically updated or not.

        The default value is \c{true}.
    */
    property alias autoUpdate: updateTimer.running

    /*! \qmlproperty bool autoSamplingEnabled

        This property holds wether the value should be automatically sampled or not.

        The default value is \c{true}.
    */
    property alias autoSampling: sampleTimer.running

    /*! This property holds the value that should be sampled.
    */
    property double value: 0.0

    /*! Adds a data to chart.
    */
    function addData(value)
    {
        valueModel.addData(value);
    }

    /*! Clears all data in the chart.
    */
    function clearData()
    {
        valueModel.clearData();
    }

    id: chart
    width: 320
    height: 320
    implicitWidth: 320
    implicitHeight: 320

    valueModel: ValueModel {
        id: valueModel
    }

    Timer {
        id: sampleTimer

        interval: 10
        running: chart.enabled
        repeat: true
        onTriggered: {
            valueModel.addData(value);
        }
    }

    Timer {
        id: updateTimer

        interval: 100
        running: chart.enabled && chart.visible
        repeat: true
        onTriggered: {
            chart.update();
        }
    }
}
