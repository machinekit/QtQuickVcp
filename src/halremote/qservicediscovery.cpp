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
#include "qservicediscovery.h"

/*!
    \qmltype ServiceDiscovery
    \instantiates QServiceDiscovery
    \inqmlmodule Machinekit.HalRemote
    \brief Service discovery component
    \ingroup halremote

    This component can be used to discover and resolve available services
    on the network. The technology behind the service discovery
    is Multicast DNS (mDNS) in combination with DNS Service Discovery (DNS-SD),
    therefore, service discovery should work on any local network.

    mDNS names are generated using the \l serviceType and \l domain properties
    as well as the \l Service::type property from the service. This results in
    the following scheme:
    _\l{Service::type}._sub._\l{serviceType}._tcp.\l{domain}

    The following example demonstrates how the service discovery can be used
    to resolve and display available config services.

    \qml
    Item {
        ListView {
            id: listView

            anchors.fill: parent
            model: configService.items
            delegate: Label {
                        anchors.left: parent.left
                        anchors.right: parent.right
                        text: name
                    }
        }

        ServiceDiscovery {
            id: serviceDiscovery

            serviceType: "machinekit"
            domain:      "local"
            running:     true

            serviceLists: [
                ServiceList {
                    services: [
                        Service {
                            id: configService

                            type: "config"
                        }
                    ]
                }
            ]
        }
    }
    \endqml

    \note To change services and filter at runtime it is necessary to
    run the update functions for these properties.

    \sa Service, ServiceList, ServiceDiscoveryFilter, ServiceDiscoveryItem
*/

/*! \qmlproperty string ServiceDiscovery::serviceType

    This property holds the DNS SRV service type that the service discovery
    should browse.

    The default value is \c{"machinekit"}.
*/

/*! \qmlproperty string ServiceDiscovery::domain

    This property holds the domain name that the service discovery should
    browse for services.

    The default value is \c{"local"}.
*/

/*! \qmlproperty bool ServiceDiscovery::running

    This property holds whether the service discovery is running or not.
    If the service discovery is set to not running the list of discovered
    services will be cleared.

    The default value is \c{false}.
*/

/*! \qmlproperty bool ServiceDiscovery::networkOpen

    This property holds whether a suitable network connection is
    available or not. The \l running property will have no effect
    until a network connection is available

    The default value is \c{false}.
*/

/*! \qmlproperty ServiceDiscoveryFilter ServiceDiscovery::filter

    This property holds the filter applied to all discovered services.
    Filter criterias listed within a filter are AND connected during
    the filter process.

    This property can be used to filter all services for a specific uuid.

    \note It is necessary to run \l updateFilter after modifying the filter.
*/

/*! \qmlproperty list<ServiceList> ServiceDiscovery::serviceLists

    This property holds a list of service lists. Typically the service
    lists are provided by other components such as application windows
    and connected via bindings at run-time.

    \note It is necessary to run \l updateServices after modifying the services.
*/

/*! \qmlmethod void ServiceDiscovery::updateServices()

    Updates the \l{serviceLists}. Needs to be executed after modifying
    the \l serviceLists property.
*/

/*! \qmlmethod void ServiceDiscovery::updateFilter()

    Updates the \l{filter}. Needs to be executed after modifying
    the \l filter property.
*/


QServiceDiscovery::QServiceDiscovery(QQuickItem *parent) :
    QQuickItem(parent),
    m_componentCompleted(false),
    m_serviceType("machinekit"),
    m_domain("local"),
    m_running(false),
    m_networkOpen(false),
    m_filter(new QServiceDiscoveryFilter(this)),
    m_networkSession(NULL),
    m_networkConfigManager(NULL),
    m_networkConfigTimer(new QTimer(this)),
    m_elapsedTimer(QElapsedTimer()),
    m_jdns(NULL),
    m_delayedInitRunning(false)
{
}

/** componentComplete is executed when the QML component is fully loaded */
void QServiceDiscovery::componentComplete()
{
    m_componentCompleted = true;
    m_elapsedTimer.start();

    initializeNetworkSession();

    QQuickItem::componentComplete();
}

void QServiceDiscovery::initializeNetworkSession()
{
    // now begin the process of opening the network link
    m_networkConfigManager = new QNetworkConfigurationManager;
    connect(m_networkConfigManager, SIGNAL(updateCompleted()),
            this, SLOT(openNetworkSession()));
    m_networkConfigManager->updateConfigurations();
    // update the connections cyclically
    m_networkConfigTimer->setInterval(5000);
    connect(m_networkConfigTimer, SIGNAL(timeout()),
            this, SLOT(updateNetConfig()));
    m_networkConfigTimer->start();
}

