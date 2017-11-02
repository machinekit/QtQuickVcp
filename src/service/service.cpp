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
#include "service.h"

namespace qtquickvcp {

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

/*! \qmlproperty string Service::baseType

    This property holds the DNS SRV service type that the service discovery
    should browse.

    The default value is \c{"machinekit"}.
*/

/*! \qmlproperty string Service::domain

    This property holds the domain name that the service discovery should
    browse for services.

    The default value is \c{"local"}.
*/

/*! \qmlproperty string Service::protocol

    This property holds the protocol of the service. If no protocol is specified
    can use the service to discover hostnames. Common protocols are \c{"tcp"} and \c{"udp"}.

    The default value is \c{"tcp"}.
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

/*! \qmlproperty bool Service::required

    This property holds whether a service is required. This property is used
    to visualize that some services are not connected in the user interface.

    The default value is \c{false}.
*/

/*! \qmlproperty string Service::hostName

    This property holds the hostName of the computer where this service located at.
*/

/*! \qmlproperty string Service::hostAddress

    This property holds the hostAddress of the computer where this service located at.
*/

Service::Service(QObject *parent) :
    QObject(parent),
    m_type(""),
    m_domain("local"),
    m_baseType("machinekit"),
    m_protocol("tcp"),
    m_name(""),
    m_uri(""),
    m_uuid(""),
    m_version(0),
    m_ready(false),
    m_filter(new ServiceDiscoveryFilter(this)),
    m_required(false),
    m_serviceQuery(new ServiceDiscoveryQuery(this)),
    m_itemsReady(false),
    m_rawUri(""),
    m_hostName(""),
    m_hostAddress("")
{
    this->setObjectName("Service");

    m_serviceQuery->setQueryType(QJDns::Ptr);
    m_queries.append(m_serviceQuery);

    connect(m_serviceQuery, &ServiceDiscoveryQuery::queryTypeChanged,
            this, &Service::queriesChanged);
    connect(m_serviceQuery, &ServiceDiscoveryQuery::filterChanged,
            this, &Service::queriesChanged);

    connect(m_serviceQuery, &ServiceDiscoveryQuery::itemsChanged,
            this, &Service::serviceQueryItemsUpdated);

    connect(this, &Service::typeChanged,
            this, &Service::updateServiceQuery);
    connect(this, &Service::baseTypeChanged,
            this, &Service::updateServiceQuery);
    connect(this, &Service::protocolChanged,
            this, &Service::updateServiceQuery);
    connect(this, &Service::domainChanged,
            this, &Service::updateServiceQuery);

    updateServiceQuery();
}

QQmlListProperty<ServiceDiscoveryItem> Service::items()
{
    return QQmlListProperty<ServiceDiscoveryItem>(this, m_items);
}

int Service::itemCount() const
{
    return m_items.count();
}

ServiceDiscoveryItem *Service::item(int index) const
{
    return m_items.at(index);
}

void Service::serviceQueryItemsUpdated(QQmlListProperty<ServiceDiscoveryItem> newItems)
{
    m_items.clear();
    for (int i = 0; i < newItems.count(&newItems); ++i)
    {
        m_items.append(newItems.at(&newItems, i));
    }

    if (m_items.count() > 0)
    {
        m_rawUri = m_items.at(0)->uri();
        m_uuid = m_items.at(0)->uuid();
        m_name = m_items.at(0)->name();
        m_version = m_items.at(0)->version();
        m_hostName = m_items.at(0)->hostName();
        m_hostAddress = m_items.at(0)->hostAddress();

        m_itemsReady = true;
    }
    else
    {
        m_rawUri = "";
        m_uuid = "";
        m_name = "";
        m_version = 0;
        m_hostName = "";
        m_hostAddress = "";
        m_itemsReady = false;
    }

    emit uuidChanged(m_uuid);
    emit nameChanged(m_name);
    emit versionChanged(m_version);
    emit hostNameChanged(m_hostName);
    emit hostAddressChanged(m_hostAddress);
    emit itemsChanged(items());

    updateUri();
}

QQmlListProperty<ServiceDiscoveryQuery> Service::queries()
{
    return QQmlListProperty<ServiceDiscoveryQuery>(this, m_queries);
}

int Service::queriesCount() const
{
    return m_queries.count();
}

ServiceDiscoveryQuery *Service::query(int index) const
{
    return m_queries.at(index);
}

void Service::updateServiceQuery()
{
    m_serviceQuery->setServiceType(
                composeSdString(m_type,
                                m_baseType,
                                m_domain,
                                m_protocol));
}

const QString Service::composeSdString(const QString &type, const QString &domain, const QString &protocol)
{
    QString sdString = type;

    if (!protocol.isEmpty()) {
        sdString.prepend("_");
        sdString.append("._" + protocol);
    }
    sdString.append("." + domain);

    return sdString;
}

const QString Service::composeSdString(const QString &subType, const QString &type, const QString &domain, const QString &protocol)
{
    if (subType.isEmpty())
    {
        return composeSdString(type, domain, protocol);
    }
    else
    {
        return QString("_%1._sub._%2._%3.%4").arg(subType).arg(type).arg(protocol).arg(domain);
    }
}

/** Updates the uri and creates additional requests if necessary */
void Service::updateUri()
{
    if (!m_itemsReady)
    {
        m_ready = false;
        m_uri = "";
    }
    else
    {
        QUrl url = QUrl(m_rawUri);
        const QString &host = url.host();

        if (m_hostName.contains(host, Qt::CaseInsensitive))  // hostname is in form .local. and host in .local
        {
            url.setHost(m_hostAddress); // use resolved address
            m_uri = url.toString();
        }
        else
        {
            m_uri = m_rawUri;  // pass raw uri
        }
        m_ready = true;
    }

    emit uriChanged(m_uri);
    emit readyChanged(m_ready);
}
} // namespace qtquickvcp
