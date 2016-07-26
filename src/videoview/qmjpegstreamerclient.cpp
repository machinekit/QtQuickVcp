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
#include "qmjpegstreamerclient.h"

/*!
    \qmltype MjpegStreamerClient
    \instantiates QMjpegStreamerClient
    \inqmlmodule Machinekit.HalRemote
    \brief Client for a MJPEG video stream
    \ingroup videoview

    The MjpegStreamerClient component can display MJPG video streams captured
    by the application mjpeg-streamer. For that purpose mjpeg-streamer needs
    to be run with the ZeroMQ output plugin.

    \qml
    import Machinekit.HalRemote 1.0
    import Machinekit.HalRemote.Controls 1.0

    HalApplicationWindow {
        services: [
            Service {
                id: videoService
                type: "video"
                filter: ServiceDiscoveryFilter {
                    name: "Webcam1"
                }
            }
        ]

        MjpegStreamerClient {
            id: mjpegStreamerClient

            anchors.fill: parent
            videoUri: videoService.uri
            ready: videoService.ready
        }
    }
    \endqml
*/

/*! \qmlproperty string MjpegStreamerClient::videoUri

     This property holds the video service uri.
*/

/*! \qmlproperty bool MjpegStreamerClient::ready

    This property holds whether the component is ready or not.
    If the property is set to \c true the component will try to connect. If the
    property is set to \c false all connections will be closed.

    The default value is \c{false}.
*/

/*! \qmlproperty int MjpegStreamerClient::fps

    This property holds the current framerate of the video in fps.
*/

/*! \qmlproperty double MjpegStreamerClient::timestamp

    This property holds the timestamp in ms of the current displayed video frame.
*/

/*! \qmlproperty time MjpegStreamerClient::time

    This property holds the time the current displayed video frame was taken.
*/

/* \qmlproperty enumeration MjpegStreamerClient::aspectRatioMode
 *
 *  This property specifies how the video image should be scaled.
 *
 *  The default value is \c{Qt::KeepAspectRatio}
 */

QMjpegStreamerClient::QMjpegStreamerClient(QQuickPaintedItem *parent) :
    QQuickPaintedItem(parent),
    m_componentCompleted(false),
    m_framerateTimer(new QTimer(this)),
    m_streamBufferTimer(new QTimer(this)),
    m_context(nullptr),
    m_updateSocket(nullptr),
    m_videoUri(""),
    m_running(false),
    m_fps(0.0),
    m_frameCount(0),
    m_timestamp(0.0),
    m_time(QTime()),
    m_aspectRatioMode(Qt::KeepAspectRatio)
{
    this->setRenderTarget(QQuickPaintedItem::FramebufferObject);
    this->setPerformanceHint(QQuickPaintedItem::FastFBOResizing, true);
    this->setAntialiasing(false);
    this->setOpaquePainting(true);

    connect(m_framerateTimer, SIGNAL(timeout()),
            this, SLOT(updateFramerate()));
    m_framerateTimer->setInterval(1000);

    connect(m_streamBufferTimer, SIGNAL(timeout()),
            this, SLOT(updateStreamBuffer()));
    m_streamBufferTimer->setSingleShot(true);
}

QMjpegStreamerClient::~QMjpegStreamerClient()
{
    disconnectSocket();
}

/** componentComplete is executed when the QML component is fully loaded */
void QMjpegStreamerClient::componentComplete()
{
    m_componentCompleted = true;

    if (m_running == true)    // the component was set to ready before it was completed
    {
        start();
    }

    QQuickPaintedItem::componentComplete();
}

void QMjpegStreamerClient::paint(QPainter *painter)
{
    QRect boundingRect = this->boundingRect().toRect();

    // Show view finder
    if(!m_frameImg.isNull())
    {
        QImage scaledImage = m_frameImg.scaled(boundingRect.size(), m_aspectRatioMode);
        QRect drawRect(scaledImage.rect());
        drawRect.moveCenter(boundingRect.center());
        painter->drawImage(drawRect, scaledImage, scaledImage.rect());
    }
}

/** If the running property has a rising edge we try to connect
 *  if it is has a falling edge we disconnect and cleanup
 */
void QMjpegStreamerClient::setReady(bool arg)
{
    if (m_running != arg) {
        m_running = arg;
        emit readyChanged(arg);

        if (m_componentCompleted == false)
        {
            return;
        }

        if (m_running)
        {
            start();
        }
        else
        {
            stop();
        }
    }
}

void QMjpegStreamerClient::start()
{
    m_framerateTimer->start();
    connectSocket();
}

void QMjpegStreamerClient::stop()
{
    m_framerateTimer->stop();
    disconnectSocket();
}

void QMjpegStreamerClient::connectSocket()
{
    m_context = createDefaultContext(this, 1);
    m_context->start();

    m_updateSocket = m_context->createSocket(ZMQSocket::TYP_SUB, this);
    m_updateSocket->setLinger(0);
    m_updateSocket->connectTo(m_videoUri);
    m_updateSocket->subscribeTo("frames");

    connect(m_updateSocket, SIGNAL(messageReceived(QList<QByteArray>)),
         this, SLOT(updateMessageReceived(QList<QByteArray>)));
}

void QMjpegStreamerClient::disconnectSocket()
{
    if (m_updateSocket != nullptr)
    {
        m_updateSocket->close();
        m_updateSocket->deleteLater();
        m_updateSocket = nullptr;
    }

    if (m_context != nullptr)
    {
        m_context->stop();
        m_context->deleteLater();
        m_context = nullptr;
    }
}

void QMjpegStreamerClient::updateMessageReceived(QList<QByteArray> messageList)
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

void QMjpegStreamerClient::updateFramerate()
{
    m_fps = m_frameCount;
    m_frameCount = 0;
    emit fpsChanged(m_fps);

#ifdef QT_DEBUG
    qDebug() << "fps: " << m_fps;
#endif
}

void QMjpegStreamerClient::updateStreamBuffer()
{
    updateStreamBufferItem();

    if (!m_streamBuffer.isEmpty())
    {
        double timestamp;
        double interval;

        timestamp = m_currentStreamBufferItem.timestamp;
        m_currentStreamBufferItem = m_streamBuffer.dequeue();
        interval = m_currentStreamBufferItem.timestamp - timestamp;

        m_streamBufferTimer->start(qMax((int)interval, 0));
    }

}

void QMjpegStreamerClient::updateStreamBufferItem()
{
    m_frameCount++;
    m_frameImg = m_currentStreamBufferItem.image;
    m_timestamp = m_currentStreamBufferItem.timestamp;
    m_time = m_currentStreamBufferItem.time;

    update();
    emit timestampChanged(m_timestamp);
    emit timeChanged(m_time);
}
