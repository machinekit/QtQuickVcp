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
#ifndef SERVICE_H
#define SERVICE_H

#include <QObject>
#include <QQmlListProperty>
#include "servicediscoveryitem.h"
#include "servicediscoveryfilter.h"
#include "servicediscoveryquery.h"

namespace qtquickvcp {

class Service : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString type READ type WRITE setType NOTIFY typeChanged)
    Q_PROPERTY(QString domain READ domain WRITE setDomain NOTIFY domainChanged)
    Q_PROPERTY(QString baseType READ baseType WRITE setBaseType NOTIFY baseTypeChanged)
    Q_PROPERTY(QString protocol READ protocol WRITE setProtocol NOTIFY protocolChanged)
    Q_PROPERTY(QString name READ name NOTIFY nameChanged)
    Q_PROPERTY(QString uri READ uri NOTIFY uriChanged)
    Q_PROPERTY(QString uuid READ uuid NOTIFY uuidChanged)
    Q_PROPERTY(int version READ version NOTIFY versionChanged)
    Q_PROPERTY(bool ready READ isReady NOTIFY readyChanged)
    Q_PROPERTY(ServiceDiscoveryFilter *filter READ filter WRITE setFilter NOTIFY filterChanged)
    Q_PROPERTY(QQmlListProperty<qtquickvcp::ServiceDiscoveryItem> items READ items NOTIFY itemsChanged)
    Q_PROPERTY(bool required READ required WRITE setRequired NOTIFY requiredChanged)
    Q_PROPERTY(QQmlListProperty<qtquickvcp::ServiceDiscoveryQuery> queries READ queries)

public:
    explicit Service(QObject *parent = 0);

    QQmlListProperty<ServiceDiscoveryItem> items();
    int itemCount() const;
    ServiceDiscoveryItem *item(int index) const;

    QQmlListProperty<ServiceDiscoveryQuery> queries();
    int queriesCount() const;
    ServiceDiscoveryQuery *query(int index) const;

    QString uri() const
    {
        return m_uri;
    }

    int version() const
    {
        return m_version;
    }

    bool isReady() const
    {
        return m_ready;
    }

    QString type() const
    {
        return m_type;
    }

    QString domain() const
    {
        return m_domain;
    }

    QString baseType() const
    {
        return m_baseType;
    }

    QString protocol() const
    {
        return m_protocol;
    }

    QString name() const
    {
        return m_name;
    }

    ServiceDiscoveryFilter *filter() const
    {
        return m_filter;
    }

    QString uuid() const
    {
        return m_uuid;
    }

    bool required() const
    {
        return m_required;
    }

public slots:
    void setType(QString arg)
    {
        if (m_type != arg) {
            m_type = arg;
            emit typeChanged(arg);
        }
    }

    void setDomain(QString domain)
    {
        if (m_domain == domain)
            return;

        m_domain = domain;
        emit domainChanged(domain);
    }

    void setBaseType(QString baseType)
    {
        if (m_baseType == baseType)
            return;

        m_baseType = baseType;
        emit baseTypeChanged(baseType);
    }

    void setProtocol(QString protocol)
    {
        if (m_protocol == protocol)
            return;

        m_protocol = protocol;
        emit protocolChanged(protocol);
    }

    void setFilter(ServiceDiscoveryFilter *arg)
    {
        if (m_filter != arg) {
            m_filter = arg;
            emit filterChanged(arg);

            m_serviceQuery->setFilter(m_filter); // pass the filter to the underlying query
        }
    }

    void setRequired(bool arg)
    {
        if (m_required == arg)
            return;

        m_required = arg;
        emit requiredChanged(arg);
    }

private:
    QString m_type;
    QString m_domain;
    QString m_baseType;
    QString m_protocol;
    QString m_name;
    QString m_uri;
    QString m_uuid;
    int m_version;
    bool m_ready;
    ServiceDiscoveryFilter *m_filter;
    QList<ServiceDiscoveryItem *> m_items;
    bool m_required;
    ServiceDiscoveryQuery *m_serviceQuery;
    QList<ServiceDiscoveryQuery *> m_queries;

    bool m_itemsReady;        // true when we have items
    QString m_rawUri;         // the raw uri from the items
    QString m_hostname;       // the hostname that is queried
    QString m_hostaddress;    // the address that was resolved

    const QString composeSdString(QString type, QString domain, QString protocol);
    const QString composeSdString(QString subType, QString type, QString domain, QString protocol);

private slots:
    void updateUri();
    void updateServiceQuery();
    void serviceQueryItemsUpdated(QQmlListProperty<ServiceDiscoveryItem> newItems);

signals:
    void uriChanged(QString arg);
    void versionChanged(int arg);
    void readyChanged(bool arg);
    void typeChanged(QString arg);
    void domainChanged(QString domain);
    void baseTypeChanged(QString baseType);
    void protocolChanged(QString protocol);
    void nameChanged(QString arg);
    void itemsChanged(QQmlListProperty<ServiceDiscoveryItem> arg);
    void filterChanged(ServiceDiscoveryFilter *arg);
    void uuidChanged(QString arg);
    void requiredChanged(bool arg);
    void queriesChanged();
}; // class Service
} // namespace qtquickvcp

#endif // SERVICE_H
