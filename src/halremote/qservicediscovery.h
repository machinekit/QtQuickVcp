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
#ifndef QAPPDISCOVERY_H
#define QAPPDISCOVERY_H

#include <QQuickItem>
#include <qjdns.h>
#include <qjdnsshared.h>
#include "qservicediscoveryitem.h"
#include "qservice.h"
#include "qservicelist.h"
#include "qservicediscoveryfilter.h"

class QServiceDiscovery : public QQuickItem
{
    Q_OBJECT
    Q_INTERFACES(QQmlParserStatus)
    Q_PROPERTY(QString regType READ regType WRITE setRegType NOTIFY regTypeChanged)
    Q_PROPERTY(QString domain READ domain WRITE setDomain NOTIFY domainChanged)
    Q_PROPERTY(bool running READ running WRITE setRunning NOTIFY runningChanged)
    Q_PROPERTY(bool networkOpen READ isNetworkOpen NOTIFY networkOpenChanged)
    Q_PROPERTY(QServiceDiscoveryFilter *filter READ filter WRITE setFilter NOTIFY filterChanged)
    Q_PROPERTY(QQmlListProperty<QServiceList> serviceLists READ serviceLists)

public:
    explicit QServiceDiscovery(QQuickItem *parent = 0);

    virtual void componentComplete();

    QString regType() const
    {
        return m_regType;
    }

    bool running() const
    {
        return m_running;
    }

    bool isNetworkOpen() const
    {
        return m_networkOpen;
    }

    QString domain() const
    {
        return m_domain;
    }

    QServiceDiscoveryFilter *filter() const
    {
        return m_filter;
    }

    QQmlListProperty<QServiceList> serviceLists();
    int serviceListCount() const;
    QServiceList *serviceList(int index) const;

public slots:
    void setRegType(QString arg);
    void setDomain(QString arg);
    void setRunning(bool arg);
    void setFilter(QServiceDiscoveryFilter *arg);
    void updateServices();
    void updateFilter();

signals:
    void regTypeChanged(QString arg);
    void runningChanged(bool arg);
    void networkOpenChanged(bool arg);
    void domainChanged(QString arg);
    void filterChanged(QServiceDiscoveryFilter *arg);

private:
    QString m_regType;
    bool m_running;
    bool m_networkOpen;
    QString m_domain;
    QServiceDiscoveryFilter *m_filter;
    QList<QServiceList*> m_serviceLists;

    QNetworkSession *m_networkSession;
    QNetworkConfigurationManager *m_networkConfigManager;
    QTimer *m_networkConfigTimer; // Timer for refreshing the network status
    QElapsedTimer m_elapsedTimer;

    QJDns *m_jdns;
    QMap<int, int> m_queryTypeMap;
    QMap<int, QServiceDiscoveryItem*> m_queryItemMap;
    QMap<QString, QList<QServiceDiscoveryItem*> > m_serviceTypeMap;
    QMap<int, QString> m_queryServiceMap;
    QTimer *m_expiryCheckTimer;

    bool m_componentCompleted;

    void initializeNetworkSession();
    void startQueries();
    void stopQueries();
    void startQuery(QString type);
    void stopQuery(QString type);
    void addServiceType(QString type);
    void removeServiceType(QString type);
    void updateServiceType(QString type);
    void removeAllServiceTypes();
    void updateAllServiceTypes();
    static bool filterServiceDiscoveryItem(QServiceDiscoveryItem *item, QServiceDiscoveryFilter *serviceDiscoveryFilter);
    static QList<QServiceDiscoveryItem *> filterServiceDiscoveryItems(QList<QServiceDiscoveryItem *> serviceDiscoveryItems, QServiceDiscoveryFilter *primaryFilter, QServiceDiscoveryFilter *secondaryFilter);
    QServiceDiscoveryItem *addItem(QString name, QString type);
    QServiceDiscoveryItem *getItem(QString name, QString type);
    void updateItem(QString name, QString type);
    void removeItem(QString name, QString type);
    void clearItems(QString type);
    static QString composeSdString(QString type, QString domain);
    static QString composeSdString(QString subType, QString type, QString domain);

    private slots:
    void resultsReady(int id, const QJDns::Response &results);
    void error(int id, QJDns::Error e);
    void expiryCheck();
    void openNetworkSession();
    void updateNetConfig();
    void initializeMdns();
    void deinitializeMdns();
    void delayedInit();
};

#endif // QAPPDISCOVERY_H
