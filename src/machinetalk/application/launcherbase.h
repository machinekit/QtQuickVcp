/****************************************************************************
**
** This file was generated by a code generator based on imatix/gsl
** Any changes in this file will be lost.
**
****************************************************************************/
#ifndef APPLICATION_LAUNCHER_BASE_H
#define APPLICATION_LAUNCHER_BASE_H
#include <QObject>
#include <QSet>
#include <QQmlParserStatus>
#include <nzmqt/nzmqt.hpp>
#include <machinetalk/protobuf/message.pb.h>
#include "../common/rpcclient.h"
#include "../application/launchersubscribe.h"

namespace machinetalk { namespace application {

class LauncherBase
    : public QObject
    , public QQmlParserStatus
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

    void setLaunchercmdUri(const QString &uri)
    {
        m_launchercmdChannel->setSocketUri(uri);
    }

    void setLauncherUri(const QString &uri)
    {
        m_launcherChannel->setSocketUri(uri);
    }

    void setDebugName(const QString &debugName)
    {
        if (m_debugName == debugName) {
            return;
        }

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


    void sendLaunchercmdMessage(ContainerType type, Container &tx);
    void sendLauncherStart(Container &tx);
    void sendLauncherKill(Container &tx);
    void sendLauncherTerminate(Container &tx);
    void sendLauncherWriteStdin(Container &tx);
    void sendLauncherCall(Container &tx);
    void sendLauncherShutdown(Container &tx);
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

    common::RpcClient *m_launchercmdChannel;
    QSet<QString> m_launcherTopics;  // the topics we are interested in
    application::LauncherSubscribe *m_launcherChannel;

    State         m_state;
    State         m_previousState;
    QString       m_errorString;
    // more efficient to reuse a protobuf Messages
    Container m_launchercmdRx;
    Container m_launchercmdTx;
    Container m_launcherRx;

private slots:

    void startLaunchercmdChannel();
    void stopLaunchercmdChannel();
    void launchercmdChannelStateChanged(common::RpcClient::State state);
    void processLaunchercmdChannelMessage(const Container &rx);

    void startLauncherChannel();
    void stopLauncherChannel();
    void launcherChannelStateChanged(application::LauncherSubscribe::State state);
    void processLauncherChannelMessage(const QByteArray &topic, const Container &rx);

    void fsmDown();
    void fsmDownConnectEvent();
    void fsmTrying();
    void fsmTryingLaunchercmdUpEvent();
    void fsmTryingDisconnectEvent();
    void fsmSyncing();
    void fsmSyncingLaunchercmdTryingEvent();
    void fsmSyncingLauncherUpEvent();
    void fsmSyncingDisconnectEvent();
    void fsmSynced();
    void fsmSyncedEntry();
    void fsmSyncedExit();
    void fsmSyncedLauncherTryingEvent();
    void fsmSyncedLaunchercmdTryingEvent();
    void fsmSyncedDisconnectEvent();

    virtual void launcherFullUpdateReceived(const QByteArray &topic, const Container &rx) = 0;
    virtual void launcherIncrementalUpdateReceived(const QByteArray &topic, const Container &rx) = 0;
    virtual void syncStatus() = 0;
    virtual void unsyncStatus() = 0;

signals:
    void launchercmdUriChanged(const QString &uri);
    void launcherUriChanged(const QString &uri);
    void launchercmdMessageReceived(const Container &rx);
    void launcherMessageReceived(const QByteArray &topic, const Container &rx);
    void debugNameChanged(const QString &debugName);
    void stateChanged(LauncherBase::State state);
    void errorStringChanged(const QString &errorString);
    void launchercmdHeartbeatIntervalChanged(int interval);
    void launcherHeartbeatIntervalChanged(int interval);
    void readyChanged(bool ready);
    // fsm
    void fsmDownEntered(QPrivateSignal);
    void fsmDownExited(QPrivateSignal);
    void fsmDownConnect(QPrivateSignal);
    void fsmTryingEntered(QPrivateSignal);
    void fsmTryingExited(QPrivateSignal);
    void fsmTryingLaunchercmdUp(QPrivateSignal);
    void fsmTryingDisconnect(QPrivateSignal);
    void fsmSyncingEntered(QPrivateSignal);
    void fsmSyncingExited(QPrivateSignal);
    void fsmSyncingLaunchercmdTrying(QPrivateSignal);
    void fsmSyncingLauncherUp(QPrivateSignal);
    void fsmSyncingDisconnect(QPrivateSignal);
    void fsmSyncedEntered(QPrivateSignal);
    void fsmSyncedExited(QPrivateSignal);
    void fsmSyncedLauncherTrying(QPrivateSignal);
    void fsmSyncedLaunchercmdTrying(QPrivateSignal);
    void fsmSyncedDisconnect(QPrivateSignal);
};

} } // namespace machinetalk::application
#endif // APPLICATION_LAUNCHER_BASE_H
