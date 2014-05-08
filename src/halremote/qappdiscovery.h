#ifndef QAPPDISCOVERY_H
#define QAPPDISCOVERY_H

#include <QQuickItem>
#include <qjdns.h>
#include <qjdnsshared.h>
#include "qappdiscoveryitem.h"

class QAppDiscovery : public QQuickItem
{
    Q_OBJECT
    Q_INTERFACES(QQmlParserStatus)
    Q_PROPERTY(QString regType READ regType WRITE setRegType NOTIFY regTypeChanged)
    Q_PROPERTY(bool running READ running WRITE setRunning NOTIFY runningChanged)
    Q_PROPERTY(int interval READ interval WRITE setInterval NOTIFY intervalChanged)
    Q_PROPERTY(QQmlListProperty<QAppDiscoveryItem> discoveredApps READ discoveredApps NOTIFY discoveredAppsChanged)

public:
    explicit QAppDiscovery(QQuickItem *parent = 0);

    virtual void componentComplete();

    QString regType() const
    {
        return m_regType;
    }

    bool running() const
    {
        return m_running;
    }

    int interval() const
    {
        return m_interval;
    }

    QQmlListProperty<QAppDiscoveryItem> discoveredApps();
    int discoveredAppCount() const;
    QAppDiscoveryItem *discoveredApp(int index) const;

public slots:

    void setRegType(QString arg)
    {
        if (m_regType != arg) {
            m_regType = arg;
            emit regTypeChanged(arg);
        }
    }

    void setRunning(bool arg)
    {
        if (m_running != arg) {
            m_running = arg;
            emit runningChanged(arg);
        }
    }

    void setInterval(int arg)
    {
        if (m_interval != arg) {
            m_interval = arg;
            emit intervalChanged(arg);
        }
    }

signals:
    void regTypeChanged(QString arg);
    void runningChanged(bool arg);
    void intervalChanged(int arg);
    void discoveredAppsChanged(QQmlListProperty<QAppDiscoveryItem> arg);

private:
    QString m_regType;
    bool m_running;
    int m_interval;
    QList<QAppDiscoveryItem*> m_discoveredApps;

    QJDns *m_jdns;
    QMap<int, int> m_queryTypeMap;
    QMap<int, QAppDiscoveryItem*> m_queryItemMap;
    bool m_componentCompleted;
    QTimer *m_expiryCheckTimer;

    void startQuery();
    QAppDiscoveryItem* addItem(QString name);
    QAppDiscoveryItem* getItem(QString name);
    void removeItem(QString name);

private slots:
    void resultsReady(int id, const QJDns::Response &results);
    void error(int id, QJDns::Error e);
    void expiryCheck();
};

#endif // QAPPDISCOVERY_H
