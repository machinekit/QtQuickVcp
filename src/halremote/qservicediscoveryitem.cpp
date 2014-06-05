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
#include "qservicediscoveryitem.h"

/*!
    \qmltype ServiceDiscoveryItem
    \instantiates QServiceDiscoveryItem
    \inqmlmodule Machinekit.HalRemote
    \brief Service discovery item
    \ingroup halremote

    This component is used to store information and data of resolved services.
    The properties \l{uuid}, \l{uri} and \l{version} are provided for
    convenience reasons and filled with the corresponding data from the
    \l txtRecords.

    \sa Service
*/

/*! \qmlproperty string ServiceDiscoveryItem::type

    This property holds the type of the service.
*/

/*! \qmlproperty string ServiceDiscoveryItem::name

    This property holds the name of the service.
*/

/*! \qmlproperty string ServiceDiscoveryItem::uri

    This property holds the uri of the service.
*/

/*! \qmlproperty string ServiceDiscoveryItem::uuid

    This property holds the uuid of the service. The uuid can be used to
    create a filter for filtering instances.
*/

/*! \qmlproperty int ServiceDiscoveryItem::version

    This property holds the version of the service. This property is \c 0
    if no version is specified within the service.
*/

/*! \qmlproperty int ServiceDiscoveryItem::port

    This property holds the port of the service.
*/

/*! \qmlproperty string ServiceDiscoveryItem::hostAddress

    This property holds the host address of the service.
*/

/*! \qmlproperty list<string> ServiceDiscoveryItem::txtRecords

    This property holds the TXT records of the service.
*/

QServiceDiscoveryItem::QServiceDiscoveryItem(QQuickItem *parent) :
    QQuickItem(parent),
    m_name(""),
    m_type(""),
    m_uri(""),
    m_uuid(""),
    m_port(0),
    m_hostAddress(QHostAddress()),
    m_txtRecords(QStringList()),
    m_outstandingRequests(0)
{
}

void QServiceDiscoveryItem::setTxtRecords(QStringList arg)
{
    if (m_txtRecords != arg) {
        m_txtRecords = arg;

        QString uri("");
        QString uuid("");
        int version(0);

        foreach (QString string, m_txtRecords)
        {
            QStringList keyValue;
            keyValue = string.split("=");

            if (keyValue.at(0) == "dsn")
            {
                uri = keyValue.at(1);
            }
            else if (keyValue.at(0) == "uuid")
            {
                uuid =keyValue.at(1);
            }
            else if (keyValue.at(0) == "version")
            {
                version = keyValue.at(1).toInt();
            }
        }

        setUri(uri);
        setUuid(uuid);
        setVersion(version);
        emit txtRecordsChanged(arg);
    }
}
