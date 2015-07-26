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
#include "debughelper.h"

#if defined(Q_OS_ANDROID)
#include <QtAndroidExtras/QAndroidJniObject>
#endif

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

    The following example demonstrates the usage of the unicast DNS mode.

    \qml
    Item {
        ServiceDiscovery {
            id: serviceDiscovery

            running:     true
            lookupMode:  ServiceDiscovery.UnicastDNS

            nameServers: [
                NameServer {
                    hostName: "192.168.1.16"
                    port: 5353
                }
            ]

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

    \sa Service, ServiceList, ServiceDiscoveryFilter, ServiceDiscoveryItem
*/

/*! \qmlproperty bool ServiceDiscovery::running

    This property holds whether the service discovery is running or not.
    If the service discovery is set to not running the list of discovered
    services will be cleared.

    The default value is \c{false}.
*/

/*! \qmlproperty bool ServiceDiscovery::networkReady

    This property holds whether a suitable network connection is
    available or not. The \l running property will have no effect
    until a network connection is available
*/

/*! \qmlproperty bool ServiceDiscovery::lookupReady

    This property holds whether a the lookup client is successfully initialized
    or not. The \l running property will have no effect until the lookup
    client is ready.
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

/*! \qmlproperty list<NameServer> ServiceDiscovery::nameServers

    This property holds a list of name servers. Name servers have to specified
    in case unicast DNS is used as \l lookupMode . If this list is empty the
    default name servers of the system will be used.

    \note It is necessary to run \l updateNameServers after modifying the name servers.
*/

/*! \qmlproperty enumeration ServiceDiscovery::lookupMode

    This property holds the method that should be used for discovering services.

    \list
    \li ServiceDiscovery.MulticastDNS - Multicast DNS (mDNS) is used to discovery services. (default)
    \li ServiceDiscovery.UnicastDNS - Unicast DNS is used to discover services. It is necessary to specify \l nameServers .
    \endlist
*/

/*! \qmlproperty int ServiceDiscovery::unicastLookupInterval

    This property holds the interval for looking up services in unicast DNS mode
    in milliseconds.

    The default value is \c{5000}.
*/

/*! \qmlmethod void ServiceDiscovery::updateServices()

    Updates the \l{serviceLists}. Needs to be executed after modifying
    the \l serviceLists property.
*/

/*! \qmlmethod void ServiceDiscovery::updateFilter()

    Updates the \l{filter}. Needs to be executed after modifying
    the \l filter property.
*/

/*! \qmlmethod void ServiceDiscovery::updateNameServers()

    Updates the \l{nameServers}. Needs to be executed after modifying
    the \l nameServers property.
*/

/*! \qmlmethod void ServiceDiscovery::addNameServer(NameServer nameServer)

    Adds a name server to the list of \l{nameServers}.
*/

/*! \qmlmethod void ServiceDiscovery::removeNameServer(int index)

    Removes a name server from the list of \l{nameServers}.
*/

/*! \qmlmethod void ServiceDiscovery::clearNameServers()

    Clears the list of \l{nameServers}.
*/

QServiceDiscovery::QServiceDiscovery(QObject *parent) :
    QObject(parent),
    QQmlParserStatus(),
    m_componentCompleted(false),
    m_running(false),
    m_networkReady(false),
    m_lookupReady(false),
    m_lookupMode(MulticastDNS),
    m_unicastLookupInterval(5000),
    m_unicastErrorThreshold(2),
    m_filter(new QServiceDiscoveryFilter(this)),
    m_networkSession(NULL),
    m_networkConfigManager(NULL),
    m_networkConfigTimer(new QTimer(this)),
    m_jdns(NULL),
    m_unicastLookupTimer(new QTimer(this))
{
    m_networkConfigTimer->setInterval(3000);
    connect(m_networkConfigTimer, SIGNAL(timeout()),
            this, SLOT(updateNetConfig()));

    m_unicastLookupTimer->setInterval(m_unicastLookupInterval);
    connect(m_unicastLookupTimer, SIGNAL(timeout()),
            this, SLOT(unicastLookup()));

    connect(this, SIGNAL(nameServersChanged(QQmlListProperty<QNameServer>)),
            this, SLOT(updateNameServers()));
}

/** componentComplete is executed when the QML component is fully loaded */
void QServiceDiscovery::componentComplete()
{
    m_componentCompleted = true;

    initializeNetworkSession();
}

void QServiceDiscovery::initializeNetworkSession()
{
    // now begin the process of opening the network link
    m_networkConfigManager = new QNetworkConfigurationManager(this);
    connect(m_networkConfigManager, SIGNAL(updateCompleted()),
            this, SLOT(openNetworkSession()));
    m_networkConfigManager->updateConfigurations();
    m_networkConfigTimer->start(); // update the connections cyclically
}

bool QServiceDiscovery::initializeMdns()
{
    bool initialized;

    if (m_jdns != NULL) // already initialized
    {
        return true;
    }

#ifdef QT_DEBUG
    QString mode;
    if (m_lookupMode == MulticastDNS)
    {
        mode = "multicast";
    }
    else {
        mode = "unicast";
    }
    DEBUG_TAG(1, "SD", "Initializing JDNS " + mode);
#endif

    m_jdns = new QJDns(this);

    connect(m_jdns, SIGNAL(resultsReady(int,QJDns::Response)),
            this, SLOT(resultsReady(int,QJDns::Response)));
    connect(m_jdns, SIGNAL(error(int,QJDns::Error)),
            this, SLOT(error(int,QJDns::Error)));

    if (m_lookupMode == MulticastDNS)
    {
#if defined(Q_OS_ANDROID)
        QAndroidJniObject::callStaticMethod<void>("io/machinekit/service/MulticastActivator",
                                                  "enable");
#endif
        initialized = m_jdns->init(QJDns::Multicast, QHostAddress::Any);
    }
    else
    {
        initialized = m_jdns->init(QJDns::Unicast, QHostAddress::Any);
    }

    if (!initialized) // something went wrong
    {
#ifdef QT_DEBUG
        DEBUG_TAG(1, "SD", "Initializing JDNS failed");
        DEBUG_TAG(1, "SD", m_jdns->debugLines());
#endif

        m_jdns->deleteLater();
        m_jdns = NULL;
        return false;
    }
    else
    {
        m_lookupReady = true;

        if (m_lookupMode == UnicastDNS)
        {
            updateNameServers();
        }

        if (m_running == true)
        {
            updateServices();

            if (m_lookupMode == UnicastDNS)
            {
                m_unicastLookupTimer->start();
            }
        }

        emit lookupReadyChanged(m_lookupReady);
    }

    return true;
}

void QServiceDiscovery::deinitializeMdns()
{
    if (m_jdns == NULL)
    {
        return;
    }

#ifdef QT_DEBUG
    DEBUG_TAG(1, "SD", "Deinitializing JDNS");
#endif

    if (m_running)
    {
        if (m_lookupMode == UnicastDNS)
        {
            m_unicastLookupTimer->stop();
        }

        removeAllServiceTypes();
        m_queryIdItemMap.clear();
        m_queryIdServiceMap.clear();
        m_queryIdTypeMap.clear();
    }

    m_jdns->deleteLater();
    m_jdns = NULL;

#if defined(Q_OS_ANDROID)
    QAndroidJniObject::callStaticMethod<void>("io/machinekit/service/MulticastActivator",
                                              "disable");
#endif

    m_lookupReady = false;                      // lookup no ready anymore
    emit lookupReadyChanged(m_lookupReady);
}

void QServiceDiscovery::networkSessionOpened()
{
    m_networkReady = true;          // the network is ready
    emit networkReadyChanged(m_networkReady);

    if (!initializeMdns()) {
        // if the initialization fails we set network ready to
        // false in order to retrigger the after a certain period
        m_networkReady = false;
        emit networkReadyChanged(m_networkReady);
    }
}

void QServiceDiscovery::networkSessionClosed()
{
    deinitializeMdns();

    m_networkReady = false;                     // network no ready anymore
    emit networkReadyChanged(m_networkReady);
}

void QServiceDiscovery::networkSessionError(QNetworkSession::SessionError error)
{
#ifdef QT_DEBUG
    WARNING_TAG(1, "SD", "network session error:" << error << m_networkSession->errorString());
#else
    Q_UNUSED(error)
#endif
}

void QServiceDiscovery::unicastLookup()
{
    QMapIterator<QString, QList<QServiceDiscoveryItem*> > i(m_serviceItemsMap);
    while (i.hasNext()) {
        i.next();
        refreshQuery(i.key());
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

QQmlListProperty<QNameServer> QServiceDiscovery::nameServers()
{
    return QQmlListProperty<QNameServer>(this, m_nameServers);
}

int QServiceDiscovery::nameServerCount() const
{
    return m_nameServers.count();
}

QNameServer *QServiceDiscovery::nameServer(int index) const
{
    return m_nameServers.at(index);
}

void QServiceDiscovery::addNameServer(QNameServer *nameServer)
{
    m_nameServers.append(nameServer);
    emit nameServersChanged(nameServers());
}

void QServiceDiscovery::removeNameServer(int index)
{
    m_nameServers.removeAt(index);
    emit nameServersChanged(nameServers());
}

void QServiceDiscovery::clearNameServers()
{
    m_nameServers.clear();
    emit nameServersChanged(nameServers());
}

void QServiceDiscovery::setUnicastErrorThreshold(int unicastErrorThreshold)
{
    if (m_unicastErrorThreshold == unicastErrorThreshold)
        return;

    m_unicastErrorThreshold = unicastErrorThreshold;
    emit unicastErrorThresholdChanged(unicastErrorThreshold);
}

void QServiceDiscovery::setRunning(bool arg)
{
    if (m_running != arg) {
        m_running = arg;
        emit runningChanged(arg);

        if (!m_networkReady) {
            return;
        }

        if (m_running) {
            startQueries();

            if (m_lookupMode == UnicastDNS)
            {
                m_unicastLookupTimer->start();
            }
        }
        else
        {
            if (m_lookupMode == UnicastDNS)
            {
                m_unicastLookupTimer->stop();
            }

            stopQueries();
        }
    }
}

void QServiceDiscovery::updateServices()
{
    QMap<QString, QList<QServiceDiscoveryItem*> > oldServiceTypeMap;

    oldServiceTypeMap = m_serviceItemsMap;

    // Iterate through all services and update all service types
    foreach (QServiceList *serviceList, m_serviceLists)
    {
        for (int i = 0; i < serviceList->serviceCount(); ++i)
        {
            QService *service;

            service = serviceList->service(i);
            disconnect(service, SIGNAL(queriesChanged()),
                       this, SLOT(updateServices()));
            connect(service, SIGNAL(queriesChanged()),
                    this, SLOT(updateServices()));
            for (int j = 0; j < service->queriesCount(); ++j)
            {
                QServiceDiscoveryQuery *query;

                query = service->query(j);
                if (!query->serviceType().isEmpty())
                {
                    addServiceType(query->serviceType(), query->queryType());
                    oldServiceTypeMap.remove(query->serviceType());
                    if (m_running && m_networkReady)
                    {
                        startQuery(query->serviceType());
                    }
                }
            }
        }
    }

    // Iterate trough all item that are left and remove them
    QMapIterator<QString, QList<QServiceDiscoveryItem*> > i(oldServiceTypeMap);
    while (i.hasNext()) {
        i.next();

        if (m_running && m_networkReady)
        {
            stopQuery(i.key());
        }
        removeServiceType(i.key());
    }

    updateAllServiceTypes(); // now we need to refill all queries with fresh data
}

void QServiceDiscovery::updateFilter()
{
    updateAllServiceTypes();
}

void QServiceDiscovery::updateNameServers()
{
    QList<QJDns::NameServer> nameServers;

    if ((m_jdns == NULL) || (m_lookupMode != UnicastDNS))
    {
        return;
    }

    foreach (QNameServer *nameServer, m_nameServers)
    {
        QJDns::NameServer host;

        if (nameServer->hostAddress().isNull())
        {
            continue;
        }

        host.address = nameServer->hostAddress();
        host.port = nameServer->port();
        nameServers.append(host);
    }

    if (nameServers.isEmpty())
    {
        nameServers = QJDns::systemInfo().nameServers;
    }

    if (nameServers.isEmpty())
    {
        // TODO: error
    }

    m_jdns->setNameServers(nameServers);

    if (m_running)
    {
        unicastLookup();
    }
}

void QServiceDiscovery::setUnicastLookupInterval(int arg)
{
    if (m_unicastLookupInterval != arg) {
        m_unicastLookupInterval = arg;
        emit unicastLookupIntervalChanged(arg);

        m_unicastLookupTimer->setInterval(arg);
    }
}

void QServiceDiscovery::setLookupMode(QServiceDiscovery::LookupMode arg)
{
    if (m_lookupMode != arg)
    {
        bool ready;

        if (m_lookupReady)
        {
            deinitializeMdns();
            ready = true;
        }
        else
        {
            ready = false;
        }

        m_lookupMode = arg;
        emit lookupModeChanged(arg);

        if (ready)
        {
            if (!initializeMdns()) {
                // in case init fails we reset the network state to retrigger the initialization
                m_networkReady = false;
                emit networkReadyChanged(m_networkReady);
            }
        }
    }
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
    QMapIterator<QString, QList<QServiceDiscoveryItem*> > i(m_serviceItemsMap);
    while (i.hasNext()) {
        i.next();
        startQuery(i.key());
    }
}

void QServiceDiscovery::stopQueries()
{
    QMapIterator<QString, QList<QServiceDiscoveryItem*> > i(m_serviceItemsMap);
    while (i.hasNext()) {
        i.next();
        stopQuery(i.key());
    }
}

void QServiceDiscovery::startQuery(QString serviceType)
{
    int queryId;

    QMapIterator<int, QString> i(m_queryIdServiceMap);
    while (i.hasNext()) {
        i.next();
        if (i.value() == serviceType)  // query with the type already running
        {
            return;
        }
    }

    QJDns::Type queryType = m_serviceTypeMap.value(serviceType);

    queryId = m_jdns->queryStart(serviceType.toLocal8Bit(), queryType);
    m_queryIdTypeMap.insert(queryId, queryType);
    m_queryIdServiceMap.insert(queryId, serviceType);

#ifdef QT_DEBUG
    DEBUG_TAG(1, "SD", "Started query" << queryId << serviceType << queryType);
#endif
}

void QServiceDiscovery::stopQuery(QString serviceType)
{
    int queryId;
    bool found;

    found = false;
    QMapIterator<int, QString> i(m_queryIdServiceMap);
    while (i.hasNext()) {
        i.next();
        if (i.value() == serviceType)  // query type matching
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
    m_queryIdTypeMap.remove(queryId);
    m_queryIdServiceMap.remove(queryId);
    clearItems(serviceType);

#ifdef QT_DEBUG
    DEBUG_TAG(1, "SD", "Stopped query" << queryId << serviceType);
#endif
}

void QServiceDiscovery::refreshQuery(QString serviceType)
{
    int queryId;
    bool found;

    found = false;
    QMapIterator<int, QString> i(m_queryIdServiceMap);
    while (i.hasNext()) {
        i.next();
        if (i.value() == serviceType)  // query type matching
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

    QJDns::Type queryType = m_serviceTypeMap.value(serviceType);

    m_jdns->queryCancel(queryId);                                       // stop old query
    m_queryIdTypeMap.remove(queryId);
    m_queryIdServiceMap.remove(queryId);

    purgeItems(serviceType);                                                   // purge outdated items

    queryId = m_jdns->queryStart(serviceType.toLocal8Bit(), queryType);       // start a new query
    m_queryIdTypeMap.insert(queryId, queryType);
    m_queryIdServiceMap.insert(queryId, serviceType);

#ifdef QT_DEBUG
    DEBUG_TAG(2, "SD", "Refreshed query" << queryId << serviceType);
#endif
}

void QServiceDiscovery::stopItemQueries(QServiceDiscoveryItem *item)
{
    int queryId;

    QMapIterator<int, QServiceDiscoveryItem*> i(m_queryIdItemMap);
    while (i.hasNext()) {
        i.next();
        if (i.value() == item)  // query item matching
        {
            queryId = i.key();

            m_jdns->queryCancel(queryId);
            m_queryIdItemMap.remove(queryId);
            m_queryIdTypeMap.remove(queryId);
        }
    }
}

void QServiceDiscovery::addServiceType(QString serviceType, QJDns::Type queryType)
{
    QList<QServiceDiscoveryItem*> serviceDiscoveryItems;

    if (m_serviceItemsMap.contains(serviceType))
    {
        return;
    }

    m_serviceItemsMap.insert(serviceType, serviceDiscoveryItems);
    m_serviceTypeMap.insert(serviceType, queryType);
}

void QServiceDiscovery::removeServiceType(QString serviceType)
{
    if (!m_serviceItemsMap.contains(serviceType))
    {
        return;
    }

    clearItems(serviceType);
    m_serviceItemsMap.remove(serviceType);
    m_serviceTypeMap.remove(serviceType);
}

void QServiceDiscovery::updateServiceType(QString serviceType)
{
    QList<QServiceDiscoveryItem*> serviceDiscoveryItems;

    if (m_serviceItemsMap.contains(serviceType))
    {
        serviceDiscoveryItems = m_serviceItemsMap.value(serviceType);
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
            for (int j = 0; j < service->queriesCount(); ++j)
            {
                QServiceDiscoveryQuery *query;

                query = service->query(j);
                if (query->serviceType() == serviceType)
                {
                    if (query->queryType() == QJDns::A) // do not filter hostname resolve queries
                    {
                        query->setItems(serviceDiscoveryItems);
                    }
                    else
                    {
                        query->setItems(filterServiceDiscoveryItems(serviceDiscoveryItems, m_filter, query->filter()));
                    }
                }
            }
        }
    }
}

void QServiceDiscovery::removeAllServiceTypes()
{
    QMap<QString, QList<QServiceDiscoveryItem*> > serviceTypeMap = m_serviceItemsMap;
    QMapIterator<QString, QList<QServiceDiscoveryItem*> > i(serviceTypeMap);
    while (i.hasNext()) {
        i.next();
        removeServiceType(i.key());
    }
}

void QServiceDiscovery::updateAllServiceTypes()
{
    QMapIterator<QString, QList<QServiceDiscoveryItem*> > i(m_serviceItemsMap);
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

    if (m_serviceItemsMap.contains(type))
    {
        serviceDiscoveryItems = m_serviceItemsMap.value(type);
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

    QServiceDiscoveryItem *item = new QServiceDiscoveryItem(this);
    item->setName(name);
    item->setType(type);
    serviceDiscoveryItems.append(item);
    m_serviceItemsMap.insert(type, serviceDiscoveryItems);

    return item;
}

QServiceDiscoveryItem *QServiceDiscovery::getItem(QString name, QString type)
{
    QList<QServiceDiscoveryItem*> serviceDiscoveryItems;

    if (m_serviceItemsMap.contains(type))
    {
        serviceDiscoveryItems = m_serviceItemsMap.value(type);
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

    if (m_serviceItemsMap.contains(type))
    {
        serviceDiscoveryItems = m_serviceItemsMap.value(type);
    }
    else
    {
        return;
    }

    for (int i = 0; i < serviceDiscoveryItems.count(); ++i)
    {
        if (serviceDiscoveryItems.at(i)->name() == name)
        {
            stopItemQueries(serviceDiscoveryItems.at(i));
            serviceDiscoveryItems.takeAt(i)->deleteLater();
            m_serviceItemsMap.insert(type, serviceDiscoveryItems);
            updateServiceType(type);
            return;
        }
    }
}

void QServiceDiscovery::clearItems(QString type)
{
    QList<QServiceDiscoveryItem*> serviceDiscoveryItems;

    if (m_serviceItemsMap.contains(type))
    {
        serviceDiscoveryItems = m_serviceItemsMap.value(type);
    }
    else
    {
        return;
    }

    // delete all service discovery items
    for (int i = (serviceDiscoveryItems.count()-1); i >= 0; i--)
    {
        QServiceDiscoveryItem *item;
        item = serviceDiscoveryItems.takeAt(i);
        stopItemQueries(item);
        item->deleteLater();
    }

    m_serviceItemsMap.insert(type, serviceDiscoveryItems);   // insert the empty list
    updateServiceType(type);
}

/** Removes items that have not been updated and flags other items with not updated **/
void QServiceDiscovery::purgeItems(QString serviceType)
{
    QList<QServiceDiscoveryItem*> serviceDiscoveryItems;
    bool modified;

    if (m_serviceItemsMap.contains(serviceType))
    {
        serviceDiscoveryItems = m_serviceItemsMap.value(serviceType);
    }
    else
    {
        return;
    }

    modified = false;
    for (int i = (serviceDiscoveryItems.count()-1); i >= 0; i--)
    {
        QServiceDiscoveryItem *serviceDiscoveryItem = serviceDiscoveryItems[i];

        if (!serviceDiscoveryItem->updated())    // remove old items
        {
            serviceDiscoveryItem->increaseErrorCount();
            if (serviceDiscoveryItem->errorCount() > m_unicastErrorThreshold ) // if threshold is reached we cleanup
            {
                stopItemQueries(serviceDiscoveryItem);
                serviceDiscoveryItems.removeAt(i);
                serviceDiscoveryItem->deleteLater();
                modified = true;
            }
        }
        else
        {
            serviceDiscoveryItem->setUpdated(false);
        }
    }

    if (modified)
    {
        m_serviceItemsMap.insert(serviceType, serviceDiscoveryItems);   // insert the modified list
        updateServiceType(serviceType);
    }
}

void QServiceDiscovery::resultsReady(int id, const QJDns::Response &results)
{
    QJDns::Type type;

    type = m_queryIdTypeMap.value(id);

    foreach(QJDns::Record r, results.answerRecords)
    {
        QServiceDiscoveryItem * item;
        int newId;

        item = NULL;

        if (type == QJDns::Ptr)
        {
            QString serviceType = m_queryIdServiceMap.value(id);
            QString name = r.name.left(r.name.indexOf("._"));

#ifdef QT_DEBUG
            DEBUG_TAG(2, "SD", "Ptr DNS record:" << r.owner << r.name << serviceType << name << "TTL:" << r.ttl);
#endif

            if (r.ttl > 0)
            {
                item = addItem(name, serviceType);
                item->setOutstandingRequests(3);     // We have to do 3 requests before the item is fully resolved
                newId = m_jdns->queryStart(r.name, QJDns::Txt);
                m_queryIdTypeMap.insert(newId, QJDns::Txt);
                m_queryIdItemMap.insert(newId, item);
                newId = m_jdns->queryStart(r.name, QJDns::Srv);
                m_queryIdTypeMap.insert(newId, QJDns::Srv);
                m_queryIdItemMap.insert(newId, item);
            }
            else
            {
                removeItem(name, serviceType);
            }
        }
        else if (type == QJDns::Txt)
        {
            QStringList txtRecords;

            item = m_queryIdItemMap.value(id);
            m_jdns->queryCancel(id);    // we have our results
            m_queryIdTypeMap.remove(id);
            m_queryIdItemMap.remove(id);

            foreach (QString string, r.texts)
            {
                txtRecords.append(string);
            }

            item->setTxtRecords(txtRecords);

#ifdef QT_DEBUG
            DEBUG_TAG(2, "SD", "Txt DNS record" << item->type() << item->name() << "Texts:" << r.texts);
#endif
        }
        else if (type == QJDns::Srv)
        {
            item = m_queryIdItemMap.value(id);
            m_jdns->queryCancel(id);    // we have our results
            m_queryIdTypeMap.remove(id);
            m_queryIdItemMap.remove(id);

            newId = m_jdns->queryStart(r.name, QJDns::A);
            m_queryIdTypeMap.insert(newId, QJDns::A);
            m_queryIdItemMap.insert(newId, item);

            item->setPort(r.port);

#ifdef QT_DEBUG
            DEBUG_TAG(2, "SD", "Srv DNS record" << item->type() << item->name() << "Port:" << r.port);
#endif
        }
        else if (type == QJDns::A)
        {
            QString serviceType = m_queryIdServiceMap.value(id, QString());

            if (serviceType.isEmpty()) // this request was started by another service
            {
                item = m_queryIdItemMap.value(id);
                m_jdns->queryCancel(id);    // we have our results
                m_queryIdTypeMap.remove(id);
                m_queryIdItemMap.remove(id);
                item->setHostAddress(r.address);
            }
            else
            {
                if (r.ttl > 0)
                {
                    item = addItem(serviceType, serviceType);
                    item->setOutstandingRequests(1);     // With this request the item is resolved
                    item->setHostAddress(r.address);
                }
                else
                {
                    removeItem(serviceType, serviceType);
                }
            }

#ifdef QT_DEBUG
            if (item) {
                DEBUG_TAG(2, "SD", "A DNS record" << item->type() << item->name() << "Address:" << r.address.toString());
            }
#endif
        }
        else if (type == QJDns::Aaaa)
        {
            QString serviceType = m_queryIdServiceMap.value(id, QString());

            if (!serviceType.isEmpty())
            {
                if (r.ttl != 0)
                {
                    item = addItem(serviceType, serviceType);
                    item->setOutstandingRequests(1);     // With this request the item is resolved
                    item->setHostAddress(r.address);
                }
                else
                {
                    removeItem(serviceType, serviceType);
                }
            }

#ifdef QT_DEBUG
            if (item) {
                DEBUG_TAG(2, "SD", "AAA DNS record" << item->type() << item->name() << "Address:" << r.address.toString());
            }
#endif
        }

        if (item != NULL)   // we got a answer to a request
        {
            item->setOutstandingRequests(item->outstandingRequests() - 1);
            if (item->outstandingRequests() <= 0)   // item is fully resolved
            {
                updateItem(item->name(), item->type());
                item->setUpdated(true);
                item->resetErrorCount();
            }
        }
    }
}

void QServiceDiscovery::error(int id, QJDns::Error e)
{
    QString errorString;
    if(e == QJDns::ErrorGeneric)
        errorString = "Generic";
    else if(e == QJDns::ErrorNXDomain)
        errorString = "NXDomain";
    else if(e == QJDns::ErrorTimeout)
        errorString = "Timeout";
    else if(e == QJDns::ErrorConflict)
        errorString = "Conflict";

#ifdef QT_DEBUG
    WARNING_TAG(1, "SD",  "==================== error ====================");
    WARNING_TAG(1, "SD",  "id:" << id << errorString);
    WARNING_TAG(1, "SD", m_jdns->debugLines());
#else
    Q_UNUSED(id)
#endif
}

void QServiceDiscovery::openNetworkSession()
{
#ifdef QT_DEBUG
                DEBUG_TAG(3, "SD", "trying to open network session");
#endif

    // use the default network configuration and make sure that the link is open
    QList<QNetworkConfiguration> availableConfigs;
    QNetworkConfiguration defaultConfig = m_networkConfigManager->defaultConfiguration();

    if (defaultConfig.isValid()) {
        availableConfigs.append(defaultConfig);
    }
    availableConfigs.append(m_networkConfigManager->allConfigurations(QNetworkConfiguration::Discovered));

#ifdef QT_DEBUG
    DEBUG_TAG(2, "SD", "number of configs: " << availableConfigs.size());
#endif

    for (int i = 0; i < availableConfigs.size(); ++i)
    {
        QNetworkConfiguration config = availableConfigs.at(i);
        if ((config.bearerType() == QNetworkConfiguration::BearerEthernet)
            || (config.bearerType() == QNetworkConfiguration::BearerWLAN)
            || (config.bearerType() == QNetworkConfiguration::BearerUnknown))  // unknown is usually ethernet or any other local network
        {
#ifdef QT_DEBUG
            DEBUG_TAG(2, "SD", "network config: " << config.bearerTypeName() << config.bearerTypeFamily() << config.name());
#endif
            if (m_networkSession != NULL)
            {
                m_networkSession->deleteLater();
            }

            m_networkSession = new QNetworkSession(config, this);

            connect(m_networkSession, SIGNAL(opened()),
                    this, SLOT(networkSessionOpened()));
            connect(m_networkSession, SIGNAL(closed()),
                    this, SLOT(networkSessionClosed()));
            connect(m_networkSession, SIGNAL(error(QNetworkSession::SessionError)),
                    this, SLOT(networkSessionError(QNetworkSession::SessionError)));

            m_networkSession->open();

            return;
        }
#ifdef QT_DEBUG
        else
        {
            DEBUG_TAG(2, "SD", "unsupported network config: " << config.bearerTypeName() << config.bearerTypeFamily() << config.name());
        }
#endif
    }
}

void QServiceDiscovery::updateNetConfig()
{
    if ((m_networkSession == NULL)
            || (!m_networkSession->isOpen())
            || (!m_networkReady))
    {
        m_networkConfigManager->updateConfigurations();
    }
}
