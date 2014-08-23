#include "qnameserver.h"

QNameServer::QNameServer(QObject *parent) :
    QObject(parent),
    m_hostName(QString()),
    m_hostAddress(QHostAddress()),
    m_port(5353),
    m_dnsLookup(NULL)
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
            if (m_dnsLookup != NULL)
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
       m_dnsLookup = NULL;
       return;
   }

   foreach (const QDnsHostAddressRecord &record, m_dnsLookup->hostAddressRecords())
   {
       m_hostAddress = record.value();  // first wins
       emit hostAddressChanged(m_hostAddress);
       break;
   }

   m_dnsLookup->deleteLater();
   m_dnsLookup = NULL;
}
