/****************************************************************************
**
** This code was generated by a code generator based on imatix/gsl
** Any changes in this code will be lost.
**
****************************************************************************/
#include "rpcservice.h"
#include "debughelper.h"

#if defined(Q_OS_IOS)
namespace gpb = google_public::protobuf;
#else
namespace gpb = google::protobuf;
#endif

using namespace nzmqt;

namespace machinetalk {

/** Generic RPC Service implementation */
RpcService::RpcService(QObject *parent) :
    QObject(parent),
    m_ready(false),
    m_debugName("RPC Service"),
    m_socketUri(""),
    m_context(nullptr),
    m_socket(nullptr),
    m_state(Down),
    m_previousState(Down),
    m_fsm(nullptr),
    m_errorString("")
{

    m_fsm = new QStateMachine(this);
    QState *downState = new QState(m_fsm);
    connect(downState, &QState::entered, this, &RpcService::fsmDownEntered, Qt::QueuedConnection);
    QState *upState = new QState(m_fsm);
    connect(upState, &QState::entered, this, &RpcService::fsmUpEntered, Qt::QueuedConnection);
    m_fsm->setInitialState(downState);
    m_fsm->start();

    connect(this, &RpcService::fsmDownStart,
            this, &RpcService::fsmDownStartQueued, Qt::QueuedConnection);
    downState->addTransition(this, &RpcService::fsmDownStartQueued, upState);
    connect(this, &RpcService::fsmUpPingReceived,
            this, &RpcService::fsmUpPingReceivedQueued, Qt::QueuedConnection);
    upState->addTransition(this, &RpcService::fsmUpPingReceivedQueued, upState);
    connect(this, &RpcService::fsmUpStop,
            this, &RpcService::fsmUpStopQueued, Qt::QueuedConnection);
    upState->addTransition(this, &RpcService::fsmUpStopQueued, downState);

    connect(this, &RpcService::fsmDownStart,
            this, &RpcService::fsmDownStartEvent, Qt::QueuedConnection);
    connect(this, &RpcService::fsmUpPingReceived,
            this, &RpcService::fsmUpPingReceivedEvent, Qt::QueuedConnection);
    connect(this, &RpcService::fsmUpStop,
            this, &RpcService::fsmUpStopEvent, Qt::QueuedConnection);

    m_context = new PollingZMQContext(this, 1);
    connect(m_context, &PollingZMQContext::pollError,
            this, &RpcService::socketError);
    m_context->start();
}

RpcService::~RpcService()
{
    stopSocket();

    if (m_context != nullptr)
    {
        m_context->stop();
        m_context->deleteLater();
        m_context = nullptr;
    }
}

/** Connects the 0MQ sockets */
bool RpcService::startSocket()
{
    m_socket = m_context->createSocket(ZMQSocket::TYP_ROUTER, this);
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
            this, &RpcService::processSocketMessage);


#ifdef QT_DEBUG
    DEBUG_TAG(1, m_debugName, "sockets connected" << m_socketUri);
#endif

    return true;
}

/** Disconnects the 0MQ sockets */
void RpcService::stopSocket()
{
    if (m_socket != nullptr)
    {
        m_socket->close();
        m_socket->deleteLater();
        m_socket = nullptr;
    }
}

/** Processes all message received on socket */
void RpcService::processSocketMessage(const QList<QByteArray> &messageList)
{
    pb::Container &rx = m_socketRx;
    rx.ParseFromArray(messageList.at(0).data(), messageList.at(0).size());

#ifdef QT_DEBUG
    std::string s;
    gpb::TextFormat::PrintToString(rx, &s);
    DEBUG_TAG(3, m_debugName, "server message" << QString::fromStdString(s));
#endif

    // react to ping message
    if (rx.type() == pb::MT_PING)
    {

        if (m_state == Up)
        {
            emit fsmUpPingReceived();
        }
        return; // ping is uninteresting
    }

    emit socketMessageReceived(rx);
}

void RpcService::sendSocketMessage(pb::ContainerType type, pb::Container *tx)
{
    if (m_socket == nullptr) {  // disallow sending messages when not connected
        return;
    }

    tx->set_type(type);
#ifdef QT_DEBUG
    std::string s;
    gpb::TextFormat::PrintToString(*tx, &s);
    DEBUG_TAG(3, m_debugName, "sent message" << QString::fromStdString(s));
#endif
    try {
        m_socket->sendMessage(QByteArray(tx->SerializeAsString().c_str(), tx->ByteSize()));
    }
    catch (const zmq::error_t &e) {
        QString errorString;
        errorString = QString("Error %1: ").arg(e.num()) + QString(e.what());
        //updateState(SocketError, errorString);  TODO
        return;
    }
    tx->Clear();
}

void RpcService::sendPingAcknowledge()
{
    pb::Container *tx = &m_socketTx;
    sendSocketMessage(pb::MT_PING_ACKNOWLEDGE, tx);
}

void RpcService::socketError(int errorNum, const QString &errorMsg)
{
    QString errorString;
    errorString = QString("Error %1: ").arg(errorNum) + errorMsg;
    //updateState(SocketError, errorString);  TODO
}

void RpcService::fsmDownEntered()
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

void RpcService::fsmDownStartEvent()
{
#ifdef QT_DEBUG
    DEBUG_TAG(1, m_debugName, "Event START");
#endif

    m_state = Up;
    startSocket();
}

void RpcService::fsmUpEntered()
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

void RpcService::fsmUpPingReceivedEvent()
{
#ifdef QT_DEBUG
    DEBUG_TAG(1, m_debugName, "Event PING RECEIVED");
#endif

    m_state = Up;
    sendPingAcknowledge();
}

void RpcService::fsmUpStopEvent()
{
#ifdef QT_DEBUG
    DEBUG_TAG(1, m_debugName, "Event STOP");
#endif

    m_state = Down;
    stopSocket();
}

/** start trigger */
void RpcService::start()
{
    if (m_state == Down) {
        emit fsmDownStart();
    }
}

/** stop trigger */
void RpcService::stop()
{
    if (m_state == Up) {
        emit fsmUpStop();
    }
}
}; // namespace machinetalk
