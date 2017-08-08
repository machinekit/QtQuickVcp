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
#include "servicediscoveryfilter.h"

namespace qtquickvcp {

/*!
    \qmltype ServiceDiscoveryFilter
    \instantiates QServiceDiscoveryFilter
    \inqmlmodule Machinekit.HalRemote
    \brief Service discovery filter
    \ingroup halremote

    This component can be used to filter the results of the
    \l ServiceDiscovery component. All filter arguments are AND
    connected during the filter process. All filter arguments can
    contain Unix wildcards.

    The following example demonstrates how to use the filter for
    name and uuid.

    \qml
    ServiceDiscoveryFilter {
        id: filter

        name: "3D Printer"
        txtRecords: ["uuid=a7005ade-ea37-4f04-bf8a-7f45436beaca"]
    }
    \endqml


    \sa ServiceDiscovery, Service
*/

/*! \qmlproperty string ServiceDiscoveryFilter::name

    This property holds the name to filter the results for.
*/

/*! \qmlproperty list<string> ServiceDiscoveryFilter::txtRecords

    This property holds the TXT records to filter the results for.
*/

ServiceDiscoveryFilter::ServiceDiscoveryFilter(QObject *parent) :
    QObject(parent),
    m_name("")
{
}

QString ServiceDiscoveryFilter::name() const
{
    return m_name;
}

QStringList ServiceDiscoveryFilter::txtRecords() const
{
    return m_txtRecords;
}

void ServiceDiscoveryFilter::setName(const QString &arg)
{
    if (m_name != arg) {
        m_name = arg;
        emit nameChanged(arg);
    }
}

void ServiceDiscoveryFilter::setTxtRecords(const QStringList &arg)
{
    if (m_txtRecords != arg) {
        m_txtRecords = arg;
        emit txtRecordsChanged(arg);
    }
}
} // namespace qtquickvcp
