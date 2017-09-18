/****************************************************************************
**
** This file was generated by a code generator based on imatix/gsl
** Any changes in this file will be lost.
**
****************************************************************************/
#ifndef APPLICATION_CONFIG_BASE_H
#define APPLICATION_CONFIG_BASE_H
#include <QObject>
#include <QSet>
#include <QQmlParserStatus>
#include <nzmqt/nzmqt.hpp>
#include <machinetalk/protobuf/message.pb.h>
#include "../common/rpcclient.h"

namespace machinetalk { namespace application {

class ConfigBase
    : public QObject
    , public QQmlParserStatus
{
    Q_OBJECT
    Q_INTERFACES(QQmlParserStatus)
    Q_PROPERTY(bool ready READ ready WRITE setReady NOTIFY readyChanged)
    Q_PROPERTY(QString configUri READ configUri WRITE setConfigUri NOTIFY configUriChanged)
    Q_PROPERTY(QString debugName READ debugName WRITE setDebugName NOTIFY debugNameChanged)
    Q_PROPERTY(State connectionState READ state NOTIFY stateChanged)
    Q_PROPERTY(QString errorString READ errorString NOTIFY errorStringChanged)
    Q_PROPERTY(int configHeartbeatInterval READ configHeartbeatInterval WRITE setConfigHeartbeatInterval NOTIFY configHeartbeatIntervalChanged)
    Q_ENUMS(State)

public:
    explicit ConfigBase(QObject *parent = nullptr);
    ~ConfigBase();

    enum class State {
        Down = 0,
        Trying = 1,
        Listing = 2,
        Up = 3,
        Loading = 4,
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

    QString configUri() const
    {
        return m_configChannel->socketUri();
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

    int configHeartbeatInterval() const
    {
        return m_configChannel->heartbeatInterval();
    }

    bool ready() const
    {
        return m_ready;
    }

public slots:

    void setConfigUri(const QString &uri)
    {
        m_configChannel->setSocketUri(uri);
    }

    void setDebugName(const QString &debugName)
    {
        if (m_debugName == debugName) {
            return;
        }

        m_debugName = debugName;
        emit debugNameChanged(debugName);
    }

    void setConfigHeartbeatInterval(int interval)
    {
        m_configChannel->setHeartbeatInterval(interval);
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


    void sendConfigMessage(ContainerType type, Container &tx);
    void sendRetrieveApplication(Container &tx);

protected:
    void start(); // start trigger
    void stop(); // stop trigger

private:
    bool m_componentCompleted;
    bool m_ready;
    QString m_debugName;

    common::RpcClient *m_configChannel;

    State         m_state;
    State         m_previousState;
    QString       m_errorString;
    // more efficient to reuse a protobuf Messages
    Container m_configRx;
    Container m_configTx;

private slots:

    void startConfigChannel();
    void stopConfigChannel();
    void configChannelStateChanged(common::RpcClient::State state);
    void processConfigChannelMessage(const Container &rx);
    void sendListApplications();

    void fsmDown();
    void fsmDownConnectEvent();
    void fsmTrying();
    void fsmTryingConfigUpEvent();
    void fsmTryingDisconnectEvent();
    void fsmListing();
    void fsmListingApplicationRetrievedEvent();
    void fsmListingConfigTryingEvent();
    void fsmListingDisconnectEvent();
    void fsmUp();
    void fsmUpEntry();
    void fsmUpExit();
    void fsmUpConfigTryingEvent();
    void fsmUpLoadApplicationEvent();
    void fsmUpDisconnectEvent();
    void fsmLoading();
    void fsmLoadingApplicationLoadedEvent();
    void fsmLoadingConfigTryingEvent();
    void fsmLoadingDisconnectEvent();

    virtual void handleDescribeApplicationMessage(const Container &rx) = 0;
    virtual void handleApplicationDetailMessage(const Container &rx) = 0;
    virtual void syncConfig() = 0;
    virtual void unsyncConfig() = 0;

signals:
    void configUriChanged(const QString &uri);
    void configMessageReceived(const Container &rx);
    void debugNameChanged(const QString &debugName);
    void stateChanged(ConfigBase::State state);
    void errorStringChanged(const QString &errorString);
    void configHeartbeatIntervalChanged(int interval);
    void readyChanged(bool ready);
    // fsm
    void fsmDownEntered(QPrivateSignal);
    void fsmDownExited(QPrivateSignal);
    void fsmDownConnect(QPrivateSignal);
    void fsmTryingEntered(QPrivateSignal);
    void fsmTryingExited(QPrivateSignal);
    void fsmTryingConfigUp(QPrivateSignal);
    void fsmTryingDisconnect(QPrivateSignal);
    void fsmListingEntered(QPrivateSignal);
    void fsmListingExited(QPrivateSignal);
    void fsmListingApplicationRetrieved(QPrivateSignal);
    void fsmListingConfigTrying(QPrivateSignal);
    void fsmListingDisconnect(QPrivateSignal);
    void fsmUpEntered(QPrivateSignal);
    void fsmUpExited(QPrivateSignal);
    void fsmUpConfigTrying(QPrivateSignal);
    void fsmUpLoadApplication(QPrivateSignal);
    void fsmUpDisconnect(QPrivateSignal);
    void fsmLoadingEntered(QPrivateSignal);
    void fsmLoadingExited(QPrivateSignal);
    void fsmLoadingApplicationLoaded(QPrivateSignal);
    void fsmLoadingConfigTrying(QPrivateSignal);
    void fsmLoadingDisconnect(QPrivateSignal);
};

} } // namespace machinetalk::application
#endif // APPLICATION_CONFIG_BASE_H
