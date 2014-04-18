#ifndef MJPEGSTREAMER2_H
#define MJPEGSTREAMER2_H

#include <QQuickItem>
#include <QQuickPaintedItem>
#include <QPainter>
#include <QNetworkAccessManager>
#include <QTimer>
#include <QNetworkReply>
#include <QElapsedTimer>
#include <QQueue>
#include <nzmqt/nzmqt.hpp>
#include "package.pb.h"
#include <google/protobuf/text_format.h>

namespace gpb = google::protobuf;

using namespace nzmqt;

class MjpegStreamerClient : public QQuickPaintedItem
{
    Q_OBJECT
    Q_PROPERTY(QString url READ url WRITE setUrl NOTIFY urlChanged)
    Q_PROPERTY(int fps READ fps NOTIFY fpsChanged)
    Q_PROPERTY(bool running READ isRunning WRITE setRunning NOTIFY runningChanged)
    Q_PROPERTY(double timestamp READ timestamp NOTIFY timestampChanged)
    Q_PROPERTY(QTime time READ time NOTIFY timeChanged)
public:
    typedef struct {
        QImage image;
        double timestamp;
        QTime  time;
    } StreamBufferItem;

    explicit MjpegStreamerClient(QQuickPaintedItem *parent = 0);
    virtual void componentComplete();
    void paint(QPainter* painter);

    QString url() const
    {
        return m_url;
    }

    bool isRunning() const
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

signals:

    void urlChanged(QString arg);

    void runningChanged(bool arg);

    void fpsChanged(double arg);

    void timestampChanged(double arg);

    void timeChanged(QTime arg);

public slots:

void setUrl(QString arg)
{
    if (m_url != arg) {
        m_url = arg;
        emit urlChanged(arg);
    }
}

void setRunning(bool arg);

private:
    StreamBufferItem m_currentStreamBufferItem;
    QQueue<StreamBufferItem> m_streamBuffer;
    QImage      m_frameImg;
    QTimer     *m_framerateTimer;
    QTimer     *m_streamBufferTimer;
    ZMQContext *m_context;
    ZMQSocket  *m_updateSocket;
    pb::Package m_rx; // more efficient to reuse a protobuf Message
    bool        m_componentCompleted;

    QString m_url;
    bool m_running;
    int m_fps;
    int m_frameCount;
    double m_timestamp;

    QTime m_time;

private slots:
    void connectSocket();
    void disconnectSocket();
    void updateMessageReceived(QList<QByteArray> messageList);

    void updateFramerate();
    void updateStreamBuffer();
    void updateStreamBufferItem();
};

#endif // MJPEGSTREAMER2_H
