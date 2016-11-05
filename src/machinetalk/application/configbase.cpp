/****************************************************************************
**
** This code was generated by a code generator based on imatix/gsl
** Any changes in this code will be lost.
**
****************************************************************************/
#include "configbase.h"
#include "debughelper.h"

#if defined(Q_OS_IOS)
namespace gpb = google_public::protobuf;
#else
namespace gpb = google::protobuf;
#endif

using namespace nzmqt;

namespace application {

/** Generic Config Base implementation */
ConfigBase::ConfigBase(QObject *parent) :
    QObject(parent),
    QQmlParserStatus(),
    m_componentCompleted(false),
    m_ready(false),
    m_debugName("Config Base"),
    m_configChannel(nullptr),
    m_state(Down),
    m_previousState(Down),
    m_fsm(nullptr),
    m_errorString("")
{
    // initialize config channel
    m_configChannel = new machinetalk::RpcClient(this);
    m_configChannel->setDebugName(m_debugName + " - config");
    connect(m_configChannel, &machinetalk::RpcClient::socketUriChanged,
            this, &ConfigBase::configUriChanged);
    connect(m_configChannel, &machinetalk::RpcClient::stateChanged,
            this, &ConfigBase::configChannelStateChanged);
    connect(m_configChannel, &machinetalk::RpcClient::socketMessageReceived,
            this, &ConfigBase::processConfigChannelMessage, Qt::QueuedConnection);

    connect(m_configChannel, &machinetalk::RpcClient::heartbeatIntervalChanged,
            this, &ConfigBase::configHeartbeatIntervalChanged);

    m_fsm = new QStateMachine(this);
    QState *downState = new QState(m_fsm);
    connect(downState, &QState::entered, this, &ConfigBase::fsmDownEntered, Qt::QueuedConnection);
    QState *tryingState = new QState(m_fsm);
    connect(tryingState, &QState::entered, this, &ConfigBase::fsmTryingEntered, Qt::QueuedConnection);
    QState *listingState = new QState(m_fsm);
    connect(listingState, &QState::entered, this, &ConfigBase::fsmListingEntered, Qt::QueuedConnection);
    QState *upState = new QState(m_fsm);
    connect(upState, &QState::entered, this, &ConfigBase::fsmUpEntered, Qt::QueuedConnection);
    connect(upState, &QState::entered, this, &ConfigBase::syncConfig, Qt::QueuedConnection);
    connect(upState, &QState::exited, this, &ConfigBase::unsyncConfig, Qt::QueuedConnection);
    QState *loadingState = new QState(m_fsm);
    connect(loadingState, &QState::entered, this, &ConfigBase::fsmLoadingEntered, Qt::QueuedConnection);
    m_fsm->setInitialState(downState);
    m_fsm->start();

    connect(this, &ConfigBase::fsmDownConnect,
            this, &ConfigBase::fsmDownConnectQueued, Qt::QueuedConnection);
    downState->addTransition(this, &ConfigBase::fsmDownConnectQueued, tryingState);
    connect(this, &ConfigBase::fsmTryingConfigUp,
            this, &ConfigBase::fsmTryingConfigUpQueued, Qt::QueuedConnection);
    tryingState->addTransition(this, &ConfigBase::fsmTryingConfigUpQueued, listingState);
    connect(this, &ConfigBase::fsmTryingDisconnect,
            this, &ConfigBase::fsmTryingDisconnectQueued, Qt::QueuedConnection);
    tryingState->addTransition(this, &ConfigBase::fsmTryingDisconnectQueued, downState);
    connect(this, &ConfigBase::fsmListingApplicationRetrieved,
            this, &ConfigBase::fsmListingApplicationRetrievedQueued, Qt::QueuedConnection);
    listingState->addTransition(this, &ConfigBase::fsmListingApplicationRetrievedQueued, upState);
    connect(this, &ConfigBase::fsmListingConfigTrying,
            this, &ConfigBase::fsmListingConfigTryingQueued, Qt::QueuedConnection);
    listingState->addTransition(this, &ConfigBase::fsmListingConfigTryingQueued, tryingState);
    connect(this, &ConfigBase::fsmListingDisconnect,
            this, &ConfigBase::fsmListingDisconnectQueued, Qt::QueuedConnection);
    listingState->addTransition(this, &ConfigBase::fsmListingDisconnectQueued, downState);
    connect(this, &ConfigBase::fsmUpConfigTrying,
            this, &ConfigBase::fsmUpConfigTryingQueued, Qt::QueuedConnection);
    upState->addTransition(this, &ConfigBase::fsmUpConfigTryingQueued, tryingState);
    connect(this, &ConfigBase::fsmUpLoadApplication,
            this, &ConfigBase::fsmUpLoadApplicationQueued, Qt::QueuedConnection);
    upState->addTransition(this, &ConfigBase::fsmUpLoadApplicationQueued, loadingState);
    connect(this, &ConfigBase::fsmUpDisconnect,
            this, &ConfigBase::fsmUpDisconnectQueued, Qt::QueuedConnection);
    upState->addTransition(this, &ConfigBase::fsmUpDisconnectQueued, downState);
    connect(this, &ConfigBase::fsmLoadingApplicationLoaded,
            this, &ConfigBase::fsmLoadingApplicationLoadedQueued, Qt::QueuedConnection);
    loadingState->addTransition(this, &ConfigBase::fsmLoadingApplicationLoadedQueued, upState);
    connect(this, &ConfigBase::fsmLoadingConfigTrying,
            this, &ConfigBase::fsmLoadingConfigTryingQueued, Qt::QueuedConnection);
    loadingState->addTransition(this, &ConfigBase::fsmLoadingConfigTryingQueued, tryingState);
    connect(this, &ConfigBase::fsmLoadingDisconnect,
            this, &ConfigBase::fsmLoadingDisconnectQueued, Qt::QueuedConnection);
    loadingState->addTransition(this, &ConfigBase::fsmLoadingDisconnectQueued, downState);

    connect(this, &ConfigBase::fsmDownConnect,
            this, &ConfigBase::fsmDownConnectEvent, Qt::QueuedConnection);
    connect(this, &ConfigBase::fsmTryingConfigUp,
            this, &ConfigBase::fsmTryingConfigUpEvent, Qt::QueuedConnection);
    connect(this, &ConfigBase::fsmTryingDisconnect,
            this, &ConfigBase::fsmTryingDisconnectEvent, Qt::QueuedConnection);
    connect(this, &ConfigBase::fsmListingApplicationRetrieved,
            this, &ConfigBase::fsmListingApplicationRetrievedEvent, Qt::QueuedConnection);
    connect(this, &ConfigBase::fsmListingConfigTrying,
            this, &ConfigBase::fsmListingConfigTryingEvent, Qt::QueuedConnection);
    connect(this, &ConfigBase::fsmListingDisconnect,
            this, &ConfigBase::fsmListingDisconnectEvent, Qt::QueuedConnection);
    connect(this, &ConfigBase::fsmUpConfigTrying,
            this, &ConfigBase::fsmUpConfigTryingEvent, Qt::QueuedConnection);
    connect(this, &ConfigBase::fsmUpLoadApplication,
            this, &ConfigBase::fsmUpLoadApplicationEvent, Qt::QueuedConnection);
    connect(this, &ConfigBase::fsmUpDisconnect,
            this, &ConfigBase::fsmUpDisconnectEvent, Qt::QueuedConnection);
    connect(this, &ConfigBase::fsmLoadingApplicationLoaded,
            this, &ConfigBase::fsmLoadingApplicationLoadedEvent, Qt::QueuedConnection);
    connect(this, &ConfigBase::fsmLoadingConfigTrying,
            this, &ConfigBase::fsmLoadingConfigTryingEvent, Qt::QueuedConnection);
    connect(this, &ConfigBase::fsmLoadingDisconnect,
            this, &ConfigBase::fsmLoadingDisconnectEvent, Qt::QueuedConnection);
}

ConfigBase::~ConfigBase()
{
}

void ConfigBase::startConfigChannel()
{
    m_configChannel->setReady(true);
}

void ConfigBase::stopConfigChannel()
{
    m_configChannel->setReady(false);
}

/** Processes all message received on config */
void ConfigBase::processConfigChannelMessage(pb::Container *rx)
{

    // react to describe application message
    if (rx->type() == pb::MT_DESCRIBE_APPLICATION)
    {

        if (m_state == Listing)
        {
            emit fsmListingApplicationRetrieved();
        }
        describeApplicationReceived(rx);
    }

    // react to application detail message
    if (rx->type() == pb::MT_APPLICATION_DETAIL)
    {

        if (m_state == Loading)
        {
            emit fsmLoadingApplicationLoaded();
        }
        applicationDetailReceived(rx);
    }

    // react to error message
    if (rx->type() == pb::MT_ERROR)
    {

        // update error string with note
        m_errorString = "";
        for (int i = 0; i < rx->note_size(); ++i)
        {
            m_errorString.append(QString::fromStdString(rx->note(i)) + "\n");
        }
        emit errorStringChanged(m_errorString);
        errorReceived(rx);
    }

    emit configMessageReceived(rx);
}

void ConfigBase::sendConfigMessage(pb::ContainerType type, pb::Container *tx)
{
    m_configChannel->sendSocketMessage(type, tx);
    if (type == pb::MT_RETRIEVE_APPLICATION)
    {

        if (m_state == Up)
        {
            emit fsmUpLoadApplication();
        }
    }
}

void ConfigBase::sendListApplications()
{
    pb::Container *tx = &m_configTx;
    sendConfigMessage(pb::MT_LIST_APPLICATIONS, tx);
}

void ConfigBase::sendRetrieveApplication(pb::Container *tx)
{
    sendConfigMessage(pb::MT_RETRIEVE_APPLICATION, tx);
}

void ConfigBase::fsmDownEntered()
{
    if (m_previousState != Down)
    {
#ifdef QT_DEBUG
    DEBUG_TAG(1, m_debugName, "State DOWN");
#endif
        m_previousState = Down;
        emit stateChanged(m_state);
    }
}

void ConfigBase::fsmDownConnectEvent()
{
#ifdef QT_DEBUG
    DEBUG_TAG(1, m_debugName, "Event CONNECT");
#endif

    m_state = Trying;
    startConfigChannel();
}

void ConfigBase::fsmTryingEntered()
{
    if (m_previousState != Trying)
    {
#ifdef QT_DEBUG
    DEBUG_TAG(1, m_debugName, "State TRYING");
#endif
        m_previousState = Trying;
        emit stateChanged(m_state);
    }
}

void ConfigBase::fsmTryingConfigUpEvent()
{
#ifdef QT_DEBUG
    DEBUG_TAG(1, m_debugName, "Event CONFIG UP");
#endif

    m_state = Listing;
    sendListApplications();
}

void ConfigBase::fsmTryingDisconnectEvent()
{
#ifdef QT_DEBUG
    DEBUG_TAG(1, m_debugName, "Event DISCONNECT");
#endif

    m_state = Down;
    stopConfigChannel();
}

void ConfigBase::fsmListingEntered()
{
    if (m_previousState != Listing)
    {
#ifdef QT_DEBUG
    DEBUG_TAG(1, m_debugName, "State LISTING");
#endif
        m_previousState = Listing;
        emit stateChanged(m_state);
    }
}

void ConfigBase::fsmListingApplicationRetrievedEvent()
{
#ifdef QT_DEBUG
    DEBUG_TAG(1, m_debugName, "Event APPLICATION RETRIEVED");
#endif

    m_state = Up;
}

void ConfigBase::fsmListingConfigTryingEvent()
{
#ifdef QT_DEBUG
    DEBUG_TAG(1, m_debugName, "Event CONFIG TRYING");
#endif

    m_state = Trying;
}

void ConfigBase::fsmListingDisconnectEvent()
{
#ifdef QT_DEBUG
    DEBUG_TAG(1, m_debugName, "Event DISCONNECT");
#endif

    m_state = Down;
    stopConfigChannel();
}

void ConfigBase::fsmUpEntered()
{
    if (m_previousState != Up)
    {
#ifdef QT_DEBUG
    DEBUG_TAG(1, m_debugName, "State UP");
#endif
        m_previousState = Up;
        emit stateChanged(m_state);
    }
}

void ConfigBase::fsmUpConfigTryingEvent()
{
#ifdef QT_DEBUG
    DEBUG_TAG(1, m_debugName, "Event CONFIG TRYING");
#endif

    m_state = Trying;
}

void ConfigBase::fsmUpLoadApplicationEvent()
{
#ifdef QT_DEBUG
    DEBUG_TAG(1, m_debugName, "Event LOAD APPLICATION");
#endif

    m_state = Loading;
}

void ConfigBase::fsmUpDisconnectEvent()
{
#ifdef QT_DEBUG
    DEBUG_TAG(1, m_debugName, "Event DISCONNECT");
#endif

    m_state = Down;
    stopConfigChannel();
}

void ConfigBase::fsmLoadingEntered()
{
    if (m_previousState != Loading)
    {
#ifdef QT_DEBUG
    DEBUG_TAG(1, m_debugName, "State LOADING");
#endif
        m_previousState = Loading;
        emit stateChanged(m_state);
    }
}

void ConfigBase::fsmLoadingApplicationLoadedEvent()
{
#ifdef QT_DEBUG
    DEBUG_TAG(1, m_debugName, "Event APPLICATION LOADED");
#endif

    m_state = Up;
}

void ConfigBase::fsmLoadingConfigTryingEvent()
{
#ifdef QT_DEBUG
    DEBUG_TAG(1, m_debugName, "Event CONFIG TRYING");
#endif

    m_state = Trying;
}

void ConfigBase::fsmLoadingDisconnectEvent()
{
#ifdef QT_DEBUG
    DEBUG_TAG(1, m_debugName, "Event DISCONNECT");
#endif

    m_state = Down;
    stopConfigChannel();
}

void ConfigBase::configChannelStateChanged(machinetalk::RpcClient::State state)
{

    if (state == machinetalk::RpcClient::Trying)
    {
        if (m_state == Listing)
        {
            emit fsmListingConfigTrying();
        }
        if (m_state == Up)
        {
            emit fsmUpConfigTrying();
        }
        if (m_state == Loading)
        {
            emit fsmLoadingConfigTrying();
        }
    }

    if (state == machinetalk::RpcClient::Up)
    {
        if (m_state == Trying)
        {
            emit fsmTryingConfigUp();
        }
    }
}

/** start trigger */
void ConfigBase::start()
{
    if (m_state == Down) {
        emit fsmDownConnect();
    }
}

/** stop trigger */
void ConfigBase::stop()
{
    if (m_state == Trying) {
        emit fsmTryingDisconnect();
    }
    if (m_state == Listing) {
        emit fsmListingDisconnect();
    }
    if (m_state == Up) {
        emit fsmUpDisconnect();
    }
    if (m_state == Loading) {
        emit fsmLoadingDisconnect();
    }
}
}; // namespace application
