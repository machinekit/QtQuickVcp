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
#include "servicediscoveryitem.h"

namespace qtquickvcp {

class ServiceDiscoveryFilter : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString name READ name WRITE setName NOTIFY nameChanged)
    Q_PROPERTY(QStringList txtRecords READ txtRecords WRITE setTxtRecords NOTIFY txtRecordsChanged)

public:
    explicit ServiceDiscoveryFilter(QObject *parent = 0);

    QString name() const;
    QStringList txtRecords() const;

    /** Apply filter on passed item.
     * @arg item Item to be checked for filtering.
     * @result True if filter matches else false.
     */
    bool apply(const ServiceDiscoveryItem &item) const;

public slots:
    void setName(const QString &arg);
    void setTxtRecords(const QStringList &arg);

private:
    QString m_name;
    QStringList m_txtRecords;

signals:
    void nameChanged(const QString &arg);
    void txtRecordsChanged(const QStringList &arg);

}; // class ServiceDiscoveryFilter
} // namespace qtquickvcp

#endif // SERVICEDISCOVERYFILTER_H