void QServiceDiscovery::initializeMdns()
{
    if (m_jdns != NULL) // already initialized
    {
        return;
    }

#ifdef QT_DEBUG
            qDebug() << "Initializing MDNS";
#endif
    m_jdns = new QJDns(this);


    connect(m_jdns, SIGNAL(resultsReady(int,QJDns::Response)),
            this, SLOT(resultsReady(int,QJDns::Response)));
    connect(m_jdns, SIGNAL(error(int,QJDns::Error)),
            this, SLOT(error(int,QJDns::Error)));

    m_jdns->init(QJDns::Multicast, QHostAddress::Any);

    m_networkOpen = true;
    if (m_running == true)
    {
        updateServices();
    }

    emit networkOpenChanged(m_networkOpen);

    m_delayedInitRunning = false;
}

void QServiceDiscovery::deinitializeMdns()
{
#ifdef QT_DEBUG
            qDebug() << "Deinitializing MDNS";
#endif
    m_networkOpen = false;
    emit networkOpenChanged(m_networkOpen);

    if (m_running)
    {
        removeAllServiceTypes();
        m_queryItemMap.clear();
        m_queryServiceMap.clear();
        m_queryTypeMap.clear();
    }

    m_jdns->deleteLater();
    m_jdns = NULL;
}

void QServiceDiscovery::delayedInit()
{
    if (m_delayedInitRunning)
    {
        return;
    }
    m_delayedInitRunning = true;

    // for some reason there needs to be a delay if the network is initialized after the app was started
    // if the network comes up 500 msecs or less after the application has started skip the delay
    if (m_elapsedTimer.hasExpired(1000))
    {
        QTimer::singleShot(2000, this, SLOT(initializeMdns()));
    }
    else
    {
        initializeMdns();
    }
}

QQmlListProperty<QServiceList> QServiceDiscovery::serviceLists()
{
    return QQmlListProperty<QServiceList>(this, m_serviceLists);
}

int QServiceDiscovery::serviceListCount() const
{
    return m_serviceLists.count();
}

QServiceList *QServiceDiscovery::serviceList(int index) const
{
    return m_serviceLists.at(index);
}

void QServiceDiscovery::setServiceType(QString arg)
{
    if (m_serviceType != arg) {

        if (m_running && m_networkOpen) {
            stopQueries();
        }

        m_serviceType = arg;
        emit serviceTypeChanged(arg);

        if (m_running && m_networkOpen) {
            startQueries();
        }
    }
}

void QServiceDiscovery::setDomain(QString arg)
{
    if (m_domain != arg) {

        if (m_running && m_networkOpen) {
            stopQueries();
        }

        m_domain = arg;
        emit domainChanged(arg);

        if (m_running && m_networkOpen) {
            startQueries();
        }
    }
}

void QServiceDiscovery::setRunning(bool arg)
{
    if (m_running != arg) {
        m_running = arg;
        emit runningChanged(arg);

        if (!m_networkOpen) {
            return;
        }

        if (m_running) {
            startQueries();
        }
        else
        {
            stopQueries();
        }
    }
}

void QServiceDiscovery::updateServices()
{
    QMap<QString, QList<QServiceDiscoveryItem*> > oldServiceTypeMap;

    oldServiceTypeMap = m_serviceTypeMap;

    // Iterate through all services and update all service types
    foreach (QServiceList *serviceList, m_serviceLists)
    {
        for (int i = 0; i < serviceList->serviceCount(); ++i)
        {
            QService *service;

            service = serviceList->service(i);
            addServiceType(service->type());
            oldServiceTypeMap.remove(service->type());
            if (m_running && m_networkOpen)
            {
                startQuery(service->type());
            }
        }
    }

    // Iterate trough all item that are left and remove them
    QMapIterator<QString, QList<QServiceDiscoveryItem*> > i(oldServiceTypeMap);
    while (i.hasNext()) {
        i.next();

        if (m_running && m_networkOpen)
        {
            stopQuery(i.key());
        }
        removeServiceType(i.key());
    }
}

void QServiceDiscovery::updateFilter()
{
    updateAllServiceTypes();
}

void QServiceDiscovery::setFilter(QServiceDiscoveryFilter *arg)
{
    if (m_filter != arg) {
        m_filter = arg;
        emit filterChanged(arg);

        updateAllServiceTypes();
    }
}

