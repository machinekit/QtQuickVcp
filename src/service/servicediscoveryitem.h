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
#ifndef SERVICEDISCOVERYITEM_H
#define SERVICEDISCOVERYITEM_H

#include <QObject>
#include <QHostAddress>
#include <QDateTime>
#include <QSet>

namespace qtquickvcp {

class ServiceDiscoveryItem : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString type READ type NOTIFY typeChanged)
    Q_PROPERTY(QString name READ name NOTIFY nameChanged)
    Q_PROPERTY(QString uri READ uri NOTIFY uriChanged)
    Q_PROPERTY(QString uuid READ uuid NOTIFY uuidChanged)
    Q_PROPERTY(int version READ version NOTIFY versionChanged)
    Q_PROPERTY(int port READ port NOTIFY portChanged)
    Q_PROPERTY(QString hostName READ hostName NOTIFY hostNameChanged)
    Q_PROPERTY(QString hostAddress READ hostAddress NOTIFY hostAddressChanged)
    Q_PROPERTY(QStringList txtRecords READ txtRecords NOTIFY txtRecordsChanged)
    Q_PROPERTY(bool updated READ updated WRITE setUpdated NOTIFY updatedChanged)

public:
    explicit ServiceDiscoveryItem(QObject *parent = 0);

    QString uri() const;
    int port() const;
    QString hostName() const;
    QString hostAddress() const;
    QString name() const;
    QString type() const;
    QStringList txtRecords() const;
    QString uuid() const;
    QSet<int> outstandingRequests() const;
    bool hasOutstandingRequests();
    int version() const;
    bool updated() const;
    int errorCount() const;

public slots:
    void setUri(const QString &arg);
    void setPort(int arg);
    void setHostName(const QString &arg);
    void setHostAddress(const QString &arg);
    void setName(const QString &arg);
    void setType(const QString &arg);
    void setTxtRecords(const QStringList &arg);
    void setUuid(const QString &arg);
    void addOutstandingRequest(int arg);
    void removeOutstandingRequest(int arg);
    void clearOutstandingRequests();
    void setVersion(int arg);
    void setUpdated(bool arg);
    void setErrorCount(int errorCount);
    void resetErrorCount();
    void increaseErrorCount();

private:
    QString m_name;
    QString m_type;
    QString m_uri;
    QString m_uuid;
    int m_version;
    int m_port;
    QString m_hostName;
    QString m_hostAddress;
    QStringList m_txtRecords;
    QSet<int> m_outstandingRequests;
    bool m_updated;
    int m_errorCount;

signals:
    void uriChanged(const QString &arg);
    void portChanged(int arg);
    void hostNameChanged(const QString &hostName);
    void hostAddressChanged(const QString &arg);
    void nameChanged(const QString &arg);
    void typeChanged(const QString &arg);
    void txtRecordsChanged(const QStringList &arg);
    void uuidChanged(const QString &arg);
    void versionChanged(int arg);
    void updatedChanged(bool arg);
}; // class ServiceDiscoveryItem
} // namespace qtquickvcp

#endif // SERVICEDISCOVERYITEM_H
