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
#ifndef MJPEGSTREAMER2_H
#define MJPEGSTREAMER2_H

#include <QQuickPaintedItem>
#include <QPainter>
#include <QNetworkAccessManager>
#include <QTimer>
#include <QNetworkReply>
#include <QElapsedTimer>
#include <QQueue>
#include <QTime>
#include <nzmqt/nzmqt.hpp>
#include "package.pb.h"
#include <google/protobuf/text_format.h>

class QMjpegStreamerClient : public QQuickPaintedItem
{
    Q_OBJECT
    Q_PROPERTY(QString videoUri READ videoUri WRITE setVideoUri NOTIFY videoUriChanged)
    Q_PROPERTY(int fps READ fps NOTIFY fpsChanged)
    Q_PROPERTY(bool ready READ isReady WRITE setReady NOTIFY readyChanged)
    Q_PROPERTY(double timestamp READ timestamp NOTIFY timestampChanged)
    Q_PROPERTY(QTime time READ time NOTIFY timeChanged)
    Q_PROPERTY(Qt::AspectRatioMode aspectRatioMode READ aspectRatioMode WRITE setAspectRatioMode NOTIFY aspectRatioModeChanged)

public:
    typedef struct {
        QImage image;
        double timestamp;
        QTime  time;
    } StreamBufferItem;

    explicit QMjpegStreamerClient(QQuickPaintedItem *parent = 0);
    ~QMjpegStreamerClient();
    virtual void componentComplete();
    void paint(QPainter* painter);

    QString videoUri() const
    {
        return m_videoUri;
    }

    bool isReady() const
    {
        return m_running;
    }

    int fps() const
    {
        return m_fps;
    }

    double timestamp() const
    {
        return m_timestamp;
    }

    QTime time() const
    {
        return m_time;
    }

    Qt::AspectRatioMode aspectRatioMode() const
    {
        return m_aspectRatioMode;
    }

signals:
    void videoUriChanged(QString arg);
    void readyChanged(bool arg);
    void fpsChanged(double arg);
    void timestampChanged(double arg);
    void timeChanged(QTime arg);

    void aspectRatioModeChanged(Qt::AspectRatioMode arg);

public slots:

void setVideoUri(QString arg)
{
    if (m_videoUri != arg) {
        m_videoUri = arg;
        emit videoUriChanged(arg);
    }
}

void setReady(bool arg);

void setAspectRatioMode(Qt::AspectRatioMode arg)
{
    if (m_aspectRatioMode == arg)
        return;

    m_aspectRatioMode = arg;
    emit aspectRatioModeChanged(arg);
}

private:
    bool        m_componentCompleted;
    StreamBufferItem m_currentStreamBufferItem;
    QQueue<StreamBufferItem> m_streamBuffer;
    QImage      m_frameImg;
    QTimer     *m_framerateTimer;
    QTimer     *m_streamBufferTimer;
    nzmqt::ZMQContext *m_context;
    nzmqt::ZMQSocket *m_updateSocket;
    pb::Package m_rx; // more efficient to reuse a protobuf Message


    QString m_videoUri;
    bool    m_running;
    int     m_fps;
    int     m_frameCount;
    double  m_timestamp;
    QTime   m_time;
    Qt::AspectRatioMode m_aspectRatioMode;

private slots:
    void start();
    void stop();
    void connectSocket();
    void disconnectSocket();
    void updateMessageReceived(const QList<QByteArray> &messageList);

    void updateFramerate();
    void updateStreamBuffer();
    void updateStreamBufferItem();
};

#endif // MJPEGSTREAMER2_H
