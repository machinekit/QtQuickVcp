#ifndef QAPPDISCOVERYITEM_H
#define QAPPDISCOVERYITEM_H

#include <QQuickItem>
#include <QHostAddress>
#include <QDateTime>

class QAppDiscoveryItem : public QQuickItem
{
    Q_OBJECT
    Q_PROPERTY(QString name READ name WRITE setName NOTIFY nameChanged)
    Q_PROPERTY(QString dsname READ dsname WRITE setDsname NOTIFY dsNameChanged)
    Q_PROPERTY(int port READ port WRITE setPort NOTIFY portChanged)
    Q_PROPERTY(QHostAddress hostAddress READ hostAddress WRITE setHostAddress NOTIFY hostAddressChanged)
    Q_PROPERTY(QDateTime expiryDate READ expiryDate WRITE setExpiryDate NOTIFY expiryDateChanged)

public:
    explicit QAppDiscoveryItem(QQuickItem *parent = 0);


QString dsname() const
{
    return m_dsname;
}

int port() const
{
    return m_port;
}

QHostAddress hostAddress() const
{
    return m_hostAddress;
}

QString name() const
{
    return m_name;
}

QDateTime expiryDate() const
{
    return m_expiryDate;
}

public slots:

void setDsname(QString arg)
{
    if (m_dsname != arg) {
        m_dsname = arg;
        emit dsNameChanged(arg);
    }
}
void setPort(int arg)
{
    if (m_port != arg) {
        m_port = arg;
        emit portChanged(arg);
    }
}
void setHostAddress(QHostAddress arg)
{
    if (m_hostAddress != arg) {
        m_hostAddress = arg;
        emit hostAddressChanged(arg);
    }
}

void setName(QString arg)
{
    if (m_name != arg) {
        m_name = arg;
        emit nameChanged(arg);
    }
}

void setExpiryDate(QDateTime arg)
{
    if (m_expiryDate != arg) {
        m_expiryDate = arg;
        emit expiryDateChanged(arg);
    }
}

private:
    QString m_dsname;
    int m_port;
    QHostAddress m_hostAddress;
    QString m_name;
    QDateTime m_expiryDate;

signals:

void dsNameChanged(QString arg);
void portChanged(int arg);
void hostAddressChanged(QHostAddress arg);
void nameChanged(QString arg);
void expiryDateChanged(QDateTime arg);
};

#endif // QAPPDISCOVERYITEM_H
