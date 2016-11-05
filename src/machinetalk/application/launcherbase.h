/****************************************************************************
**
** This code was generated by a code generator based on imatix/gsl
** Any changes in this code will be lost.
**
****************************************************************************/
#ifndef LAUNCHER_BASE_H
#define LAUNCHER_BASE_H
#include <QObject>
#include <QStateMachine>
#include <QQmlParserStatus>
#include <nzmqt/nzmqt.hpp>
#include <machinetalk/protobuf/message.pb.h>
#include <google/protobuf/text_format.h>
#include <machinetalk/rpcclient.h>
#include <application/launchersubscribe.h>

namespace application {

class LauncherBase : public QObject
,public QQmlParserStatus
{
    Q_OBJECT
    Q_INTERFACES(QQmlParserStatus)
    Q_PROPERTY(bool ready READ ready WRITE setReady NOTIFY readyChanged)
    Q_PROPERTY(QString launchercmdUri READ launchercmdUri WRITE setLaunchercmdUri NOTIFY launchercmdUriChanged)
    Q_PROPERTY(QString launcherUri READ launcherUri WRITE setLauncherUri NOTIFY launcherUriChanged)
    Q_PROPERTY(QString debugName READ debugName WRITE setDebugName NOTIFY debugNameChanged)
    Q_PROPERTY(State connectionState READ state NOTIFY stateChanged)
    Q_PROPERTY(QString errorString READ errorString NOTIFY errorStringChanged)
    Q_PROPERTY(int launchercmdHeartbeatInterval READ launchercmdHeartbeatInterval WRITE setLaunchercmdHeartbeatInterval NOTIFY launchercmdHeartbeatIntervalChanged)
    Q_PROPERTY(int launcherHeartbeatInterval READ launcherHeartbeatInterval WRITE setLauncherHeartbeatInterval NOTIFY launcherHeartbeatIntervalChanged)
    Q_ENUMS(State)

public:
    explicit LauncherBase(QObject *parent = 0);
    ~LauncherBase();

    enum State {
        Down = 0,
        Trying = 1,
        Syncing = 2,
        Synced = 3,
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

    QString launchercmdUri() const
    {
        return m_launchercmdChannel->socketUri();
    }

    QString launcherUri() const
    {
        return m_launcherChannel->socketUri();
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

    int launchercmdHeartbeatInterval() const
    {
        return m_launchercmdChannel->heartbeatInterval();
    }

    int launcherHeartbeatInterval() const
    {
        return m_launcherChannel->heartbeatInterval();
    }

    bool ready() const
    {
        return m_ready;
    }

public slots:

    void setLaunchercmdUri(QString uri)
    {
        m_launchercmdChannel->setSocketUri(uri);
    }

    void setLauncherUri(QString uri)
    {
        m_launcherChannel->setSocketUri(uri);
    }

    void setDebugName(QString debugName)
    {
        if (m_debugName == debugName)
            return;

        m_debugName = debugName;
        emit debugNameChanged(debugName);
    }

    void setLaunchercmdHeartbeatInterval(int interval)
    {
        m_launchercmdChannel->setHeartbeatInterval(interval);
    }

    void setLauncherHeartbeatInterval(int interval)
    {
        m_launcherChannel->setHeartbeatInterval(interval);
    }

    void setReady(bool ready)
    {
        if (m_ready == ready)
            return;

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


    void sendLaunchercmdMessage(pb::ContainerType type, pb::Container *tx);
    void sendLauncherStart(pb::Container *tx);
    void sendLauncherKill(pb::Container *tx);
    void sendLauncherTerminate(pb::Container *tx);
    void sendLauncherWriteStdin(pb::Container *tx);
    void sendLauncherCall(pb::Container *tx);
    void sendLauncherShutdown(pb::Container *tx);
    void addLauncherTopic(const QString &name);
    void removeLauncherTopic(const QString &name);
    void clearLauncherTopics();

protected:
    void start(); // start trigger
    void stop(); // stop trigger

private:
    bool m_componentCompleted;
    bool m_ready;
    QString m_debugName;

    machinetalk::RpcClient *m_launchercmdChannel;
    QSet<QString> m_launcherTopics;  // the topics we are interested in
    application::LauncherSubscribe *m_launcherChannel;

    State         m_state;
    State         m_previousState;
    QStateMachine *m_fsm;
    QString       m_errorString;
    // more efficient to reuse a protobuf Messages
    pb::Container m_launchercmdRx;
    pb::Container m_launchercmdTx;
    pb::Container m_launcherRx;

private slots:

    void startLaunchercmdChannel();
    void stopLaunchercmdChannel();
    void launchercmdChannelStateChanged(machinetalk::RpcClient::State state);
    void processLaunchercmdChannelMessage(pb::Container *rx);

    void startLauncherChannel();
    void stopLauncherChannel();
    void launcherChannelStateChanged(application::LauncherSubscribe::State state);
    void processLauncherChannelMessage(const QByteArray &topic, pb::Container *rx);

    void fsmDownEntered();
    void fsmDownConnectEvent();
    void fsmTryingEntered();
    void fsmTryingLaunchercmdUpEvent();
    void fsmTryingDisconnectEvent();
    void fsmSyncingEntered();
    void fsmSyncingLaunchercmdTryingEvent();
    void fsmSyncingLauncherUpEvent();
    void fsmSyncingDisconnectEvent();
    void fsmSyncedEntered();
    void fsmSyncedLauncherTryingEvent();
    void fsmSyncedLaunchercmdTryingEvent();
    void fsmSyncedDisconnectEvent();

    virtual void errorReceived(pb::Container *rx) = 0;
    virtual void launcherFullUpdateReceived(const QByteArray &topic, pb::Container *rx) = 0;
    virtual void launcherIncrementalUpdateReceived(const QByteArray &topic, pb::Container *rx) = 0;
    virtual void syncStatus() = 0;
    virtual void unsyncStatus() = 0;

signals:

    void launchercmdUriChanged(QString uri);
    void launcherUriChanged(QString uri);
    void launchercmdMessageReceived(pb::Container *rx);
    void launcherMessageReceived(const QByteArray &topic, pb::Container *rx);
    void debugNameChanged(QString debugName);
    void stateChanged(LauncherBase::State state);
    void errorStringChanged(QString errorString);
    void launchercmdHeartbeatIntervalChanged(int interval);
    void launcherHeartbeatIntervalChanged(int interval);
    void readyChanged(bool ready);
    // fsm
    void fsmDownConnect();
    void fsmDownConnectQueued();
    void fsmTryingLaunchercmdUp();
    void fsmTryingLaunchercmdUpQueued();
    void fsmTryingDisconnect();
    void fsmTryingDisconnectQueued();
    void fsmSyncingLaunchercmdTrying();
    void fsmSyncingLaunchercmdTryingQueued();
    void fsmSyncingLauncherUp();
    void fsmSyncingLauncherUpQueued();
    void fsmSyncingDisconnect();
    void fsmSyncingDisconnectQueued();
    void fsmSyncedLauncherTrying();
    void fsmSyncedLauncherTryingQueued();
    void fsmSyncedLaunchercmdTrying();
    void fsmSyncedLaunchercmdTryingQueued();
    void fsmSyncedDisconnect();
    void fsmSyncedDisconnectQueued();
};
}; // namespace application
#endif //LAUNCHER_BASE_H
