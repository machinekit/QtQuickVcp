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
#ifndef QSERVICEDISCOVERYQUERY_H
#define QSERVICEDISCOVERYQUERY_H

#include <QObject>
#include <qjdns.h>
#include <QQmlListProperty>
#include "qservicediscoveryitem.h"
#include "qservicediscoveryfilter.h"

class QServiceDiscoveryQuery : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QJDns::Type queryType READ queryType WRITE setQueryType NOTIFY queryTypeChanged)
    Q_PROPERTY(QString serviceType READ serviceType WRITE setServiceType NOTIFY serviceTypeChanged)
    Q_PROPERTY(QQmlListProperty<QServiceDiscoveryItem> items READ items NOTIFY itemsChanged)
    Q_PROPERTY(QServiceDiscoveryFilter *filter READ filter WRITE setFilter NOTIFY filterChanged)

public:
    explicit QServiceDiscoveryQuery(QObject *parent = 0);

    QQmlListProperty<QServiceDiscoveryItem> items();
    int itemCount() const;
    QServiceDiscoveryItem *item(int index) const;
    void setItems(QList<QServiceDiscoveryItem*> newServiceDiscoveryItems);

    QJDns::Type queryType() const
    {
        return m_queryType;
    }

    QString serviceType() const
    {
        return m_serviceType;
    }

    QServiceDiscoveryFilter * filter() const
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

        m_items.clear();
        emit itemsChanged(items());
    }

    void setServiceType(QString serviceType)
    {
        if (m_serviceType == serviceType)
            return;

        m_serviceType = serviceType;
        emit serviceTypeChanged(serviceType);

        m_items.clear();
        emit itemsChanged(items());
    }

    void setFilter(QServiceDiscoveryFilter * filter)
    {
        if (m_filter == filter)
            return;

        m_filter = filter;
        emit filterChanged(filter);

        m_items.clear();
        emit itemsChanged(items());
    }

private:
    QJDns::Type m_queryType;
    QString m_serviceType;
    QList<QServiceDiscoveryItem *> m_items;
    QServiceDiscoveryFilter * m_filter;

signals:
    void queryTypeChanged(QJDns::Type queryType);
    void serviceTypeChanged(QString serviceType);
    void itemsChanged(QQmlListProperty<QServiceDiscoveryItem> arg);
    void filterChanged(QServiceDiscoveryFilter * filter);
};

#endif // QSERVICEDISCOVERYQUERY_H