void QServiceDiscovery::startQueries()
{
    QMapIterator<QString, QList<QServiceDiscoveryItem*> > i(m_serviceTypeMap);
    while (i.hasNext()) {
        i.next();
        startQuery(i.key());
    }
}

void QServiceDiscovery::stopQueries()
{
    QMapIterator<QString, QList<QServiceDiscoveryItem*> > i(m_serviceTypeMap);
    while (i.hasNext()) {
        i.next();
        stopQuery(i.key());
    }
}

void QServiceDiscovery::startQuery(QString type)
{
    int queryId;

    QMapIterator<int, QString> i(m_queryServiceMap);
    while (i.hasNext()) {
        i.next();
        if (i.value() == type)  // query with the type already running
        {
            return;
        }
    }

    queryId = m_jdns->queryStart(composeSdString(type, m_serviceType, m_domain).toLocal8Bit(), QJDns::Ptr);
    m_queryTypeMap.insert(queryId, QJDns::Ptr);
    m_queryServiceMap.insert(queryId, type);
}

void QServiceDiscovery::stopQuery(QString type)
{
    int queryId;
    bool found;

    QMapIterator<int, QString> i(m_queryServiceMap);
    while (i.hasNext()) {
        i.next();
        if (i.value() == type)  // query type matching
        {
            found = true;
            queryId = i.key();
            break;
        }
    }

    if (!found)
    {
        return;
    }

    m_jdns->queryCancel(queryId);
    m_queryTypeMap.remove(queryId);
    m_queryServiceMap.remove(queryId);
    clearItems(type);
}

void QServiceDiscovery::addServiceType(QString type)
{
    QList<QServiceDiscoveryItem*> serviceDiscoveryItems;

    if (m_serviceTypeMap.contains(type))
    {
        return;
    }

    m_serviceTypeMap.insert(type, serviceDiscoveryItems);
}

void QServiceDiscovery::removeServiceType(QString type)
{
    if (!m_serviceTypeMap.contains(type))
    {
        return;
    }

    clearItems(type);
    m_serviceTypeMap.remove(type);
}

void QServiceDiscovery::updateServiceType(QString type)
{
    QList<QServiceDiscoveryItem*> serviceDiscoveryItems;

    if (m_serviceTypeMap.contains(type))
    {
        serviceDiscoveryItems = m_serviceTypeMap.value(type);
    }
    else
    {
        return;
    }

    // Iterate through all services and update all service types suitable for the announcement
    foreach (QServiceList *serviceList, m_serviceLists)
    {
        for (int i = 0; i < serviceList->serviceCount(); ++i)
        {
            QService *service;

            service = serviceList->service(i);

            if (service->type() == type)
            {
                service->setItems(filterServiceDiscoveryItems(serviceDiscoveryItems, m_filter, service->filter()));
            }
        }
    }
}

void QServiceDiscovery::removeAllServiceTypes()
{
    QMap<QString, QList<QServiceDiscoveryItem*> > serviceTypeMap = m_serviceTypeMap;
    QMapIterator<QString, QList<QServiceDiscoveryItem*> > i(serviceTypeMap);
    while (i.hasNext()) {
        i.next();
        removeServiceType(i.key());
    }
}

void QServiceDiscovery::updateAllServiceTypes()
{
    QMapIterator<QString, QList<QServiceDiscoveryItem*> > i(m_serviceTypeMap);
    while (i.hasNext()) {
        i.next();
        updateServiceType(i.key());
    }
}

bool QServiceDiscovery::filterServiceDiscoveryItem(QServiceDiscoveryItem *item, QServiceDiscoveryFilter *serviceDiscoveryFilter)
{
    if (!((serviceDiscoveryFilter->name() == "") || item->name().contains(QRegExp(serviceDiscoveryFilter->name(), Qt::CaseSensitive, QRegExp::WildcardUnix))))
    {
        return false;
    }
    if (!serviceDiscoveryFilter->txtRecords().isEmpty())
    {
        QStringList txtRecords = item->txtRecords();

        foreach (QString filter, serviceDiscoveryFilter->txtRecords())
        {
            txtRecords = txtRecords.filter(QRegExp(filter, Qt::CaseSensitive, QRegExp::WildcardUnix));
        }
        if (txtRecords.isEmpty())
        {
            return false;
        }
    }

    return true;
}

