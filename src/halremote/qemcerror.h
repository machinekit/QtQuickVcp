#ifndef QEMCERROR_H
#define QEMCERROR_H

#include <QQuickItem>
#include <QTimer>
#include <nzmqt/nzmqt.hpp>
#include <google/protobuf/text_format.h>
#include "message.pb.h"

#if defined(Q_OS_IOS)
namespace gpb = google_public::protobuf;
#else
namespace gpb = google::protobuf;
#endif

using namespace nzmqt;

class QEmcError : public QQuickItem
{
    Q_OBJECT
    Q_INTERFACES(QQmlParserStatus)
    Q_PROPERTY(QString errorUri READ errorUri WRITE setErrorUri NOTIFY errorUriChanged)
    Q_PROPERTY(bool ready READ ready WRITE setReady NOTIFY readyChanged)
    Q_PROPERTY(State connectionState READ connectionState NOTIFY connectionStateChanged)
    Q_PROPERTY(ConnectionError error READ error NOTIFY errorChanged)
    Q_PROPERTY(QString errorString READ errorString NOTIFY errorStringChanged)
    Q_PROPERTY(ErrorChannels channels READ channels WRITE setChannels NOTIFY channelsChanged)
    Q_ENUMS(State ConnectionError ErrorType)
    Q_FLAGS(ErrorChannels)

public:
    explicit QEmcError(QQuickItem *parent = 0);
    ~QEmcError();

    enum SocketState {
        Down = 1,
        Trying = 2,
        Up = 3
    };

    enum State {
        Disconnected = 0,
        Connecting = 1,
        Connected = 2,
        Error = 3
    };

    enum ConnectionError {
        NoError = 0,
        ServiceError = 1,
        TimeoutError = 2,
        SocketError = 3
    };

    enum ErrorType {
        NmlError = pb::MT_EMC_NML_ERROR,
        NmlText = pb::MT_EMC_NML_TEXT,
        NmlDisplay = pb::MT_EMC_NML_DISPLAY,
        OperatorError = pb::MT_EMC_OPERATOR_ERROR,
        OperatorText = pb::MT_EMC_OPERATOR_TEXT,
        OperatorDisplay = pb::MT_EMC_OPERATOR_DISPLAY
    };

    enum ErrorChannelEnum {
        ErrorChannel   = 0x1,
        TextChannel    = 0x2,
        DisplayChannel = 0x4,
    };
    Q_DECLARE_FLAGS(ErrorChannels, ErrorChannelEnum)

    virtual void componentComplete();

    QString errorUri() const
    {
        return m_errorUri;
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

    ErrorChannels channels() const
    {
        return m_channels;
    }

public slots:

    void setErrorUri(QString arg)
    {
        if (m_errorUri == arg)
            return;

        m_errorUri = arg;
        emit errorUriChanged(arg);
    }

    void setReady(bool arg);

    void setChannels(ErrorChannels arg)
    {
        if (m_channels == arg)
            return;

        m_channels = arg;
        emit channelsChanged(arg);
    }

private:
    QString         m_errorUri;
    bool            m_ready;
    SocketState     m_sState;
    State           m_connectionState;
    ConnectionError m_error;
    QString         m_errorString;
    ErrorChannels   m_channels;
    bool            m_componentCompleted;

    PollingZMQContext *m_context;
    ZMQSocket   *m_errorSocket;
    QStringList  m_subscriptions;
    QTimer      *m_errorHeartbeatTimer;
    bool         m_errorPingOutstanding;
    // more efficient to reuse a protobuf Message
    pb::Container   m_rx;

    void start();
    void stop();
    void startErrorHeartbeat(int interval);
    void stopErrorHeartbeat();
    void refreshErrorHeartbeat();
    void updateState(State state);
    void updateError(ConnectionError error, const QString &errorString);

private slots:
    void errorMessageReceived(const QList<QByteArray> &messageList);
    void pollError(int errorNum, const QString &errorMsg);
    void errorHeartbeatTimerTick();

    bool connectSockets();
    void disconnectSockets();
    void subscribe();
    void unsubscribe();

signals:
    void errorUriChanged(QString arg);
    void readyChanged(bool arg);
    void connectionStateChanged(State arg);
    void errorChanged(ConnectionError arg);
    void errorStringChanged(QString arg);
    void channelsChanged(ErrorChannels arg);
    void messageReceived(ErrorType type, const QString &text);
};

#endif // QEMCERROR_H
