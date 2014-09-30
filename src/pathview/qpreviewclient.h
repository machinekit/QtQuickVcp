#ifndef QPREVIEWCLIENT_H
#define QPREVIEWCLIENT_H

#include <abstractserviceimplementation.h>
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

class QPreviewClient : public AbstractServiceImplementation
{
    Q_OBJECT
    Q_INTERFACES(QQmlParserStatus)
    Q_PROPERTY(QString statusUri READ statusUri WRITE setStatusUri NOTIFY statusUriChanged)
    Q_PROPERTY(QString previewUri READ previewUri WRITE setPreviewUri NOTIFY previewUriChanged)
    Q_PROPERTY(State connectionState READ connectionState NOTIFY connectionStateChanged)
    Q_PROPERTY(bool connected READ isConnected NOTIFY connectedChanged)
    Q_PROPERTY(ConnectionError error READ error NOTIFY errorChanged)
    Q_PROPERTY(QString errorString READ errorString NOTIFY errorStringChanged)
    Q_PROPERTY(QGCodeProgramModel *model READ model WRITE setModel NOTIFY modelChanged)
    Q_PROPERTY(InterpreterState interpreterState READ interpreterState NOTIFY interpreterStateChanged)
    Q_PROPERTY(QString interpreterNote READ interpreterNote NOTIFY interpreterNoteChanged)
    Q_ENUMS(State ConnectionError InterpreterState)

public:
    explicit QPreviewClient(QObject *parent = 0);
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

    enum InterpreterState {
        InterpreterIdle = pb::INTERP_IDLE,
        InterpreterRunning = pb::INTERP_RUNNING,
        InterpreterPaused = pb::INTERP_PAUSED,
        InterpreterQueueWait = pb::INTERP_QEUEUE_WAIT,
        InterpreterSyncWait = pb::INTERP_SYNC_WAIT,
        InterpreterAbortWait = pb::INTERP_ABORT_WAIT,
        InterpreterStateUnset = pb::INTERP_STATE_UNSET
    };

    QString statusUri() const
    {
        return m_statusUri;
    }

    QString previewUri() const
    {
        return m_previewUri;
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

    InterpreterState interpreterState() const
    {
        return m_interpreterState;
    }

    QString interpreterNote() const
    {
        return m_interpreterNote;
    }

    bool isConnected() const
    {
        return m_connected;
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
    State   m_connectionState;
    bool    m_connected;
    ConnectionError     m_error;
    QString             m_errorString;
    QGCodeProgramModel *m_model;
    InterpreterState    m_interpreterState;
    QString m_interpreterNote;

    PollingZMQContext *m_context;
    ZMQSocket  *m_statusSocket;
    ZMQSocket  *m_previewSocket;
    // more efficient to reuse a protobuf Message
    pb::Container   m_rx;

    PreviewStatus m_previewStatus;
    bool m_previewUpdated;

    void start();
    void stop();
    void updateState(State state);
    void updateError(ConnectionError error, QString errorString);

private slots:
    void statusMessageReceived(QList<QByteArray> messageList);
    void previewMessageReceived(QList<QByteArray> messageList);
    void pollError(int errorNum, const QString& errorMsg);

    bool connectSockets();
    void disconnectSockets();

signals:
    void statusUriChanged(QString arg);
    void previewUriChanged(QString arg);
    void connectionStateChanged(State arg);
    void errorChanged(ConnectionError arg);
    void errorStringChanged(QString arg);
    void modelChanged(QGCodeProgramModel * arg);
    void interpreterStateChanged(InterpreterState arg);
    void interpreterNoteChanged(QString arg);
    void connectedChanged(bool arg);
};

#endif // QPREVIEWCLIENT_H
