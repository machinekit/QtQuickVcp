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
    Q_PROPERTY(int version READ version NOTIFY versionChanged)
    Q_PROPERTY(bool ready READ isReady NOTIFY readyChanged)
    Q_PROPERTY(QServiceDiscoveryFilter *filter READ filter WRITE setFilter NOTIFY filterChanged)
    Q_PROPERTY(QQmlListProperty<QServiceDiscoveryItem> items READ items NOTIFY itemsChanged)
    Q_PROPERTY(bool required READ required WRITE setRequired NOTIFY requiredChanged)

public:
    explicit QService(QObject *parent = 0);

    QQmlListProperty<QServiceDiscoveryItem> items();
    int serviceDiscoveryItemCount() const;
    QServiceDiscoveryItem *serviceDiscoveryItem(int index) const;
    void setItems(QList<QServiceDiscoveryItem*> newServiceDiscoveryItems);

    QString uri() const
    {
        return m_uri;
    }

    int version() const
    {
        return m_version;
    }

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

    bool required() const
    {
        return m_required;
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

    void setRequired(bool arg)
    {
        if (m_required == arg)
            return;

        m_required = arg;
        emit requiredChanged(arg);
    }

private:
    QString m_type;
    QString m_name;
    QString m_uri;
    QString m_uuid;
    int m_version;
    bool m_ready;
    QServiceDiscoveryFilter *m_filter;
    QList<QServiceDiscoveryItem *> m_items;
    bool m_required;

signals:
    void uriChanged(QString arg);
    void versionChanged(int arg);
    void readyChanged(bool arg);
    void typeChanged(QString arg);
    void nameChanged(QString arg);
    void itemsChanged(QQmlListProperty<QServiceDiscoveryItem> arg);
    void filterChanged(QServiceDiscoveryFilter *arg);
    void uuidChanged(QString arg);
    void requiredChanged(bool arg);
};

#endif // QSERVICE_H
