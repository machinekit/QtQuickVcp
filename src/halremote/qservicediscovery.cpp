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
#include "qservicediscovery.h"

/** Service discovery class implemented for use with C++ or QML.
 */
QServiceDiscovery::QServiceDiscovery(QQuickItem *parent) :
    QQuickItem(parent)
{
    m_port = 10042;
    m_instance = 0;
    m_retryTime = 500;
    m_maxWait = 2000;
    m_trace = false;
    m_running = false;
    m_repeat = false;

    m_udpSocket = new QUdpSocket(this);
    connect(m_udpSocket, SIGNAL(readyRead()),
            this,SLOT(udpReadyRead()));

    m_timeoutTimer = new QTimer(this);
    connect(m_timeoutTimer, SIGNAL(timeout()),
            this, SLOT(timeout()));

    m_waitTime = new QTime();
}

/** componentComplete is executed when the QML component is fully loaded */
void QServiceDiscovery::componentComplete()
{
    if (m_running)  // If the the component is set to running inside QML we start the discovery at startup
    {
        startDiscovery();
    }
}

/** This function starts the service discovery process. It is non blocking. */
void QServiceDiscovery::startDiscovery()
{
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

/** This function either stops the service discovery process if
 *  a service discovery is running or restarts it if repeat is set to true
 */
void QServiceDiscovery::stopDiscovery()
{
    if (!m_repeat)
    {
        m_timeoutTimer->stop();
        m_running = false;
        emit runningChanged(m_running);
    }
    else
    {
        startDiscovery();
    }
}

/** Sends a single broadcast if probe was generated (a discovery must have
 *  been issued previously)
 */
void QServiceDiscovery::sendBroadcast()
{
    if (m_probe.isEmpty())
    {
        return;
    }

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

/** This function processes all responses received over the UDP socket */
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
                    receivedService->setData(QString::fromStdString(announcement.uri()),        // URI
                                            announcement.version(),                             // version
                                            (QService::ServiceApi)announcement.api(),           // API
                                            QString::fromStdString(announcement.description()), // Description
                                            true);                                              // found
                    receivedService->setType((QService::ServiceType)announcement.stype());

                    // Iterate through all serives and update all services suitable for the announcement
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

/** The function called by the timeout timer
 *  Checks the amount of time passed by since the last broadcast
 */
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

void QServiceDiscovery::setRunning(bool arg)
{
    if (m_running != arg) {
        m_running = arg;
        emit runningChanged(arg);

        if (m_running)
        {
            startDiscovery();
        }
        else
        {
            stopDiscovery();
        }
    }
}

