#include "qappdiscovery.h"

QAppDiscovery::QAppDiscovery(QQuickItem *parent) :
    QQuickItem(parent)
{
    m_componentCompleted = false;
    m_jdns = NULL;
    m_networkSession = NULL;

    m_expiryCheckTimer = new QTimer(this);
    connect(m_expiryCheckTimer, SIGNAL(timeout()),
            this, SLOT(expiryCheck()));
    m_expiryCheckTimer->setInterval(60000); // check every 60s
}

/** componentComplete is executed when the QML component is fully loaded */
void QAppDiscovery::componentComplete()
{
    m_componentCompleted = true;

    initializeNetworkSession();

    QQuickItem::componentComplete();
}

void QAppDiscovery::initializeNetworkSession()
{
    // now begin the process of opening the network link
    m_networkConfigManager = new QNetworkConfigurationManager;
    connect(m_networkConfigManager, SIGNAL(updateCompleted()),
            this, SLOT(openNetworkSession()));
    m_networkConfigManager->updateConfigurations();
    // update the connections cyclically
    m_networkConfigTimer = new QTimer(this);
    m_networkConfigTimer->setInterval(5000);
    connect(m_networkConfigTimer, SIGNAL(timeout()),
            this, SLOT(updateNetConfig()));
    m_networkConfigTimer->start();
}

void QAppDiscovery::initializeMdns()
{
#ifdef QT_DEBUG
            qDebug() << "Initializing MDNS";
#endif
    m_jdns = new QJDns(this);

    connect(m_jdns, SIGNAL(resultsReady(int,QJDns::Response)),
            this, SLOT(resultsReady(int,QJDns::Response)));
    connect(m_jdns, SIGNAL(error(int,QJDns::Error)),
            this, SLOT(error(int,QJDns::Error)));

    m_jdns->init(QJDns::Multicast, QHostAddress::Any);

    if (m_running == true)
    {
        startQuery();
    }

    m_expiryCheckTimer->start();

    emit networkOpened();
}

void QAppDiscovery::deinitializeMdns()
{
#ifdef QT_DEBUG
            qDebug() << "Deinitializing MDNS";
#endif
    if (m_running)
    {
        m_queryTypeMap.remove(m_queryId);
    }

    m_jdns->deleteLater();
    m_jdns = NULL;

    m_expiryCheckTimer->stop();
    clearItems();

    emit networkClosed();
}

void QAppDiscovery::delayedInit()
{
    // for some reason there needs to be a delay if the network is initialized after the app was started
    QTimer::singleShot(1000,this, SLOT(initializeMdns()));
}

QQmlListProperty<QAppDiscoveryItem> QAppDiscovery::discoveredApps()
{
    return QQmlListProperty<QAppDiscoveryItem>(this, m_discoveredApps);
}

int QAppDiscovery::discoveredAppCount() const
{
    return m_discoveredApps.count();
}

QAppDiscoveryItem *QAppDiscovery::discoveredApp(int index) const
{
    return m_discoveredApps.at(index);
}

void QAppDiscovery::setRegType(QString arg)
{
    if (m_regType != arg) {

        if (m_running && (m_networkSession != NULL) && m_networkSession->isOpen()) {
            stopQuery();
        }

        m_regType = arg;
        emit regTypeChanged(arg);

        if (m_running && (m_networkSession != NULL) && m_networkSession->isOpen()) {
            startQuery();
        }
    }
}

void QAppDiscovery::setRunning(bool arg)
{
    if (m_running != arg) {
        m_running = arg;
        emit runningChanged(arg);

        if ((m_networkSession == NULL) || (!m_networkSession->isOpen())) {
            return;
        }

        if (m_running) {
            startQuery();
        }
        else
        {
            stopQuery();
        }
    }
}

void QAppDiscovery::startQuery()
{
    m_queryId = m_jdns->queryStart(m_regType.toLocal8Bit(), QJDns::Ptr);
    m_queryTypeMap.insert(m_queryId, QJDns::Ptr);
}

void QAppDiscovery::stopQuery()
{
    m_jdns->queryCancel(m_queryId);
    m_queryTypeMap.remove(m_queryId);
}

