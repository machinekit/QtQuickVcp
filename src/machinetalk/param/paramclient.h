/****************************************************************************
**
** This file was generated by a code generator based on imatix/gsl
** Any changes in this file will be lost.
**
****************************************************************************/
#ifndef PARAM_PARAM_CLIENT_H
#define PARAM_PARAM_CLIENT_H
#include <QObject>
#include <QSet>
#include <QQmlParserStatus>
#include <nzmqt/nzmqt.hpp>
#include <machinetalk/protobuf/message.pb.h>
#include "../common/rpcclient.h"
#include "../common/subscribe.h"

namespace machinetalk { namespace param {

class ParamClient
    : public QObject
    , public QQmlParserStatus
{
    Q_OBJECT
    Q_INTERFACES(QQmlParserStatus)
    Q_PROPERTY(bool ready READ ready WRITE setReady NOTIFY readyChanged)
    Q_PROPERTY(QString paramcmdUri READ paramcmdUri WRITE setParamcmdUri NOTIFY paramcmdUriChanged)
    Q_PROPERTY(QString paramUri READ paramUri WRITE setParamUri NOTIFY paramUriChanged)
    Q_PROPERTY(QString debugName READ debugName WRITE setDebugName NOTIFY debugNameChanged)
    Q_PROPERTY(State connectionState READ state NOTIFY stateChanged)
    Q_PROPERTY(QString errorString READ errorString NOTIFY errorStringChanged)
    Q_PROPERTY(int paramcmdHeartbeatInterval READ paramcmdHeartbeatInterval WRITE setParamcmdHeartbeatInterval NOTIFY paramcmdHeartbeatIntervalChanged)
    Q_PROPERTY(int paramHeartbeatInterval READ paramHeartbeatInterval WRITE setParamHeartbeatInterval NOTIFY paramHeartbeatIntervalChanged)
    Q_ENUMS(State)

public:
    explicit ParamClient(QObject *parent = nullptr);
    ~ParamClient();

    enum class State {
        Down = 0,
        Connecting = 1,
        Syncing = 2,
        Trying = 3,
        Up = 4,
    };

    void classBegin() {}
    /** componentComplete is executed when the QML component is fully loaded */
    void componentComplete()
    {
        m_componentCompleted = true;

        if (m_ready == true)    // the component was set to ready before it was completed
        {
            start();
        }
    }

    QString paramcmdUri() const
    {
        return m_paramcmdChannel->socketUri();
    }

    QString paramUri() const
    {
        return m_paramChannel->socketUri();
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

    int paramcmdHeartbeatInterval() const
    {
        return m_paramcmdChannel->heartbeatInterval();
    }

    int paramHeartbeatInterval() const
    {
        return m_paramChannel->heartbeatInterval();
    }

    bool ready() const
    {
        return m_ready;
    }

public slots:

    void setParamcmdUri(const QString &uri)
    {
        m_paramcmdChannel->setSocketUri(uri);
    }

    void setParamUri(const QString &uri)
    {
        m_paramChannel->setSocketUri(uri);
    }

    void setDebugName(const QString &debugName)
    {
        if (m_debugName == debugName) {
            return;
        }

        m_debugName = debugName;
        emit debugNameChanged(debugName);
    }

    void setParamcmdHeartbeatInterval(int interval)
    {
        m_paramcmdChannel->setHeartbeatInterval(interval);
    }

    void setParamHeartbeatInterval(int interval)
    {
        m_paramChannel->setHeartbeatInterval(interval);
    }

    void setReady(bool ready)
    {
        if (m_ready == ready) {
            return;
        }

        m_ready = ready;
        emit readyChanged(ready);

        if (m_componentCompleted == false)
        {
            return;
        }

        if (m_ready)
        {
            start();
        }
        else
        {
            stop();
        }
    }


    void sendParamcmdMessage(ContainerType type, Container &tx);
    void sendIncrementalUpdate(Container &tx);
    void addParamTopic(const QString &name);
    void removeParamTopic(const QString &name);
    void clearParamTopics();

protected:
    void start(); // start trigger
    void stop(); // stop trigger

private:
    bool m_componentCompleted;
    bool m_ready;
    QString m_debugName;

    common::RpcClient *m_paramcmdChannel;
    QSet<QString> m_paramTopics;    // the topics we are interested in
    common::Subscribe *m_paramChannel;

    State         m_state;
    State         m_previousState;
    QString       m_errorString;
    // more efficient to reuse a protobuf Messages
    Container m_paramcmdTx;
    Container m_paramRx;

private slots:

    void startParamcmdChannel();
    void stopParamcmdChannel();
    void paramcmdChannelStateChanged(common::RpcClient::State state);

    void startParamChannel();
    void stopParamChannel();
    void paramChannelStateChanged(common::Subscribe::State state);
    void processParamChannelMessage(const QByteArray &topic, const Container &rx);

    void fsmDown();
    void fsmDownConnectEvent();
    void fsmConnecting();
    void fsmConnectingParamcmdUpEvent();
    void fsmConnectingParamUpEvent();
    void fsmConnectingDisconnectEvent();
    void fsmSyncing();
    void fsmSyncingParamUpEvent();
    void fsmSyncingParamcmdTryingEvent();
    void fsmSyncingDisconnectEvent();
    void fsmTrying();
    void fsmTryingParamcmdUpEvent();
    void fsmTryingParamTryingEvent();
    void fsmTryingDisconnectEvent();
    void fsmUp();
    void fsmUpEntry();
    void fsmUpExit();
    void fsmUpParamcmdTryingEvent();
    void fsmUpParamTryingEvent();
    void fsmUpDisconnectEvent();

    virtual void handleFullUpdateMessage(const QByteArray &topic, const Container &rx) = 0;
    virtual void handleIncrementalUpdateMessage(const QByteArray &topic, const Container &rx) = 0;
    virtual void removeKeys() = 0;
    virtual void unsyncKeys() = 0;
    virtual void setSynced() = 0;
    virtual void clearSynced() = 0;

signals:
    void paramcmdUriChanged(const QString &uri);
    void paramUriChanged(const QString &uri);
    void paramMessageReceived(const QByteArray &topic, const Container &rx);
    void debugNameChanged(const QString &debugName);
    void stateChanged(ParamClient::State state);
    void errorStringChanged(const QString &errorString);
    void paramcmdHeartbeatIntervalChanged(int interval);
    void paramHeartbeatIntervalChanged(int interval);
    void readyChanged(bool ready);
    // fsm
    void fsmDownEntered(QPrivateSignal);
    void fsmDownExited(QPrivateSignal);
    void fsmDownConnect(QPrivateSignal);
    void fsmConnectingEntered(QPrivateSignal);
    void fsmConnectingExited(QPrivateSignal);
    void fsmConnectingParamcmdUp(QPrivateSignal);
    void fsmConnectingParamUp(QPrivateSignal);
    void fsmConnectingDisconnect(QPrivateSignal);
    void fsmSyncingEntered(QPrivateSignal);
    void fsmSyncingExited(QPrivateSignal);
    void fsmSyncingParamUp(QPrivateSignal);
    void fsmSyncingParamcmdTrying(QPrivateSignal);
    void fsmSyncingDisconnect(QPrivateSignal);
    void fsmTryingEntered(QPrivateSignal);
    void fsmTryingExited(QPrivateSignal);
    void fsmTryingParamcmdUp(QPrivateSignal);
    void fsmTryingParamTrying(QPrivateSignal);
    void fsmTryingDisconnect(QPrivateSignal);
    void fsmUpEntered(QPrivateSignal);
    void fsmUpExited(QPrivateSignal);
    void fsmUpParamcmdTrying(QPrivateSignal);
    void fsmUpParamTrying(QPrivateSignal);
    void fsmUpDisconnect(QPrivateSignal);
};

} } // namespace machinetalk::param
#endif // PARAM_PARAM_CLIENT_H
