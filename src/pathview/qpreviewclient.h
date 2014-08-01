#ifndef QPREVIEWCLIENT_H
#define QPREVIEWCLIENT_H

#include <QQuickItem>
#include <nzmqt/nzmqt.hpp>
#include <google/protobuf/text_format.h>
#include "message.pb.h"
#include "qgcodeprogrammodel.h"

#if defined(Q_OS_IOS)
namespace gpb = google_public::protobuf;
#else
namespace gpb = google::protobuf;
#endif

using namespace nzmqt;

class QPreviewClient : public QQuickItem
{
    Q_OBJECT
    Q_INTERFACES(QQmlParserStatus)
    Q_PROPERTY(QString statusUri READ statusUri WRITE setStatusUri NOTIFY statusUriChanged)
    Q_PROPERTY(QString previewUri READ previewUri WRITE setPreviewUri NOTIFY previewUriChanged)
    Q_PROPERTY(bool ready READ ready WRITE setReady NOTIFY readyChanged)
    Q_PROPERTY(State connectionState READ connectionState NOTIFY connectionStateChanged)
    Q_PROPERTY(ConnectionError error READ error NOTIFY errorChanged)
    Q_PROPERTY(QString errorString READ errorString NOTIFY errorStringChanged)
    Q_PROPERTY(QGCodeProgramModel *model READ model WRITE setModel NOTIFY modelChanged)
    Q_ENUMS(State ConnectionError)

public:
    explicit QPreviewClient(QQuickItem *parent = 0);
    ~QPreviewClient();

    enum State {
        Disconnected = 0,
        Connecting = 1,
        Connected = 2,
        Error = 3
    };

    enum ConnectionError {
        NoError = 0,
        BindError = 1,
        SocketError = 2
    };

    virtual void componentComplete();

    QString statusUri() const
    {
        return m_statusUri;
    }

    QString previewUri() const
    {
        return m_previewUri;
    }

    bool ready() const
    {
        return m_ready;
    }

    State connectionState() const
    {
        return m_connectionState;
    }

    ConnectionError error() const
    {
        return m_error;
    }

    QString errorString() const
    {
        return m_errorString;
    }

    QGCodeProgramModel * model() const
    {
        return m_model;
    }

public slots:

    void setStatusUri(QString arg)
    {
        if (m_statusUri != arg) {
            m_statusUri = arg;
            emit statusUriChanged(arg);
        }
    }

    void setPreviewUri(QString arg)
    {
        if (m_previewUri != arg) {
            m_previewUri = arg;
            emit previewUriChanged(arg);
        }
    }

    void setReady(bool arg);

    void setModel(QGCodeProgramModel * arg)
    {
        if (m_model != arg) {
            m_model = arg;
            emit modelChanged(arg);
        }
    }

private:
    typedef struct {
        QString fileName;
        int lineNumber;
    } PreviewStatus;

    QString m_statusUri;
    QString m_previewUri;
    bool    m_ready;
    State       m_connectionState;
    ConnectionError       m_error;
    QString m_errorString;
    bool    m_componentCompleted;

    PollingZMQContext *m_context;
    ZMQSocket  *m_statusSocket;
    ZMQSocket  *m_previewSocket;
    // more efficient to reuse a protobuf Message
    pb::Container   m_rx;

    PreviewStatus m_previewStatus;

    void start();
    void stop();
    void updateState(State state);
    void updateError(ConnectionError error, QString errorString);

    QGCodeProgramModel * m_model;

private slots:
    void statusMessageReceived(QList<QByteArray> messageList);
    void previewMessageReceived(QList<QByteArray> messageList);
    void pollError(int errorNum, const QString& errorMsg);

    bool connectSockets();
    void disconnectSockets();

signals:
    void statusUriChanged(QString arg);
    void previewUriChanged(QString arg);
    void readyChanged(bool arg);
    void connectionStateChanged(State arg);
    void errorChanged(ConnectionError arg);
    void errorStringChanged(QString arg);
    void modelChanged(QGCodeProgramModel * arg);
};

#endif // QPREVIEWCLIENT_H
