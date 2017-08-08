/****************************************************************************
**
** Copyright (C) 2015 Alexander Rössler
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
#include "servicediscoveryquery.h"

namespace qtquickvcp {

ServiceDiscoveryQuery::ServiceDiscoveryQuery(QObject *parent) : QObject(parent),
    m_queryType(QJDns::Ptr),
    m_serviceType(""),
    m_filter(new ServiceDiscoveryFilter(this))
{

}

QQmlListProperty<ServiceDiscoveryItem> ServiceDiscoveryQuery::items()
{
    return QQmlListProperty<ServiceDiscoveryItem>(this, m_items);
}

int ServiceDiscoveryQuery::itemCount() const
{
    return m_items.count();
}

ServiceDiscoveryItem *ServiceDiscoveryQuery::item(int index) const
{
    return m_items.at(index);
}

void ServiceDiscoveryQuery::setItems(QList<ServiceDiscoveryItem *> newServiceDiscoveryItems)
{
    m_items = newServiceDiscoveryItems;

    emit itemsChanged(items());
}

QJDns::Type ServiceDiscoveryQuery::queryType() const
{
    return m_queryType;
}

QString ServiceDiscoveryQuery::serviceType() const
{
    return m_serviceType;
}

ServiceDiscoveryFilter *ServiceDiscoveryQuery::filter() const
{
    return m_filter;
}

void ServiceDiscoveryQuery::setQueryType(QJDns::Type type)
{
    if (m_queryType == type) {
        return;
    }

    m_queryType = type;
    emit queryTypeChanged(type);
}

void ServiceDiscoveryQuery::setServiceType(const QString &serviceType)
{
    if (m_serviceType == serviceType) {
        return;
    }

    m_serviceType = serviceType;
    emit serviceTypeChanged(serviceType);
}

void ServiceDiscoveryQuery::setFilter(ServiceDiscoveryFilter *filter)
{
    if (m_filter == filter) {
        return;
    }

    m_filter = filter;
    emit filterChanged(filter);
}
} // namepsace qtquickvcp
