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
#ifndef SERVICEDISCOVERYQUERY_H
#define SERVICEDISCOVERYQUERY_H

#include <QObject>
#include <qjdns.h>
#include <QQmlListProperty>
#include "servicediscoveryitem.h"
#include "servicediscoveryfilter.h"

namespace qtquickvcp {

class ServiceDiscoveryQuery : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QJDns::Type queryType READ queryType WRITE setQueryType NOTIFY queryTypeChanged)
    Q_PROPERTY(QString serviceType READ serviceType WRITE setServiceType NOTIFY serviceTypeChanged)
    Q_PROPERTY(QQmlListProperty<qtquickvcp::ServiceDiscoveryItem> items READ items NOTIFY itemsChanged)
    Q_PROPERTY(ServiceDiscoveryFilter *filter READ filter WRITE setFilter NOTIFY filterChanged)

public:
    explicit ServiceDiscoveryQuery(QObject *parent = 0);

    QQmlListProperty<ServiceDiscoveryItem> items();
    int itemCount() const;
    ServiceDiscoveryItem *item(int index) const;
    void setItems(QList<ServiceDiscoveryItem*> newServiceDiscoveryItems);

    QJDns::Type queryType() const
    {
        return m_queryType;
    }

    QString serviceType() const
    {
        return m_serviceType;
    }

    ServiceDiscoveryFilter * filter() const
    {
        return m_filter;
    }

public slots:
    void setQueryType(QJDns::Type type)
    {
        if (m_queryType == type)
            return;

        m_queryType = type;
        emit queryTypeChanged(type);
    }

    void setServiceType(QString serviceType)
    {
        if (m_serviceType == serviceType)
            return;

        m_serviceType = serviceType;
        emit serviceTypeChanged(serviceType);
    }

    void setFilter(ServiceDiscoveryFilter * filter)
    {
        if (m_filter == filter)
            return;

        m_filter = filter;
        emit filterChanged(filter);
    }

private:
    QJDns::Type m_queryType;
    QString m_serviceType;
    QList<ServiceDiscoveryItem *> m_items;
    ServiceDiscoveryFilter * m_filter;

signals:
    void queryTypeChanged(QJDns::Type queryType);
    void serviceTypeChanged(QString serviceType);
    void itemsChanged(QQmlListProperty<ServiceDiscoveryItem> arg);
    void filterChanged(ServiceDiscoveryFilter * filter);
}; // class ServiceDiscoveryQuery
}; // namespace qtquickvcp

#endif // SERVICEDISCOVERYQUERY_H
