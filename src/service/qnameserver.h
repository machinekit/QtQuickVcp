#ifndef QNAMESERVER_H
#define QNAMESERVER_H

#include <QObject>
#include <QHostAddress>
#include <QDnsLookup>

class QNameServer : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString hostName READ hostName WRITE setHostName NOTIFY hostNameChanged)
    Q_PROPERTY(QHostAddress hostAddress READ hostAddress NOTIFY hostAddressChanged)
    Q_PROPERTY(int port READ port WRITE setPort NOTIFY portChanged)

public:
    explicit QNameServer(QObject *parent = 0);

    QString hostName() const
    {
        return m_hostName;
    }

    QHostAddress hostAddress() const
    {
        return m_hostAddress;
    }

    int port() const
    {
        return m_port;
    }

public slots:
    void setHostName(QString arg)
    {
        if (m_hostName != arg) {
            m_hostName = arg;
            emit hostNameChanged(arg);
            updateHostAddress();
        }
    }

    void setPort(int arg)
    {
        if (m_port != arg) {
            m_port = arg;
            emit portChanged(arg);
        }
    }

private:
    QString m_hostName;
    QHostAddress m_hostAddress;
    int m_port;
    QDnsLookup *m_dnsLookup;

    void updateHostAddress();

private slots:
    void handleServers();

signals:
    void hostNameChanged(QString arg);
    void hostAddressChanged(QHostAddress arg);
    void portChanged(int arg);
};

#endif // QNAMESERVER_H
