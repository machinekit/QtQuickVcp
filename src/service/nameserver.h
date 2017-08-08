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

#ifndef NAMESERVER_H
#define NAMESERVER_H

#include <QObject>
#include <QHostAddress>
#include <QDnsLookup>
#include <QPointer>

namespace qtquickvcp {

class NameServer : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString hostName READ hostName WRITE setHostName NOTIFY hostNameChanged)
    Q_PROPERTY(QHostAddress hostAddress READ hostAddress NOTIFY hostAddressChanged)
    Q_PROPERTY(int port READ port WRITE setPort NOTIFY portChanged)

public:
    explicit NameServer(QObject *parent = 0);

    QString hostName() const;
    QHostAddress hostAddress() const;
    int port() const;

public slots:
    void setHostName(const QString &arg);
    void setPort(int arg);

private:
    QString m_hostName;
    QHostAddress m_hostAddress;
    int m_port;
    QPointer<QDnsLookup> m_dnsLookup;

    void updateHostAddress();

private slots:
    void handleServers();

signals:
    void hostNameChanged(const QString &arg);
    void hostAddressChanged(const QHostAddress &arg);
    void portChanged(int arg);

}; // class NameServer
} // namespace qtquickvcp

#endif // NAMESERVER_H
