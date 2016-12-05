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
    \qmltype SlidePage
    \inqmlmodule Machinekit.Controls
    \brief Provides a page element for the \l SlideView.
    \ingroup machinekitcontrols

    \sa SlideView
*/


Rectangle {
    /*! The title of the page. Will be visible in the menu.
    */
    property string title: "SlidePage"

    /*! The source of the icon that will be displayed in the menu.
    */
    property string iconSource: ""

    anchors.fill: parent
    color: systemPalette.window
    opacity: 0
    enabled: opacity !== 0

    SystemPalette { id: systemPalette; colorGroup: SystemPalette.Active }

    Behavior on opacity { NumberAnimation { duration: 400; easing.type: Easing.OutQuad } }
}