QAppDiscoveryItem *QAppDiscovery::addItem(QString name)
{
    foreach (QAppDiscoveryItem *item, m_discoveredApps)
    {
        if (item->name() == name)
        {
            return item;
        }
    }

    QAppDiscoveryItem *item = new QAppDiscoveryItem();
    item->setName(name);
    m_discoveredApps.append(item);
    emit discoveredAppsChanged(discoveredApps());

    return item;
}

QAppDiscoveryItem *QAppDiscovery::getItem(QString name)
{
    foreach (QAppDiscoveryItem *item, m_discoveredApps)
    {
        if (item->name() == name)
        {
            return item;
        }
    }

    return NULL;
}

void QAppDiscovery::removeItem(QString name)
{
    for (int i = 0; i < m_discoveredApps.count(); ++i)
    {
        if (m_discoveredApps.at(i)->name() == name)
        {
            m_discoveredApps.removeAt(i);
            emit discoveredAppsChanged(discoveredApps());
            return;
        }
    }
}

void QAppDiscovery::clearItems()
{
    m_discoveredApps.clear();
    emit discoveredAppsChanged(discoveredApps());
}

void QAppDiscovery::resultsReady(int id, const QJDns::Response &results)
{
    int type;

    type = m_queryTypeMap.value(id);

    foreach(QJDns::Record r, results.answerRecords)
    {
        int newId;

        if (type == QJDns::Ptr)
        {
            QString name = r.name.left(r.name.indexOf(r.owner)-1);

#ifdef QT_DEBUG
            qDebug() << "Found MDNS:" << name << "TTL:" << r.ttl;
#endif

            if (r.ttl != 0)
            {
                QAppDiscoveryItem * item;

                item = addItem(name);
                id = m_jdns->queryStart(r.name, QJDns::Txt);
                m_queryTypeMap.insert(id, QJDns::Txt);
                m_queryItemMap.insert(id, item);
                id = m_jdns->queryStart(r.name, QJDns::Srv);
                m_queryTypeMap.insert(id, QJDns::Srv);
                m_queryItemMap.insert(id, item);
            }
            else
            {
                removeItem(name);
            }
        }
        else if (type == QJDns::Txt)
        {
            QAppDiscoveryItem * item;

            item = m_queryItemMap.value(id);
            m_jdns->queryCancel(id);    // we have our results
            m_queryTypeMap.remove(id);
            m_queryItemMap.remove(id);

            foreach (QString string, r.texts)
            {
                QStringList keyValue;
                keyValue = string.split("=");

                if (keyValue.at(0) == "dsname")
                {
                    item->setDsname(keyValue.at(1));
                }
            }

#ifdef QT_DEBUG
            qDebug() << "Texts:" << r.texts;
#endif
        }
        else if (type == QJDns::Srv)
        {
            QAppDiscoveryItem * item;

            item = m_queryItemMap.value(id);
            m_jdns->queryCancel(id);    // we have our results
            m_queryTypeMap.remove(id);
            m_queryItemMap.remove(id);

            newId = m_jdns->queryStart(r.name, QJDns::A);
            m_queryTypeMap.insert(newId, QJDns::A);
            m_queryItemMap.insert(newId, item);

            item->setPort(r.port);
#ifdef QT_DEBUG
            qDebug() << "Port:" << r.port;
#endif
        }
        else if (type == QJDns::A)
        {
            QAppDiscoveryItem * item;

            item = m_queryItemMap.value(id);
            m_jdns->queryCancel(id);    // we have our results
            m_queryTypeMap.remove(id);
            m_queryItemMap.remove(id);

            item->setHostAddress(r.address);
#ifdef QT_DEBUG
            qDebug() << "Address:" << r.address.toString();
#endif
        }
    }
}

void QAppDiscovery::error(int id, QJDns::Error e)
{
    qDebug() << "==================== error ====================";
    qDebug() << id << e;
}

void QAppDiscovery::expiryCheck()
{
    foreach (QAppDiscoveryItem *item, m_discoveredApps)
    {
        if (item->expiryDate() < QDateTime::currentDateTime())  // item is expired
        {
            removeItem(item->name());
        }
    }
}

void QAppDiscovery::openNetworkSession()
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

void QAppDiscovery::updateNetConfig()
{
    if ((m_networkSession == NULL) || (!m_networkSession->isOpen()))
    {
        m_networkConfigManager->updateConfigurations();
    }
}
