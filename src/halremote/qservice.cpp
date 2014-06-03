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
#include "qservice.h"

/*!
    \qmltype Service
    \instantiates QService
    \inqmlmodule Machinekit.HalRemote
    \brief A Machinekit service
    \ingroup halremote

    This component represents a Machninekit service. In combination
    with the \l ServiceDiscovery component it can be used to automatically
    resolve services running on a remote host.

    To find a specific service use the uuid \l ServiceDiscovery::filter for
    the \l ServiceDiscovery component and the name \l filter for this component.

    The following example demonstrates a service implementation for a video
    service.

    \qml
    Service {
        id: videoService

        type: "video"
        name: "Webcam1"
    }
    \endqml

    \sa ServiceList, ServiceDiscovery
*/

/*! \qmlproperty string Service::type

    This property holds the type of the service. If this property is set
    to an empty string all available services will be listed. In any other
    case the type is equivalent to a mDNS service subtype.
*/

/*! \qmlproperty string Service::name

    This property holds the name of the service.
*/

/*! \qmlproperty string Service::uri

    This property holds the uri of the service.
*/

/*! \qmlproperty string Service::uuid

    This property holds the uuid of the service. The uuid can be used to
    create a filter for filtering instances.
*/

/*! \qmlproperty int Service::version

    This property holds the version of the service. This property is \c 0
    if no version is specified within the service.
*/

/*! \qmlproperty bool Service::ready

    This property holds whether a service is ready or not. A service is
    ready if one or more \l items exist.

    The default value is \c{false}.
*/

/*! \qmlproperty ServiceDiscoveryFilter Service::filter

    This property holds the filter applied to all \l items.
    Filter criterias listed within a filter are AND connected during
    the filter process.

    This property can be used to filter all \l items for a specific name.
*/

/*! \qmlproperty list<ServiceDiscoveryItem> Service::items

    This property holds all discovered service instances.
*/

QService::QService(QObject *parent) :
    QObject(parent),
    m_type(""),
    m_name(""),
    m_uri(""),
    m_uuid(""),
    m_version(0),
    m_ready(false),
    m_filter(new QServiceDiscoveryFilter(this))
{
}

QQmlListProperty<QServiceDiscoveryItem> QService::items()
{
    return QQmlListProperty<QServiceDiscoveryItem>(this, m_items);
}

int QService::serviceDiscoveryItemCount() const
{
    return m_items.count();
}

QServiceDiscoveryItem *QService::serviceDiscoveryItem(int index) const
{
    return m_items.at(index);
}

void QService::setItems(QList<QServiceDiscoveryItem *> newServiceDiscoveryItems)
{
    m_items = newServiceDiscoveryItems;

    if (m_items.count() > 0)
    {
        m_uri = m_items.at(0)->uri();
        m_uuid = m_items.at(0)->uuid();
        m_name = m_items.at(0)->name();
        m_version = m_items.at(0)->version();
        m_ready = true;
    }
    else
    {
        m_uri = "";
        m_uuid = "";
        m_name = "";
        m_version = 0;
        m_ready = false;
    }

    emit uriChanged(m_uri);
    emit uuidChanged(m_uuid);
    emit nameChanged(m_name);
    emit versionChanged(m_version);
    emit readyChanged(m_ready);
    emit itemsChanged(items());
}
