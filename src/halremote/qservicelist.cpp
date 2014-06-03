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
#include "qservicelist.h"

/*!
    \qmltype ServiceList
    \instantiates QServiceList
    \inqmlmodule Machinekit.HalRemote
    \brief A list of services
    \ingroup halremote

    This component is used to represent a list of \l{Service}s for the
    use with the \l ServiceDiscovery component. Use QML bindings to
    add and remove services at runtime.

    \qml
    Item {
        property list<Service> services: [
            Service {
                id: halrcompService
                type: "halrcomp"
            }
            ]

        id: root

        ServiceList {
            id: serviceList
        }

        function bindServices() {
            serviceList.services = Qt.binding( function() { return root.services } )
        }

        function unbindServices() {
            serviceList.services = []
        }
    }
    \endqml

    \sa Service, ServiceDiscovery
*/

QServiceList::QServiceList(QObject *parent) :
    QObject(parent)
{
}

QQmlListProperty<QService> QServiceList::services()
{
    return QQmlListProperty<QService>(this, m_services);
}

int QServiceList::serviceCount() const
{
    return m_services.count();
}

QService *QServiceList::service(int index) const
{
    return m_services.at(index);
}
