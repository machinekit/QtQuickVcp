#ifndef QSERVICEDISCOVERY_H
#define QSERVICEDISCOVERY_H

#include <QQuickItem>
#include <QUdpSocket>
#include <QTimer>
#include <QTime>
#include <QQmlListProperty>
#include "message.pb.h"
#include "qservice.h"

class QServiceDiscovery : public QQuickItem
{
    Q_OBJECT
    Q_INTERFACES(QQmlParserStatus)
    Q_PROPERTY(int port READ port WRITE setPort NOTIFY portChanged)
    Q_PROPERTY(int instance READ instance WRITE setInstance NOTIFY instanceChanged)
    Q_PROPERTY(int retryTime READ retryTime WRITE setRetryTime NOTIFY retryTimeChanged)
    Q_PROPERTY(int maxWait READ maxWait WRITE setMaxWait NOTIFY maxWaitChanged)
    Q_PROPERTY(bool trace READ trace WRITE setTrace NOTIFY traceChanged)
    Q_PROPERTY(bool running READ isRunning WRITE setRunning NOTIFY runningChanged)
    Q_PROPERTY(bool repeat READ repeat WRITE setRepeat NOTIFY repeatChanged)
    Q_PROPERTY(QQmlListProperty<QService> services READ services)
    Q_PROPERTY(QQmlListProperty<QService> replies READ replies)


public:
    explicit QServiceDiscovery(QQuickItem *parent = 0);

    virtual void componentComplete();

    int port() const
    {
        return m_port;
    }

    int instance() const
    {
        return m_instance;
    }

    int retryTime() const
    {
        return m_retryTime;
    }

    int maxWait() const
    {
        return m_maxWait;
    }

    bool trace() const
    {
        return m_trace;
    }

    bool isRunning() const
    {
        return m_running;
    }

    bool repeat() const
    {
        return m_repeat;
    }

    QQmlListProperty<QService> services();
    int serviceCount() const;
    QService *service(int index) const;

    QQmlListProperty<QService> replies();
    int replieCount() const;
    QService *replie(int index) const;

public slots:

    void startDiscovery();
    void stopDiscovery();
    void sendBroadcast();

    void setPort(int arg)
    {
        if (m_port != arg) {
            m_port = arg;
            emit portChanged(arg);
        }
    }
    void setInstance(int arg)
    {
        if (m_instance != arg) {
            m_instance = arg;
            emit instanceChanged(arg);
        }
    }
    void setRetryTime(int arg)
    {
        if (m_retryTime != arg) {
            m_retryTime = arg;
            emit retryTimeChanged(arg);
        }
    }
    void setMaxWait(int arg)
    {
        if (m_maxWait != arg) {
            m_maxWait = arg;
            emit maxWaitChanged(arg);
        }
    }
    void setTrace(bool arg);

    void setRunning(bool arg);

    void setRepeat(bool arg)
    {
        if (m_repeat != arg) {
            m_repeat = arg;
            emit repeatChanged(arg);
        }
    }

private:
    int m_currentWanted;
    int m_port;
    int m_instance;
    int m_retryTime;
    int m_maxWait;
    bool m_trace;
    bool m_running;
    bool m_repeat;

    QByteArray m_probe;
    QUdpSocket *m_udpSocket;
    QTimer *m_timeoutTimer;
    QTime *m_waitTime;

    QList<QService*> m_services;
    QList<QService*> m_replies;

private slots:
    void udpReadyRead();
    void timeout();

signals:
    void discoveryTimeout();
    void discoverySucceded();

    void portChanged(int arg);
    void instanceChanged(int arg);
    void retryTimeChanged(int arg);
    void maxWaitChanged(int arg);
    void traceChanged(bool arg);
    void runningChanged(bool arg);
    void repeatChanged(bool arg);
};

#endif // QSERVICEDISCOVERY_H
