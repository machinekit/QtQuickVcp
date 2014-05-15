#include "qappdiscovery.h"

QAppDiscovery::QAppDiscovery(QQuickItem *parent) :
    QQuickItem(parent)
{
    m_componentCompleted = false;

    m_jdns = new QJDns(this);

    connect(m_jdns, SIGNAL(resultsReady(int,QJDns::Response)),
            this, SLOT(resultsReady(int,QJDns::Response)));
    connect(m_jdns, SIGNAL(error(int,QJDns::Error)),
            this, SLOT(error(int,QJDns::Error)));

    m_jdns->init(QJDns::Multicast, QHostAddress::Any);

    m_expiryCheckTimer = new QTimer(this);
    connect(m_expiryCheckTimer, SIGNAL(timeout()),
            this, SLOT(expiryCheck()));
    m_expiryCheckTimer->start(60000);   // check every 60s
}

/** componentComplete is executed when the QML component is fully loaded */
void QAppDiscovery::componentComplete()
{
    m_componentCompleted = true;

    if (m_running == true)
    {
        startQuery();
    }

    QQuickItem::componentComplete();
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
            qDebug() << name;
            qDebug() << r.ttl;
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
            qDebug() << r.texts;
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
            qDebug() << r.port;
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
            qDebug() << r.address.toString();
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
