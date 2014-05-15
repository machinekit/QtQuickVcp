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
import QtQuick.Controls 1.1
import QtQuick.Layouts 1.0
import Machinekit.HalRemote.Controls 1.0

/*!
    \qmltype HalDiscoveryPage
    \inqmlmodule Machinekit.HalRemote.Controls
    \brief Discovery page for the HalApplicationWindow.
    \ingroup halremotecontrols
    \internal
*/

Rectangle {
    /*! This property holds the HAL remote component.
    */
    property variant remoteComponent

    /*! This property holds the HAL remote component service.
    */
    property variant halrcompService

    /*! This property holds the HAL remote command service.
    */
    property variant rcommandService

    id: main

    width: 500
    height: 800
    color: systemPalette.window

    SystemPalette {
        id: systemPalette;
        colorGroup: enabled ? SystemPalette.Active : SystemPalette.Disabled
    }

    ColumnLayout {
        anchors.margins: 10
        anchors.fill: parent
        spacing: 5

        Text {
            id: sStateText

            Layout.fillWidth: true
            text: qsTr("sState: ") + remoteComponent.sState
        }

        Text {
            id: cStateText

            Layout.fillWidth: true
            text: qsTr("cState: ") + remoteComponent.cState
        }

        RadioButton {
            id: halrcompServiceCheck

            Layout.fillWidth: true
            text: qsTr("ST_STP_HALRCOMP command discovered")
            checked: halrcompService.ready
        }

        RadioButton {
            id: rcommandServiceCheck

            Layout.fillWidth: true
            text: qsTr("ST_HAL_RCOMMAND command discovered")
            checked: rcommandService.ready
        }
    }
}
