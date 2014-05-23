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
    QObject(parent),
    m_ready(false),
    m_type(""),
    m_name(""),
    m_filter(new QServiceDiscoveryFilter(this))
{
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
    QList<QByteArray> registeredProperties; // used to keep tracked of nonexistent properties that where previusly register
    registeredProperties = this->dynamicPropertyNames();

    m_serviceDiscoveryItems = newServiceDiscoveryItems;

    if (m_serviceDiscoveryItems.count() > 0)
    {
        m_name = m_serviceDiscoveryItems.at(0)->name();
        m_ready = true;

        // Register all text records as properties
        foreach (QString record, m_serviceDiscoveryItems.at(0)->txtRecords())
        {
            QStringList keyValue;
            QByteArray propertyName;

            keyValue = record.split('=');

            if (keyValue.count() < 2) // record without = is a wrong record -> skip
            {
                continue;
            }

            propertyName = keyValue.at(0).toLocal8Bit();
            this->setProperty(propertyName, keyValue.at(1));

            for (int i = 0; i < registeredProperties.count(); ++i)
            {
                if (registeredProperties.at(i) == propertyName)
                {
                    registeredProperties.removeAt(i);   // remove set property from list
                }
            }
        }
    }
    else
    {
        m_name = "";
        m_ready = false;
    }

    for (int i = 0; i < registeredProperties.size(); ++i)
    {
        this->setProperty(registeredProperties.at(i), QVariant());  // delete by setting an invalid variant
    }

    emit nameChanged(m_name);
    emit readyChanged(m_ready);
    emit serviceDiscoveryItemsChanged(serviceDiscoveryItems());
}
