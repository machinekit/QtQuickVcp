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

ComboBox {
    property double distance: cbItems.get(jogCombo.currentIndex).distance

    id: jogCombo
    model: ListModel {
            id: cbItems
            ListElement { text: "Continous"; distance: 0 }
            ListElement { text: "1"; distance: 1 }
            ListElement { text: "0.1"; distance: 0.1 }
            ListElement { text: "0.05"; distance: 0.05 }
        }
    // TODO: should come from ini, maybe per axis
}
