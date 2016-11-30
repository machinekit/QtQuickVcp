/****************************************************************************
**
** This file was generated by a code generator based on imatix/gsl
** Any changes in this file will be lost.
**
****************************************************************************/
#include "halrcompsubscribe.h"
#include "debughelper.h"

#if defined(Q_OS_IOS)
namespace gpb = google_public::protobuf;
#else
namespace gpb = google::protobuf;
#endif

using namespace nzmqt;

namespace machinetalk {
namespace halremote {

/** Generic Halrcomp Subscribe implementation */
HalrcompSubscribe::HalrcompSubscribe(QObject *parent) :
    QObject(parent),
    m_ready(false),
    m_debugName("Halrcomp Subscribe"),
    m_socketUri(""),
    m_context(nullptr),
    m_socket(nullptr),
    m_state(Down),
    m_previousState(Down),
    m_errorString("")
    ,m_heartbeatTimer(new QTimer(this)),
    m_heartbeatInterval(0),
    m_heartbeatLiveness(0),
    m_heartbeatResetLiveness(2)
{

    m_heartbeatTimer->setSingleShot(true);
    connect(m_heartbeatTimer, &QTimer::timeout, this, &HalrcompSubscribe::heartbeatTimerTick);
    // state machine
    connect(this, &HalrcompSubscribe::fsmDownConnect,
            this, &HalrcompSubscribe::fsmDownConnectEvent);
    connect(this, &HalrcompSubscribe::fsmTryingConnected,
            this, &HalrcompSubscribe::fsmTryingConnectedEvent);
    connect(this, &HalrcompSubscribe::fsmTryingDisconnect,
            this, &HalrcompSubscribe::fsmTryingDisconnectEvent);
    connect(this, &HalrcompSubscribe::fsmUpTimeout,
            this, &HalrcompSubscribe::fsmUpTimeoutEvent);
    connect(this, &HalrcompSubscribe::fsmUpTick,
            this, &HalrcompSubscribe::fsmUpTickEvent);
    connect(this, &HalrcompSubscribe::fsmUpMessageReceived,
            this, &HalrcompSubscribe::fsmUpMessageReceivedEvent);
    connect(this, &HalrcompSubscribe::fsmUpDisconnect,
            this, &HalrcompSubscribe::fsmUpDisconnectEvent);

    m_context = new PollingZMQContext(this, 1);
    connect(m_context, &PollingZMQContext::pollError,
            this, &HalrcompSubscribe::socketError);
    m_context->start();
}

HalrcompSubscribe::~HalrcompSubscribe()
{
    stopSocket();

    if (m_context != nullptr)
    {
        m_context->stop();
        m_context->deleteLater();
        m_context = nullptr;
    }
}

/** Add a topic that should be subscribed **/
void HalrcompSubscribe::addSocketTopic(const QString &name)
{
    m_socketTopics.insert(name);
}

/** Removes a topic from the list of topics that should be subscribed **/
void HalrcompSubscribe::removeSocketTopic(const QString &name)
{
    m_socketTopics.remove(name);
}

/** Clears the the topics that should be subscribed **/
void HalrcompSubscribe::clearSocketTopics()
{
    m_socketTopics.clear();
}

/** Connects the 0MQ sockets */
bool HalrcompSubscribe::startSocket()
{
    m_socket = m_context->createSocket(ZMQSocket::TYP_SUB, this);
    m_socket->setLinger(0);

    try {
        m_socket->connectTo(m_socketUri);
    }
    catch (const zmq::error_t &e) {
        QString errorString;
        errorString = QString("Error %1: ").arg(e.num()) + QString(e.what());
        //updateState(SocketError, errorString); TODO
        return false;
    }

    connect(m_socket, &ZMQSocket::messageReceived,
            this, &HalrcompSubscribe::processSocketMessage);


    foreach(QString topic, m_socketTopics)
    {
        m_socket->subscribeTo(topic.toLocal8Bit());
    }

#ifdef QT_DEBUG
    DEBUG_TAG(1, m_debugName, "sockets connected" << m_socketUri);
#endif

    return true;
}

/** Disconnects the 0MQ sockets */
void HalrcompSubscribe::stopSocket()
{
    if (m_socket != nullptr)
    {
        m_socket->close();
        m_socket->deleteLater();
        m_socket = nullptr;
    }
}

void HalrcompSubscribe::resetHeartbeatLiveness()
{
    m_heartbeatLiveness = m_heartbeatResetLiveness;
}

void HalrcompSubscribe::resetHeartbeatTimer()
{
    if (m_heartbeatTimer->isActive())
    {
        m_heartbeatTimer->stop();
    }

    if (m_heartbeatInterval > 0)
    {
        m_heartbeatTimer->setInterval(m_heartbeatInterval);
        m_heartbeatTimer->start();
    }
}

void HalrcompSubscribe::startHeartbeatTimer()
{
    resetHeartbeatTimer();
}

void HalrcompSubscribe::stopHeartbeatTimer()
{
    m_heartbeatTimer->stop();
}

void HalrcompSubscribe::heartbeatTimerTick()
{
    m_heartbeatLiveness -= 1;
    if (m_heartbeatLiveness == 0)
    {
         if (m_state == Up)
         {
             emit fsmUpTimeout(QPrivateSignal());
         }
         return;
    }
    if (m_state == Up)
    {
        emit fsmUpTick(QPrivateSignal());
    }
}

/** Processes all message received on socket */
void HalrcompSubscribe::processSocketMessage(const QList<QByteArray> &messageList)
{
    Container &rx = m_socketRx;
    QByteArray topic;

    if (messageList.length() < 2)  // in case we received insufficient data
    {
        return;
    }

    // we only handle the first two messges
    topic = messageList.at(0);
    rx.ParseFromArray(messageList.at(1).data(), messageList.at(1).size());

#ifdef QT_DEBUG
    std::string s;
    gpb::TextFormat::PrintToString(rx, &s);
    DEBUG_TAG(3, m_debugName, "server message" << QString::fromStdString(s));
#endif

    // react to any incoming message

    if (m_state == Up)
    {
        emit fsmUpMessageReceived(QPrivateSignal());
    }

    // react to ping message
    if (rx.type() == MT_PING)
    {
        return; // ping is uninteresting
    }

    // react to halrcomp full update message
    if (rx.type() == MT_HALRCOMP_FULL_UPDATE)
    {
        if (rx.has_pparams())
        {
            ProtocolParameters pparams = rx.pparams();
            m_heartbeatInterval = pparams.keepalive_timer();
        }

        if (m_state == Trying)
        {
            emit fsmTryingConnected(QPrivateSignal());
        }
    }

    emit socketMessageReceived(topic, rx);
}

void HalrcompSubscribe::socketError(int errorNum, const QString &errorMsg)
{
    QString errorString;
    errorString = QString("Error %1: ").arg(errorNum) + errorMsg;
}

void HalrcompSubscribe::fsmDown()
{
#ifdef QT_DEBUG
    DEBUG_TAG(1, m_debugName, "State DOWN");
#endif
    m_state = Down;
    emit stateChanged(m_state);
}

void HalrcompSubscribe::fsmDownConnectEvent()
{
    if (m_state == Down)
    {
#ifdef QT_DEBUG
        DEBUG_TAG(1, m_debugName, "Event CONNECT");
#endif
        // handle state change
        emit fsmDownExited(QPrivateSignal());
        fsmTrying();
        emit fsmTryingEntered(QPrivateSignal());
        // execute actions
        startSocket();
     }
}

void HalrcompSubscribe::fsmTrying()
{
#ifdef QT_DEBUG
    DEBUG_TAG(1, m_debugName, "State TRYING");
#endif
    m_state = Trying;
    emit stateChanged(m_state);
}

void HalrcompSubscribe::fsmTryingConnectedEvent()
{
    if (m_state == Trying)
    {
#ifdef QT_DEBUG
        DEBUG_TAG(1, m_debugName, "Event CONNECTED");
#endif
        // handle state change
        emit fsmTryingExited(QPrivateSignal());
        fsmUp();
        emit fsmUpEntered(QPrivateSignal());
        // execute actions
        resetHeartbeatLiveness();
        startHeartbeatTimer();
     }
}

void HalrcompSubscribe::fsmTryingDisconnectEvent()
{
    if (m_state == Trying)
    {
#ifdef QT_DEBUG
        DEBUG_TAG(1, m_debugName, "Event DISCONNECT");
#endif
        // handle state change
        emit fsmTryingExited(QPrivateSignal());
        fsmDown();
        emit fsmDownEntered(QPrivateSignal());
        // execute actions
        stopHeartbeatTimer();
        stopSocket();
     }
}

void HalrcompSubscribe::fsmUp()
{
#ifdef QT_DEBUG
    DEBUG_TAG(1, m_debugName, "State UP");
#endif
    m_state = Up;
    emit stateChanged(m_state);
}

void HalrcompSubscribe::fsmUpTimeoutEvent()
{
    if (m_state == Up)
    {
#ifdef QT_DEBUG
        DEBUG_TAG(1, m_debugName, "Event TIMEOUT");
#endif
        // handle state change
        emit fsmUpExited(QPrivateSignal());
        fsmTrying();
        emit fsmTryingEntered(QPrivateSignal());
        // execute actions
        stopHeartbeatTimer();
        stopSocket();
        startSocket();
     }
}

void HalrcompSubscribe::fsmUpTickEvent()
{
    if (m_state == Up)
    {
#ifdef QT_DEBUG
        DEBUG_TAG(1, m_debugName, "Event TICK");
#endif
        // execute actions
        resetHeartbeatTimer();
     }
}

void HalrcompSubscribe::fsmUpMessageReceivedEvent()
{
    if (m_state == Up)
    {
#ifdef QT_DEBUG
        DEBUG_TAG(1, m_debugName, "Event MESSAGE RECEIVED");
#endif
        // execute actions
        resetHeartbeatLiveness();
        resetHeartbeatTimer();
     }
}

void HalrcompSubscribe::fsmUpDisconnectEvent()
{
    if (m_state == Up)
    {
#ifdef QT_DEBUG
        DEBUG_TAG(1, m_debugName, "Event DISCONNECT");
#endif
        // handle state change
        emit fsmUpExited(QPrivateSignal());
        fsmDown();
        emit fsmDownEntered(QPrivateSignal());
        // execute actions
        stopHeartbeatTimer();
        stopSocket();
     }
}

/** start trigger function */
void HalrcompSubscribe::start()
{
    if (m_state == Down) {
        emit fsmDownConnect(QPrivateSignal());
    }
}

/** stop trigger function */
void HalrcompSubscribe::stop()
{
    if (m_state == Trying) {
        emit fsmTryingDisconnect(QPrivateSignal());
    }
    if (m_state == Up) {
        emit fsmUpDisconnect(QPrivateSignal());
    }
}
} // namespace halremote
} // namespace machinetalk
