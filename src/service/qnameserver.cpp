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

#include "qnameserver.h"

QNameServer::QNameServer(QObject *parent) :
    QObject(parent),
    m_hostName(QString()),
    m_hostAddress(QHostAddress()),
    m_port(5353),
    m_dnsLookup(nullptr)
{
}

void QNameServer::updateHostAddress()
{
    QHostAddress newHostAddress(m_hostName);

    if (!newHostAddress.isNull())
    {
        m_hostAddress = newHostAddress;
    }
    else
    {
        m_hostAddress = QHostAddress();

        if (!m_hostName.isEmpty())
        {
            if (m_dnsLookup != nullptr)
            {
                m_dnsLookup->abort();
                m_dnsLookup->deleteLater();
            }

            m_dnsLookup = new QDnsLookup(this);
            connect(m_dnsLookup, SIGNAL(finished()),
                    this, SLOT(handleServers()));

            m_dnsLookup->setType(QDnsLookup::A);
            m_dnsLookup->setName(m_hostName);
            m_dnsLookup->lookup();
        }
    }

    emit hostAddressChanged(m_hostAddress);
}

void QNameServer::handleServers()
{
   if (m_dnsLookup->error() != QDnsLookup::NoError) // TODO: better error handling
   {
#ifdef QT_DEBUG
      qDebug() << "DNS lookup error:" << m_dnsLookup->errorString();
#endif
       m_dnsLookup->deleteLater();
       m_dnsLookup = nullptr;
       return;
   }

   foreach (const QDnsHostAddressRecord &record, m_dnsLookup->hostAddressRecords())
   {
       m_hostAddress = record.value();  // first wins
       emit hostAddressChanged(m_hostAddress);
       break;
   }

   m_dnsLookup->deleteLater();
   m_dnsLookup = nullptr;
}
