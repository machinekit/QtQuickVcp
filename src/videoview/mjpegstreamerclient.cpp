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
#include "mjpegstreamerclient.h"

MjpegStreamerClient::MjpegStreamerClient(QQuickPaintedItem *parent) :
    QQuickPaintedItem(parent)
{
    this->setRenderTarget(QQuickPaintedItem::FramebufferObject);
    this->setPerformanceHint(QQuickPaintedItem::FastFBOResizing, true);
    this->setAntialiasing(false);

    m_url = "";
    m_fps = 0.0;
    m_running = false;
    m_frameCount = 0;
    m_timestamp = 0.0;
    m_componentCompleted = false;
    m_updateSocket = NULL;

    m_framerateTimer = new QTimer(this);
    connect(m_framerateTimer, SIGNAL(timeout()),
            this, SLOT(updateFramerate()));
    m_framerateTimer->start(1000);

    m_streamBufferTimer = new QTimer(this);
    connect(m_streamBufferTimer, SIGNAL(timeout()),
            this, SLOT(updateStreamBuffer()));
    m_streamBufferTimer->setSingleShot(true);

    m_context = createDefaultContext(this);
    m_context->start();
}

MjpegStreamerClient::~MjpegStreamerClient()
{
    disconnectSocket();
}

/** componentComplete is executed when the QML component is fully loaded */
void MjpegStreamerClient::componentComplete()
{
    m_componentCompleted = true;

    if (m_running == true)    // the component was set to ready before it was completed
    {
        connectSocket();
    }

    QQuickItem::componentComplete();
}

void MjpegStreamerClient::paint(QPainter *painter)
{
    QRect r = this->boundingRect().toRect();

    // Show view finder
    if(!m_frameImg.isNull())
    {
        painter->drawImage(r, m_frameImg, m_frameImg.rect());
    }
}

/** If the running property has a rising edge we try to connect
 *  if it is has a falling edge we disconnect and cleanup
 */
void MjpegStreamerClient::setRunning(bool arg)
{
    if (m_running != arg) {
        m_running = arg;
        emit runningChanged(arg);

        if (m_componentCompleted == false)
        {
            return;
        }

        if (m_running)
        {
            connectSocket();
        }
        else
        {
            disconnectSocket();
        }
    }
}

void MjpegStreamerClient::connectSocket()
{
     m_updateSocket = m_context->createSocket(ZMQSocket::TYP_SUB, this);
     m_updateSocket->setLinger(0);
     m_updateSocket->connectTo(m_url);
     m_updateSocket->subscribeTo("frames");

     connect(m_updateSocket, SIGNAL(messageReceived(QList<QByteArray>)),
             this, SLOT(updateMessageReceived(QList<QByteArray>)));
}

void MjpegStreamerClient::disconnectSocket()
{
    if (m_updateSocket != NULL)
    {
        m_updateSocket->close();
        m_updateSocket->deleteLater();
        m_updateSocket = NULL;
    }
}

void MjpegStreamerClient::updateMessageReceived(QList<QByteArray> messageList)
{
    QByteArray topic;

    topic = messageList.at(0);
    m_rx.ParseFromArray(messageList.at(1).data(), messageList.at(1).size());

    m_streamBuffer.clear();
    m_streamBufferTimer->stop();
    m_currentStreamBufferItem.timestamp = 0;
    for (int i = 0; i < m_rx.frame_size(); ++i)
    {
        pb::Package_Frame frame;
        QByteArray data;
        QDateTime dateTime;
        QTime time;
        StreamBufferItem streamBufferItem;

        frame = m_rx.frame(i);
        data = QByteArray(frame.blob().data(), frame.blob().size());

#ifdef QT_DEBUG
        qDebug() << "received frame" << topic;
        qDebug() << "timestamp: " << frame.timestamp_unix() << frame.timestamp_s() << frame.timestamp_us();
#endif

        streamBufferItem.image = QImage::fromData(data, "JPG");
        streamBufferItem.timestamp = (double)frame.timestamp_s()*1000 +  (double)frame.timestamp_us() / 1000.0;

        dateTime.setMSecsSinceEpoch((quint64)frame.timestamp_unix()*(quint64)1000);
        dateTime = dateTime.toLocalTime();
        time = dateTime.time();
        streamBufferItem.time = time.addMSecs(frame.timestamp_us()/1000.0);

#ifdef QT_DEBUG
        qDebug() << "time: " << streamBufferItem.time;
#endif

        m_streamBuffer.enqueue(streamBufferItem);
    }
    m_currentStreamBufferItem = m_streamBuffer.dequeue();
    updateStreamBuffer();
}

void MjpegStreamerClient::updateFramerate()
{
    m_fps = m_frameCount;
    m_frameCount = 0;
    emit fpsChanged(m_fps);

#ifdef QT_DEBUG
    qDebug() << "fps: " << m_fps;
#endif
}

void MjpegStreamerClient::updateStreamBuffer()
{
    updateStreamBufferItem();

    if (!m_streamBuffer.isEmpty())
    {
        double timestamp;
        double interval;

        timestamp = m_currentStreamBufferItem.timestamp;
        m_currentStreamBufferItem = m_streamBuffer.dequeue();
        interval = m_currentStreamBufferItem.timestamp - timestamp;

        m_streamBufferTimer->start(qMax((int)interval,0));
    }

}

void MjpegStreamerClient::updateStreamBufferItem()
{
    m_frameCount++;
    m_frameImg = m_currentStreamBufferItem.image;
    m_timestamp = m_currentStreamBufferItem.timestamp;
    m_time = m_currentStreamBufferItem.time;

    update();
    emit timestampChanged(m_timestamp);
    emit timeChanged(m_time);
}
