/****************************************************************************
**
** This file was generated by a code generator based on imatix/gsl
** Any changes in this file will be lost.
**
****************************************************************************/
#include "previewclientbase.h"
#include <google/protobuf/text_format.h>
#include "debughelper.h"

#if defined(Q_OS_IOS)
namespace gpb = google_public::protobuf;
#else
namespace gpb = google::protobuf;
#endif

using namespace nzmqt;

namespace machinetalk { namespace pathview {

/** Generic Preview Client Base implementation */
PreviewClientBase::PreviewClientBase(QObject *parent)
    : QObject(parent)
    , QQmlParserStatus()
    , m_componentCompleted(false)
    , m_ready(false)
    , m_debugName("Preview Client Base")
    , m_previewChannel(nullptr)
    , m_previewstatusChannel(nullptr)
    , m_state(State::Down)
    , m_previousState(State::Down)
    , m_errorString("")
{
    // initialize preview channel
    m_previewChannel = new pathview::PreviewSubscribe(this);
    m_previewChannel->setDebugName(m_debugName + " - preview");
    connect(m_previewChannel, &pathview::PreviewSubscribe::socketUriChanged,
            this, &PreviewClientBase::previewUriChanged);
    connect(m_previewChannel, &pathview::PreviewSubscribe::stateChanged,
            this, &PreviewClientBase::previewChannelStateChanged);
    connect(m_previewChannel, &pathview::PreviewSubscribe::socketMessageReceived,
            this, &PreviewClientBase::processPreviewChannelMessage);
    // initialize previewstatus channel
    m_previewstatusChannel = new pathview::PreviewSubscribe(this);
    m_previewstatusChannel->setDebugName(m_debugName + " - previewstatus");
    connect(m_previewstatusChannel, &pathview::PreviewSubscribe::socketUriChanged,
            this, &PreviewClientBase::previewstatusUriChanged);
    connect(m_previewstatusChannel, &pathview::PreviewSubscribe::stateChanged,
            this, &PreviewClientBase::previewstatusChannelStateChanged);
    connect(m_previewstatusChannel, &pathview::PreviewSubscribe::socketMessageReceived,
            this, &PreviewClientBase::processPreviewstatusChannelMessage);
    // state machine
    connect(this, &PreviewClientBase::fsmUpEntered,
            this, &PreviewClientBase::fsmUpEntry);
    connect(this, &PreviewClientBase::fsmUpExited,
            this, &PreviewClientBase::fsmUpExit);
    connect(this, &PreviewClientBase::fsmDownConnect,
            this, &PreviewClientBase::fsmDownConnectEvent);
    connect(this, &PreviewClientBase::fsmTryingStatusUp,
            this, &PreviewClientBase::fsmTryingStatusUpEvent);
    connect(this, &PreviewClientBase::fsmTryingPreviewUp,
            this, &PreviewClientBase::fsmTryingPreviewUpEvent);
    connect(this, &PreviewClientBase::fsmTryingDisconnect,
            this, &PreviewClientBase::fsmTryingDisconnectEvent);
    connect(this, &PreviewClientBase::fsmPreviewtryingPreviewUp,
            this, &PreviewClientBase::fsmPreviewtryingPreviewUpEvent);
    connect(this, &PreviewClientBase::fsmPreviewtryingStatusTrying,
            this, &PreviewClientBase::fsmPreviewtryingStatusTryingEvent);
    connect(this, &PreviewClientBase::fsmPreviewtryingDisconnect,
            this, &PreviewClientBase::fsmPreviewtryingDisconnectEvent);
    connect(this, &PreviewClientBase::fsmStatustryingStatusUp,
            this, &PreviewClientBase::fsmStatustryingStatusUpEvent);
    connect(this, &PreviewClientBase::fsmStatustryingPreviewTrying,
            this, &PreviewClientBase::fsmStatustryingPreviewTryingEvent);
    connect(this, &PreviewClientBase::fsmStatustryingDisconnect,
            this, &PreviewClientBase::fsmStatustryingDisconnectEvent);
    connect(this, &PreviewClientBase::fsmUpPreviewTrying,
            this, &PreviewClientBase::fsmUpPreviewTryingEvent);
    connect(this, &PreviewClientBase::fsmUpStatusTrying,
            this, &PreviewClientBase::fsmUpStatusTryingEvent);
    connect(this, &PreviewClientBase::fsmUpDisconnect,
            this, &PreviewClientBase::fsmUpDisconnectEvent);
}

PreviewClientBase::~PreviewClientBase()
{
}

/** Add a topic that should be subscribed **/
void PreviewClientBase::addPreviewTopic(const QString &name)
{
    m_previewChannel->addSocketTopic(name);
}

/** Removes a topic from the list of topics that should be subscribed **/
void PreviewClientBase::removePreviewTopic(const QString &name)
{
    m_previewChannel->removeSocketTopic(name);
}

/** Clears the the topics that should be subscribed **/
void PreviewClientBase::clearPreviewTopics()
{
    m_previewChannel->clearSocketTopics();
}

/** Add a topic that should be subscribed **/
void PreviewClientBase::addPreviewstatusTopic(const QString &name)
{
    m_previewstatusChannel->addSocketTopic(name);
}

/** Removes a topic from the list of topics that should be subscribed **/
void PreviewClientBase::removePreviewstatusTopic(const QString &name)
{
    m_previewstatusChannel->removeSocketTopic(name);
}

/** Clears the the topics that should be subscribed **/
void PreviewClientBase::clearPreviewstatusTopics()
{
    m_previewstatusChannel->clearSocketTopics();
}

void PreviewClientBase::startPreviewChannel()
{
    m_previewChannel->setReady(true);
}

void PreviewClientBase::stopPreviewChannel()
{
    m_previewChannel->setReady(false);
}

void PreviewClientBase::startPreviewstatusChannel()
{
    m_previewstatusChannel->setReady(true);
}

void PreviewClientBase::stopPreviewstatusChannel()
{
    m_previewstatusChannel->setReady(false);
}

/** Processes all message received on preview */
void PreviewClientBase::processPreviewChannelMessage(const QByteArray &topic, const Container &rx)
{

    // react to preview message
    if (rx.type() == MT_PREVIEW)
    {
        handlePreviewMessage(topic, rx);
    }

    emit previewMessageReceived(topic, rx);
}

/** Processes all message received on previewstatus */
void PreviewClientBase::processPreviewstatusChannelMessage(const QByteArray &topic, const Container &rx)
{

    // react to interp stat message
    if (rx.type() == MT_INTERP_STAT)
    {
        handleInterpStatMessage(topic, rx);
    }

    emit previewstatusMessageReceived(topic, rx);
}

void PreviewClientBase::fsmDown()
{
#ifdef QT_DEBUG
    DEBUG_TAG(1, m_debugName, "State DOWN");
#endif
    m_state = State::Down;
    emit stateChanged(m_state);
}

void PreviewClientBase::fsmDownConnectEvent()
{
    if (m_state == State::Down)
    {
#ifdef QT_DEBUG
        DEBUG_TAG(1, m_debugName, "Event CONNECT");
#endif
        // handle state change
        emit fsmDownExited(QPrivateSignal());
        fsmTrying();
        emit fsmTryingEntered(QPrivateSignal());
        // execute actions
        startPreviewChannel();
        startPreviewstatusChannel();
     }
}

void PreviewClientBase::fsmTrying()
{
#ifdef QT_DEBUG
    DEBUG_TAG(1, m_debugName, "State TRYING");
#endif
    m_state = State::Trying;
    emit stateChanged(m_state);
}

void PreviewClientBase::fsmTryingStatusUpEvent()
{
    if (m_state == State::Trying)
    {
#ifdef QT_DEBUG
        DEBUG_TAG(1, m_debugName, "Event STATUS UP");
#endif
        // handle state change
        emit fsmTryingExited(QPrivateSignal());
        fsmPreviewtrying();
        emit fsmPreviewtryingEntered(QPrivateSignal());
        // execute actions
     }
}

void PreviewClientBase::fsmTryingPreviewUpEvent()
{
    if (m_state == State::Trying)
    {
#ifdef QT_DEBUG
        DEBUG_TAG(1, m_debugName, "Event PREVIEW UP");
#endif
        // handle state change
        emit fsmTryingExited(QPrivateSignal());
        fsmStatustrying();
        emit fsmStatustryingEntered(QPrivateSignal());
        // execute actions
     }
}

void PreviewClientBase::fsmTryingDisconnectEvent()
{
    if (m_state == State::Trying)
    {
#ifdef QT_DEBUG
        DEBUG_TAG(1, m_debugName, "Event DISCONNECT");
#endif
        // handle state change
        emit fsmTryingExited(QPrivateSignal());
        fsmDown();
        emit fsmDownEntered(QPrivateSignal());
        // execute actions
        stopPreviewChannel();
        stopPreviewstatusChannel();
     }
}

void PreviewClientBase::fsmPreviewtrying()
{
#ifdef QT_DEBUG
    DEBUG_TAG(1, m_debugName, "State PREVIEWTRYING");
#endif
    m_state = State::Previewtrying;
    emit stateChanged(m_state);
}

void PreviewClientBase::fsmPreviewtryingPreviewUpEvent()
{
    if (m_state == State::Previewtrying)
    {
#ifdef QT_DEBUG
        DEBUG_TAG(1, m_debugName, "Event PREVIEW UP");
#endif
        // handle state change
        emit fsmPreviewtryingExited(QPrivateSignal());
        fsmUp();
        emit fsmUpEntered(QPrivateSignal());
        // execute actions
     }
}

void PreviewClientBase::fsmPreviewtryingStatusTryingEvent()
{
    if (m_state == State::Previewtrying)
    {
#ifdef QT_DEBUG
        DEBUG_TAG(1, m_debugName, "Event STATUS TRYING");
#endif
        // handle state change
        emit fsmPreviewtryingExited(QPrivateSignal());
        fsmTrying();
        emit fsmTryingEntered(QPrivateSignal());
        // execute actions
     }
}

void PreviewClientBase::fsmPreviewtryingDisconnectEvent()
{
    if (m_state == State::Previewtrying)
    {
#ifdef QT_DEBUG
        DEBUG_TAG(1, m_debugName, "Event DISCONNECT");
#endif
        // handle state change
        emit fsmPreviewtryingExited(QPrivateSignal());
        fsmDown();
        emit fsmDownEntered(QPrivateSignal());
        // execute actions
        stopPreviewChannel();
        stopPreviewstatusChannel();
     }
}

void PreviewClientBase::fsmStatustrying()
{
#ifdef QT_DEBUG
    DEBUG_TAG(1, m_debugName, "State STATUSTRYING");
#endif
    m_state = State::Statustrying;
    emit stateChanged(m_state);
}

void PreviewClientBase::fsmStatustryingStatusUpEvent()
{
    if (m_state == State::Statustrying)
    {
#ifdef QT_DEBUG
        DEBUG_TAG(1, m_debugName, "Event STATUS UP");
#endif
        // handle state change
        emit fsmStatustryingExited(QPrivateSignal());
        fsmUp();
        emit fsmUpEntered(QPrivateSignal());
        // execute actions
     }
}

void PreviewClientBase::fsmStatustryingPreviewTryingEvent()
{
    if (m_state == State::Statustrying)
    {
#ifdef QT_DEBUG
        DEBUG_TAG(1, m_debugName, "Event PREVIEW TRYING");
#endif
        // handle state change
        emit fsmStatustryingExited(QPrivateSignal());
        fsmTrying();
        emit fsmTryingEntered(QPrivateSignal());
        // execute actions
     }
}

void PreviewClientBase::fsmStatustryingDisconnectEvent()
{
    if (m_state == State::Statustrying)
    {
#ifdef QT_DEBUG
        DEBUG_TAG(1, m_debugName, "Event DISCONNECT");
#endif
        // handle state change
        emit fsmStatustryingExited(QPrivateSignal());
        fsmDown();
        emit fsmDownEntered(QPrivateSignal());
        // execute actions
        stopPreviewChannel();
        stopPreviewstatusChannel();
     }
}

void PreviewClientBase::fsmUp()
{
#ifdef QT_DEBUG
    DEBUG_TAG(1, m_debugName, "State UP");
#endif
    m_state = State::Up;
    emit stateChanged(m_state);
}
void PreviewClientBase::fsmUpEntry()
{
    setConnected();
}
void PreviewClientBase::fsmUpExit()
{
    clearConnected();
}

void PreviewClientBase::fsmUpPreviewTryingEvent()
{
    if (m_state == State::Up)
    {
#ifdef QT_DEBUG
        DEBUG_TAG(1, m_debugName, "Event PREVIEW TRYING");
#endif
        // handle state change
        emit fsmUpExited(QPrivateSignal());
        fsmPreviewtrying();
        emit fsmPreviewtryingEntered(QPrivateSignal());
        // execute actions
     }
}

void PreviewClientBase::fsmUpStatusTryingEvent()
{
    if (m_state == State::Up)
    {
#ifdef QT_DEBUG
        DEBUG_TAG(1, m_debugName, "Event STATUS TRYING");
#endif
        // handle state change
        emit fsmUpExited(QPrivateSignal());
        fsmStatustrying();
        emit fsmStatustryingEntered(QPrivateSignal());
        // execute actions
     }
}

void PreviewClientBase::fsmUpDisconnectEvent()
{
    if (m_state == State::Up)
    {
#ifdef QT_DEBUG
        DEBUG_TAG(1, m_debugName, "Event DISCONNECT");
#endif
        // handle state change
        emit fsmUpExited(QPrivateSignal());
        fsmDown();
        emit fsmDownEntered(QPrivateSignal());
        // execute actions
        stopPreviewChannel();
        stopPreviewstatusChannel();
     }
}

void PreviewClientBase::previewChannelStateChanged(pathview::PreviewSubscribe::State state)
{

    if (state == pathview::PreviewSubscribe::State::Trying)
    {
        if (m_state == State::Up)
        {
            emit fsmUpPreviewTrying(QPrivateSignal());
        }
        else if (m_state == State::Statustrying)
        {
            emit fsmStatustryingPreviewTrying(QPrivateSignal());
        }
    }

    else if (state == pathview::PreviewSubscribe::State::Up)
    {
        if (m_state == State::Trying)
        {
            emit fsmTryingPreviewUp(QPrivateSignal());
        }
        else if (m_state == State::Previewtrying)
        {
            emit fsmPreviewtryingPreviewUp(QPrivateSignal());
        }
    }
}

void PreviewClientBase::previewstatusChannelStateChanged(pathview::PreviewSubscribe::State state)
{

    if (state == pathview::PreviewSubscribe::State::Trying)
    {
        if (m_state == State::Up)
        {
            emit fsmUpStatusTrying(QPrivateSignal());
        }
        else if (m_state == State::Previewtrying)
        {
            emit fsmPreviewtryingStatusTrying(QPrivateSignal());
        }
    }

    else if (state == pathview::PreviewSubscribe::State::Up)
    {
        if (m_state == State::Trying)
        {
            emit fsmTryingStatusUp(QPrivateSignal());
        }
        else if (m_state == State::Statustrying)
        {
            emit fsmStatustryingStatusUp(QPrivateSignal());
        }
    }
}

/** start trigger function */
void PreviewClientBase::start()
{
    if (m_state == State::Down) {
        emit fsmDownConnect(QPrivateSignal());
    }
}

/** stop trigger function */
void PreviewClientBase::stop()
{
    if (m_state == State::Trying) {
        emit fsmTryingDisconnect(QPrivateSignal());
    }
    else if (m_state == State::Up) {
        emit fsmUpDisconnect(QPrivateSignal());
    }
}

} } // namespace machinetalk::pathview
