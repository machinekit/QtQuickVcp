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

    void setRegType(QString arg);
    void setRunning(bool arg);

signals:
    void regTypeChanged(QString arg);
    void runningChanged(bool arg);
    void discoveredAppsChanged(QQmlListProperty<QAppDiscoveryItem> arg);
    void networkOpened();
    void networkClosed();

private:
    QString m_regType;
    bool m_running;
    QList<QAppDiscoveryItem*> m_discoveredApps;

    QNetworkSession *m_networkSession;
    QNetworkConfigurationManager *m_networkConfigManager;
    QJDns *m_jdns;
    QMap<int, int> m_queryTypeMap;
    QMap<int, QAppDiscoveryItem*> m_queryItemMap;
    bool m_componentCompleted;
    QTimer *m_expiryCheckTimer;
    QTimer *m_networkConfigTimer; // Timer for refreshing the network status
    int m_queryId;

    void initializeNetworkSession();
    void startQuery();
    void stopQuery();
    QAppDiscoveryItem* addItem(QString name);
    QAppDiscoveryItem* getItem(QString name);
    void removeItem(QString name);
    void clearItems();

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
