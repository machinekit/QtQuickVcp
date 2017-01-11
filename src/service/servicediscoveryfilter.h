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
#ifndef SERVICEDISCOVERYFILTER_H
#define SERVICEDISCOVERYFILTER_H

#include <QObject>
#include <QQmlListProperty>

namespace qtquickvcp {

class ServiceDiscoveryFilter : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString name READ name WRITE setName NOTIFY nameChanged)
    Q_PROPERTY(QStringList txtRecords READ txtRecords WRITE setTxtRecords NOTIFY txtRecordsChanged)

public:
    explicit ServiceDiscoveryFilter(QObject *parent = 0);

    QString name() const
    {
        return m_name;
    }

    QStringList txtRecords() const
    {
        return m_txtRecords;
    }

public slots:
    void setName(QString arg)
    {
        if (m_name != arg) {
            m_name = arg;
            emit nameChanged(arg);
        }
    }

    void setTxtRecords(QStringList arg)
    {
        if (m_txtRecords != arg) {
            m_txtRecords = arg;
            emit txtRecordsChanged(arg);
        }
    }

private:
    QString m_name;
    QStringList m_txtRecords;

signals:
    void nameChanged(QString arg);
    void txtRecordsChanged(QStringList arg);
}; // class ServiceDiscoveryFilter
} // namespace qtquickvcp

#endif // SERVICEDISCOVERYFILTER_H