QList<QServiceDiscoveryItem *> QServiceDiscovery::filterServiceDiscoveryItems(QList<QServiceDiscoveryItem *> serviceDiscoveryItems, QServiceDiscoveryFilter *primaryFilter, QServiceDiscoveryFilter *secondaryFilter)
{
    QList<QServiceDiscoveryItem*> newServiceDiscoveryItems;

    foreach (QServiceDiscoveryItem *item, serviceDiscoveryItems)
    {
        if (filterServiceDiscoveryItem(item, primaryFilter)
                && filterServiceDiscoveryItem(item, secondaryFilter))
        {
            newServiceDiscoveryItems.append(item);
        }
    }

    return newServiceDiscoveryItems;
}

QServiceDiscoveryItem *QServiceDiscovery::addItem(QString name, QString type)
{
    QList<QServiceDiscoveryItem*> serviceDiscoveryItems;

    if (m_serviceTypeMap.contains(type))
    {
        serviceDiscoveryItems = m_serviceTypeMap.value(type);
    }
    else
    {
        return NULL;
    }

    foreach (QServiceDiscoveryItem *item, serviceDiscoveryItems)
    {
        if (item->name() == name)
        {
            return item;
        }
    }

    QServiceDiscoveryItem *item = new QServiceDiscoveryItem();
    item->setName(name);
    item->setType(type);
    serviceDiscoveryItems.append(item);
    m_serviceTypeMap.insert(type, serviceDiscoveryItems);

    return item;
}

QServiceDiscoveryItem *QServiceDiscovery::getItem(QString name, QString type)
{
    QList<QServiceDiscoveryItem*> serviceDiscoveryItems;

    if (m_serviceTypeMap.contains(type))
    {
        serviceDiscoveryItems = m_serviceTypeMap.value(type);
    }
    else
    {
        return NULL;
    }

    foreach (QServiceDiscoveryItem *item, serviceDiscoveryItems)
    {
        if (item->name() == name)
        {
            return item;
        }
    }

    return NULL;
}

void QServiceDiscovery::updateItem(QString name, QString type)
{
    Q_UNUSED(name)
    updateServiceType(type);
}

void QServiceDiscovery::removeItem(QString name, QString type)
{
    QList<QServiceDiscoveryItem*> serviceDiscoveryItems;

    if (m_serviceTypeMap.contains(type))
    {
        serviceDiscoveryItems = m_serviceTypeMap.value(type);
    }
    else
    {
        return;
    }

    for (int i = 0; i < serviceDiscoveryItems.count(); ++i)
    {
        if (serviceDiscoveryItems.at(i)->name() == name)
        {
            serviceDiscoveryItems.removeAt(i);
            m_serviceTypeMap.insert(type, serviceDiscoveryItems);
            updateServiceType(type);
            return;
        }
    }
}

void QServiceDiscovery::clearItems(QString type)
{
    QList<QServiceDiscoveryItem*> serviceDiscoveryItems;

    if (m_serviceTypeMap.contains(type))
    {
        serviceDiscoveryItems = m_serviceTypeMap.value(type);
    }
    else
    {
        return;
    }

    // delete all service discovery items
    for (int i = (serviceDiscoveryItems.count()-1); i >= 0; i--)
    {
        serviceDiscoveryItems.at(i)->deleteLater();
        serviceDiscoveryItems.removeAt(i);
    }

    m_serviceTypeMap.insert(type, serviceDiscoveryItems);   // insert the empty list
    updateServiceType(type);
}

QString QServiceDiscovery::composeSdString(QString type, QString domain)
{
    return QString("_%1._tcp.%2").arg(type).arg(domain);
}

QString QServiceDiscovery::composeSdString(QString subType, QString type, QString domain)
{
    if (subType.isEmpty())
    {
        return composeSdString(type, domain);
    }
    else
    {
        return QString("_%1._sub._%2._tcp.%3").arg(subType).arg(type).arg(domain);
    }
}

