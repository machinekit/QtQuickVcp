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
#ifndef QSERVICE_H
#define QSERVICE_H

#include <QObject>
#include <QQmlListProperty>
#include "qservicediscoveryitem.h"
#include "qservicediscoveryfilter.h"

class QService : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString type READ type WRITE setType NOTIFY typeChanged)
    Q_PROPERTY(QString name READ name NOTIFY nameChanged)
    Q_PROPERTY(QString uri READ uri NOTIFY uriChanged)
    Q_PROPERTY(QString uuid READ uuid NOTIFY uuidChanged)
    Q_PROPERTY(bool ready READ isReady NOTIFY readyChanged)
    Q_PROPERTY(QServiceDiscoveryFilter *filter READ filter WRITE setFilter NOTIFY filterChanged)
    Q_PROPERTY(QQmlListProperty<QServiceDiscoveryItem> serviceDiscoveryItems READ serviceDiscoveryItems NOTIFY serviceDiscoveryItemsChanged)

public:
    explicit QService(QObject *parent = 0);

    QQmlListProperty<QServiceDiscoveryItem> serviceDiscoveryItems();
    int serviceDiscoveryItemCount() const;
    QServiceDiscoveryItem *serviceDiscoveryItem(int index) const;
    void setServiceDiscoveryItems(QList<QServiceDiscoveryItem*> newServiceDiscoveryItems);

    bool isReady() const
    {
        return m_ready;
    }

    QString type() const
    {
        return m_type;
    }

    QString name() const
    {
        return m_name;
    }

    QServiceDiscoveryFilter *filter() const
    {
        return m_filter;
    }

    QString uuid() const
    {
        return m_uuid;
    }

    QString uri() const
    {
        return m_uri;
    }

public slots:

    void setType(QString arg)
    {
        if (m_type != arg) {
            m_type = arg;
            emit typeChanged(arg);
        }
    }

    void setFilter(QServiceDiscoveryFilter *arg)
    {
        if (m_filter != arg) {
            m_filter = arg;
            emit filterChanged(arg);
        }
    }

private:
    bool m_ready;
    QString m_type;
    QString m_name;
    QString m_uuid;
    QString m_uri;
    QServiceDiscoveryFilter *m_filter;
    QList<QServiceDiscoveryItem*> m_serviceDiscoveryItems;


signals:
    void readyChanged(bool arg);
    void typeChanged(QString arg);
    void nameChanged(QString arg);
    void serviceDiscoveryItemsChanged(QQmlListProperty<QServiceDiscoveryItem> arg);
    void filterChanged(QServiceDiscoveryFilter *arg);
    void uuidChanged(QString arg);
    void uriChanged(QString arg);
};

#endif // QSERVICE_H
