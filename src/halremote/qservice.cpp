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
#include "qservice.h"

QService::QService(QObject *parent) :
    QObject(parent)
{
    m_uri = "";
    m_uuid = "";
    m_version = 0;
    m_minVersion = 0;
    m_ready = false;
    m_filter = new QServiceDiscoveryFilter(this);
}

QQmlListProperty<QServiceDiscoveryItem> QService::serviceDiscoveryItems()
{
    return QQmlListProperty<QServiceDiscoveryItem>(this, m_serviceDiscoveryItems);
}

int QService::serviceDiscoveryItemCount() const
{
    return m_serviceDiscoveryItems.count();
}

QServiceDiscoveryItem *QService::serviceDiscoveryItem(int index) const
{
    return m_serviceDiscoveryItems.at(index);
}

void QService::setServiceDiscoveryItems(QList<QServiceDiscoveryItem *> newServiceDiscoveryItems)
{
    m_serviceDiscoveryItems = newServiceDiscoveryItems;

    if (m_serviceDiscoveryItems.count() > 0)
    {
        m_uri = m_serviceDiscoveryItems.at(0)->uri();
        m_uuid = m_serviceDiscoveryItems.at(0)->uuid();
        m_name = m_serviceDiscoveryItems.at(0)->name();
        m_ready = true;
    }
    else
    {
        m_ready = false;
    }

    emit uriChanged(m_uri);
    emit uuidChanged(m_uuid);
    emit nameChanged(m_name);
    //emit versionChanged(m_version);
    emit readyChanged(m_ready);
    emit serviceDiscoveryItemsChanged(serviceDiscoveryItems());
}