void QServiceDiscovery::resultsReady(int id, const QJDns::Response &results)
{
    int type;
    QServiceDiscoveryItem * item;

    type = m_queryTypeMap.value(id);

    foreach(QJDns::Record r, results.answerRecords)
    {
        int newId;

        item = NULL;

        if (type == QJDns::Ptr)
        {

            QString serviceType = m_queryServiceMap.value(id);
            QString name = r.name.left(r.name.indexOf(composeSdString(m_serviceType, m_domain)) - 1);

#ifdef QT_DEBUG
            qDebug() << "Found mDNS/DNS-SD:" << r.owner << r.name << serviceType << name << "TTL:" << r.ttl;
#endif

            if (r.ttl != 0)
            {
                item = addItem(name, serviceType);
                item->setOutstandingRequests(3);     // We have to do 3 requests before the item is fully resolved
                newId = m_jdns->queryStart(r.name, QJDns::Txt);
                m_queryTypeMap.insert(newId, QJDns::Txt);
                m_queryItemMap.insert(newId, item);
                newId = m_jdns->queryStart(r.name, QJDns::Srv);
                m_queryTypeMap.insert(newId, QJDns::Srv);
                m_queryItemMap.insert(newId, item);
            }
            else
            {
                removeItem(name, serviceType);
            }
        }
        else if (type == QJDns::Txt)
        {
            QStringList txtRecords;

            item = m_queryItemMap.value(id);
            m_jdns->queryCancel(id);    // we have our results
            m_queryTypeMap.remove(id);
            m_queryItemMap.remove(id);

            foreach (QString string, r.texts)
            {
                txtRecords.append(string);
            }

            item->setTxtRecords(txtRecords);

#ifdef QT_DEBUG
            qDebug() << item->type() << item->name() << "Texts:" << r.texts;
#endif
        }
        else if (type == QJDns::Srv)
        {
            item = m_queryItemMap.value(id);
            m_jdns->queryCancel(id);    // we have our results
            m_queryTypeMap.remove(id);
            m_queryItemMap.remove(id);

            newId = m_jdns->queryStart(r.name, QJDns::A);
            m_queryTypeMap.insert(newId, QJDns::A);
            m_queryItemMap.insert(newId, item);

            item->setPort(r.port);
#ifdef QT_DEBUG
            qDebug() << item->type() << item->name() << "Port:" << r.port;
#endif
        }
        else if (type == QJDns::A)
        {
            item = m_queryItemMap.value(id);
            m_jdns->queryCancel(id);    // we have our results
            m_queryTypeMap.remove(id);
            m_queryItemMap.remove(id);

#ifdef QT_DEBUG
            qDebug() << item->type() << item->name() << "Address:" << r.address.toString();
#endif
            item->setHostAddress(r.address);
        }

        if (item != NULL)   // we got a answer to a request
        {
            item->setOutstandingRequests(item->outstandingRequests() - 1);
            if (item->outstandingRequests() == 0)   // item is fully resolved
            {
                updateItem(item->name(), item->type());
            }
        }
    }
}

void QServiceDiscovery::error(int id, QJDns::Error e)
{
    qDebug() << "==================== error ====================";
    qDebug() << id << e;
}

void QServiceDiscovery::openNetworkSession()
{
    // use the default network configuration and make sure that
    // the link is open
    QNetworkConfiguration networkConfig;

    if ((m_networkConfigManager->defaultConfiguration().bearerType() == QNetworkConfiguration::BearerEthernet)
            || (m_networkConfigManager->defaultConfiguration().bearerType() == QNetworkConfiguration::BearerWLAN))
    {
        networkConfig = m_networkConfigManager->defaultConfiguration();
    }
    else
    {
        foreach (QNetworkConfiguration config, m_networkConfigManager->allConfigurations(QNetworkConfiguration::Discovered))
        {
            if ((config.bearerType() == QNetworkConfiguration::BearerEthernet) ||
                    (config.bearerType() == QNetworkConfiguration::BearerWLAN))
            {
                networkConfig = config;
#ifdef QT_DEBUG
                qDebug() << "network configs: " << config.bearerTypeName() << config.bearerTypeFamily() << config.name();
#endif
            }
        }
    }

    if (networkConfig.isValid())
    {
#ifdef QT_DEBUG
            qDebug() << "network config: " << networkConfig.bearerTypeName() << networkConfig.bearerTypeFamily() << networkConfig.name();
#endif
        if (m_networkSession != NULL)
        {
            m_networkSession->deleteLater();
        }

        m_networkSession = new QNetworkSession(networkConfig);

        connect(m_networkSession, SIGNAL(opened()),
                this, SLOT(delayedInit()));
        connect(m_networkSession, SIGNAL(closed()),
                this, SLOT(deinitializeMdns()));

        m_networkSession->open();
    }
}

void QServiceDiscovery::updateNetConfig()
{
    if ((m_networkSession == NULL) || (!m_networkSession->isOpen()))
    {
        m_networkConfigManager->updateConfigurations();
    }
}
