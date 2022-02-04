/****************************************************************************
**
** This file was generated by a code generator based on imatix/gsl
** Any changes in this file will be lost.
**
****************************************************************************/
#ifndef COMMON_RPC_SERVICE_H
#define COMMON_RPC_SERVICE_H
#include <QObject>
#include <QSet>
#include <QDebug>
#include <nzmqt/nzmqt.hpp>
#include <machinetalk/protobuf/message.pb.h>

namespace machinetalk { namespace common {

class RpcService
    : public QObject
{
    Q_OBJECT
    Q_PROPERTY(bool ready READ ready WRITE setReady NOTIFY readyChanged)
    Q_PROPERTY(QString socketUri READ socketUri WRITE setSocketUri NOTIFY socketUriChanged)
    Q_PROPERTY(QString debugName READ debugName WRITE setDebugName NOTIFY debugNameChanged)
    Q_PROPERTY(State connectionState READ state NOTIFY stateChanged)
    Q_PROPERTY(QString errorString READ errorString NOTIFY errorStringChanged)

public:
    explicit RpcService(QObject *parent = nullptr);
    ~RpcService();

    enum class State {
        Down = 0,
        Up = 1,
    };
    Q_ENUM(State)

    QString socketUri() const
    {
        return m_socketUri;
    }

    QString debugName() const
    {
        return m_debugName;
    }

    State state() const
    {
        return m_state;
    }

    QString errorString() const
    {
        return m_errorString;
    }

    bool ready() const
    {
        return m_ready;
    }

public slots:

    void setSocketUri(const QString &uri)
    {
        if (m_socketUri == uri) {
            return;
        }

        m_socketUri = uri;
        emit socketUriChanged(uri);
    }

    void setDebugName(const QString &debugName)
    {
        if (m_debugName == debugName) {
            return;
        }

        m_debugName = debugName;
        emit debugNameChanged(debugName);
    }

    void setReady(bool ready)
    {
        if (m_ready == ready) {
            return;
        }

        m_ready = ready;
        emit readyChanged(ready);

        if (m_ready)
        {
            start();
        }
        else
        {
            stop();
        }
    }


    void sendSocketMessage(const QByteArray &topic, ContainerType type, Container &tx);

protected:
    void start(); // start trigger
    void stop(); // stop trigger

private:
    bool m_ready;
    QString m_debugName;

    QString m_socketUri;
    nzmqt::PollingZMQContext *m_context;
    nzmqt::ZMQSocket *m_socket;

    State         m_state;
    State         m_previousState;
    QString       m_errorString;
    // more efficient to reuse a protobuf Messages
    Container m_socketRx;
    Container m_socketTx;

private slots:

    bool startSocket();
    void stopSocket();

    void processSocketMessage(const QList<QByteArray> &messageList);
    void socketError(int errorNum, const QString &errorMsg);

    void sendPingAcknowledge();

    void fsmDown();
    void fsmDownStartEvent();
    void fsmUp();
    void fsmUpPingReceivedEvent();
    void fsmUpStopEvent();


signals:
    void socketUriChanged(const QString &uri);
    void socketMessageReceived(const QByteArray &topic, const Container &rx);
    void debugNameChanged(const QString &debugName);
    void stateChanged(RpcService::State state);
    void errorStringChanged(const QString &errorString);
    void readyChanged(bool ready);
    // fsm
    void fsmDownEntered(QPrivateSignal);
    void fsmDownExited(QPrivateSignal);
    void fsmDownStart(QPrivateSignal);
    void fsmUpEntered(QPrivateSignal);
    void fsmUpExited(QPrivateSignal);
    void fsmUpPingReceived(QPrivateSignal);
    void fsmUpStop(QPrivateSignal);
};

} } // namespace machinetalk::common
#endif // COMMON_RPC_SERVICE_H
