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

    QQmlListProperty<QAppDiscoveryItem> discoveredApps();
    int discoveredAppCount() const;
    QAppDiscoveryItem *discoveredApp(int index) const;

public slots:

    void setRegType(QString arg)
    {
        if (m_regType != arg) {

            if (m_running && m_componentCompleted) {
                stopQuery();
            }

            m_regType = arg;
            emit regTypeChanged(arg);

            if (m_running && m_componentCompleted) {
                startQuery();
            }
        }
    }

    void setRunning(bool arg)
    {
        if (m_running != arg) {
            m_running = arg;
            emit runningChanged(arg);

            if (m_componentCompleted == false) {
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

signals:
    void regTypeChanged(QString arg);
    void runningChanged(bool arg);
    void discoveredAppsChanged(QQmlListProperty<QAppDiscoveryItem> arg);

private:
    QString m_regType;
    bool m_running;
    QList<QAppDiscoveryItem*> m_discoveredApps;

    QJDns *m_jdns;
    QMap<int, int> m_queryTypeMap;
    QMap<int, QAppDiscoveryItem*> m_queryItemMap;
    bool m_componentCompleted;
    QTimer *m_expiryCheckTimer;
    int m_queryId;

    void startQuery();
    void stopQuery();
    QAppDiscoveryItem* addItem(QString name);
    QAppDiscoveryItem* getItem(QString name);
    void removeItem(QString name);

private slots:
    void resultsReady(int id, const QJDns::Response &results);
    void error(int id, QJDns::Error e);
    void expiryCheck();
};

#endif // QAPPDISCOVERY_H
