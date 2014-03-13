#include "qservicediscovery.h"



QServiceDiscovery::QServiceDiscovery(int port, int instance, int retryTime, int maxWait, bool trace, QObject *parent)
{
    m_port = port;
    m_instance = instance;
    m_wanted = 0;
    m_retryTime = retryTime;
    m_maxWait = maxWait;
    m_trace = trace;
    m_running = false;

    pb::Container tx;
    tx.set_type(pb::MT_SERVICE_PROBE);
    tx.set_trace(trace);
    m_probe = QString::fromStdString(tx.SerializeAsString());

    m_udpSocket = new QUdpSocket(this);
    m_udpSocket->bind(port, QAbstractSocket::ReuseAddressHint);
    connect(m_udpSocket, SIGNAL(readyRead()),
            this,SLOT(udpReadyRead()));

    m_timeoutTimer = new QTimer(this);
    m_timeoutTimer->setInterval(retryTime);
    connect(m_timeoutTimer, SIGNAL(timeout()),
            this, SLOT(timeout()));

    m_waitTime = new QTime();
}

void QServiceDiscovery::addService(ServiceType type, int minVersion)
{
    Service service;

    service.minVersion = minVersion;
    m_services[type] = service;
    m_wanted += 1;

    emit servicesChanged(m_services);
}

void QServiceDiscovery::removeService(QServiceDiscovery::ServiceType type)
{
    if (!m_services.keys().contains(type))
    {
        return; // service not added
    }

    m_services.remove(type);
    emit servicesChanged(m_services);
}

void QServiceDiscovery::startDiscovery()
{
    if (m_running)
    {
        return;
    }

    m_replies.clear();
    emit repliesChanged(m_replies);

    m_currentWanted = m_wanted;
    m_running = true;

    m_waitTime->start();
    m_timeoutTimer->start();
    // initial probe request
    sendBroadcast();
}

void QServiceDiscovery::stopDiscovery()
{
    m_timeoutTimer->stop();
    m_running = false;
    emit runningChanged(m_running);
}

void QServiceDiscovery::sendBroadcast()
{
    m_udpSocket->writeDatagram(m_probe.toLocal8Bit(),
                               QHostAddress("255.255.255.255"),
                               m_port);
}

void QServiceDiscovery::udpReadyRead()
{
    QByteArray datagram;
    pb::Container rx;

    datagram = m_udpSocket->readAll();

    if (rx.ParseFromArray(datagram.data(), datagram.size()))
    {
        if (rx.type() == pb::MT_SERVICE_PROBE)
        {
            return; // skip requests
        }

        if (m_trace)
        {
            qDebug() << "got reply=" << datagram << "msg=" << QString::fromStdString(rx.SerializeAsString());
        }

        if (rx.type() == pb::MT_SERVICE_ANNOUNCEMENT)
        {
            for (int i = 0; i < rx.service_announcement_size(); ++i)
            {
                pb::ServiceAnnouncement announcement;
                Service service;
                ServiceType stype;

                announcement = rx.service_announcement(i);
                service.uri = QString::fromStdString(announcement.uri());
                service.version = announcement.version();
                service.api = (ServiceApi)announcement.api();
                service.description = QString::fromStdString(announcement.description());
                service.found = true;
                stype = (ServiceType)announcement.stype();

                if ((!m_replies.keys().contains(stype))
                        && (m_services.keys().contains(stype)))
                {
                    m_currentWanted--;
                }

                m_replies[stype] = service;  // last wins
                emit repliesChanged(m_replies);

                if (m_services.keys().contains(stype))
                {
                    m_services[stype] = service;
                    emit servicesChanged(m_services);
                }

                if (m_currentWanted == 0)
                {
                    stopDiscovery();
                    emit discoverySucceded();
                }
            }
        }
    }
    else
    {
        // we have a error
    }
}

void QServiceDiscovery::timeout()
{
    if (m_waitTime->msec() > m_maxWait) // test for timeout
    {
        stopDiscovery();
        emit discoveryTimeout();
    }
    else
    {
        sendBroadcast();    // resend the broadcast
    }
}

