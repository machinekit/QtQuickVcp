#include "qservicediscovery.h"



QServiceDiscovery::QServiceDiscovery(QQuickItem *parent) :
    QQuickItem(parent)
{
    //int port, int instance, int retryTime, int maxWait, bool trace,
    m_port = 10042;
    m_instance = 0;
    m_retryTime = 500;
    m_maxWait = 2000;
    m_trace = false;
    m_running = false;

    m_udpSocket = new QUdpSocket(this);
    connect(m_udpSocket, SIGNAL(readyRead()),
            this,SLOT(udpReadyRead()));

    m_timeoutTimer = new QTimer(this);
    connect(m_timeoutTimer, SIGNAL(timeout()),
            this, SLOT(timeout()));

    m_waitTime = new QTime();
}

void QServiceDiscovery::startDiscovery()
{
    if (m_running)
    {
        return;
    }

    foreach (QService *service, m_services)
    {
        service->clearFound();  // reset the found tag
    }
    m_replies.clear();

    m_udpSocket->close();
    m_udpSocket->bind(m_port, QAbstractSocket::ReuseAddressHint | QAbstractSocket::ShareAddress);
    m_timeoutTimer->setInterval(m_retryTime);
    m_currentWanted = m_services.size();
    m_running = true;
    emit runningChanged(m_running);

    pb::Container tx;
    tx.set_type(pb::MT_SERVICE_PROBE);
    tx.set_trace(m_trace);
    m_probe = QByteArray(QByteArray(tx.SerializeAsString().c_str(), tx.ByteSize()));

    m_waitTime->restart();
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
    m_udpSocket->writeDatagram(m_probe,
                               QHostAddress("255.255.255.255"),
                               m_port);
}

QQmlListProperty<QService> QServiceDiscovery::services()
{
    return QQmlListProperty<QService>(this, m_services);
}

int QServiceDiscovery::serviceCount() const
{
    return m_services.count();
}

QService *QServiceDiscovery::service(int index) const
{
    return m_services.at(index);
}

QQmlListProperty<QService> QServiceDiscovery::replies()
{
    return QQmlListProperty<QService>(this, m_replies);
}

int QServiceDiscovery::replieCount() const
{
    return m_replies.count();
}

QService *QServiceDiscovery::replie(int index) const
{
    return m_replies.at(index);
}

void QServiceDiscovery::udpReadyRead()
{
    QByteArray datagram;
    pb::Container rx;

    while (m_udpSocket->hasPendingDatagrams())
    {
       datagram.resize(m_udpSocket->pendingDatagramSize());
       m_udpSocket->readDatagram(datagram.data(), datagram.size());

        if (datagram.isEmpty())
        {
            return;
        }

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
                    QService *receivedService = new QService(this);

                    announcement = rx.service_announcement(i);
                    receivedService->setData(QString::fromStdString(announcement.uri()),
                                            announcement.version(),
                                            (QService::ServiceApi)announcement.api(),
                                            QString::fromStdString(announcement.description()),
                                            true);
                    receivedService->setType((QService::ServiceType)announcement.stype());

                    foreach(QService *service, m_services)
                    {
                        if ((service->type() == receivedService->type())
                            && (receivedService->version() >= service->minVersion()))
                        {
                            if (service->isFound() == false)
                            {
                                m_currentWanted--;
                            }

                            // last wins
                            service->setData(receivedService->uri(),
                                             receivedService->version(),
                                             receivedService->api(),
                                             receivedService->description(),
                                             true);
                        }
                    }

                    m_replies.append(receivedService);

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
}

void QServiceDiscovery::timeout()
{
    if (m_waitTime->elapsed() > m_maxWait) // test for timeout
    {
        stopDiscovery();
        emit discoveryTimeout();
    }
    else
    {
        if (m_trace)
        {
            qDebug() << "wait:" << m_waitTime->elapsed() << "resending probe";
        }
        sendBroadcast();    // resend the broadcast
    }
}

void QServiceDiscovery::setTrace(bool arg)
{
    if (m_trace != arg) {
        m_trace = arg;
        emit traceChanged(arg);
    }
}

