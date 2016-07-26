#include "qapplicationlauncher.h"
#include "debughelper.h"

QApplicationLauncher::QApplicationLauncher(QObject *parent) :
    AbstractServiceImplementation(parent),
    m_subscribeUri(""),
    m_commandUri(""),
    m_commandIdentity("launcher"),
    m_heartbeatPeriod(3000),
    m_connected(false),
    m_subscribeSocketState(Service::Down),
    m_commandSocketState(Service::Down),
    m_connectionState(Service::Disconnected),
    m_error(Service::NoError),
    m_errorString(""),
    m_launchers(QJsonValue(QJsonArray())),
    m_synced(false),
    m_context(nullptr),
    m_subscribeSocket(nullptr),
    m_commandSocket(nullptr),
    m_commandHeartbeatTimer(new QTimer(this)),
    m_subscribeHeartbeatTimer(new QTimer(this)),
    m_commandPingOutstanding(false)
{
    connect(m_commandHeartbeatTimer, SIGNAL(timeout()),
            this, SLOT(commandHeartbeatTimerTick()));
    connect(m_subscribeHeartbeatTimer, SIGNAL(timeout()),
            this, SLOT(subscribeHeartbeatTimerTick()));

    initializeObject();
}

QApplicationLauncher::~QApplicationLauncher()
{
    Service::removeTempPath("launcher"); // clean up dir created by json
}

void QApplicationLauncher::start(int index)
{
    if (!m_connected) {
        return;
    }

#ifdef QT_DEBUG
    DEBUG_TAG(1, m_commandIdentity, "starting launcher" << index)
#endif

    m_tx.set_index(index);
    sendCommandMessage(pb::MT_LAUNCHER_START);
}

void QApplicationLauncher::kill(int index)
{
    if (!m_connected) {
        return;
    }

    m_tx.set_index(index);
    sendCommandMessage(pb::MT_LAUNCHER_KILL);
}

void QApplicationLauncher::terminate(int index)
{
    if (!m_connected) {
        return;
    }

    m_tx.set_index(index);
    sendCommandMessage(pb::MT_LAUNCHER_TERMINATE);
}

void QApplicationLauncher::writeToStdin(int index, const QString &data)
{
    if (!m_connected) {
        return;
    }

    m_tx.set_index(index);
    m_tx.set_name(data.toStdString());
    sendCommandMessage(pb::MT_LAUNCHER_WRITE_STDIN);
}

void QApplicationLauncher::call(const QString &command)
{
    if (!m_connected) {
        return;
    }

    m_tx.set_name(command.toStdString());
    sendCommandMessage(pb::MT_LAUNCHER_CALL);
}

void QApplicationLauncher::shutdown()
{
    if (!m_connected) {
        return;
    }

    sendCommandMessage(pb::MT_LAUNCHER_SHUTDOWN);
}

/** Connects the 0MQ sockets */
bool QApplicationLauncher::connectSockets()
{
    m_context = new PollingZMQContext(this, 1);
    connect(m_context, SIGNAL(pollError(int,QString)),
            this, SLOT(pollError(int,QString)));
    m_context->start();

    m_commandSocket = m_context->createSocket(ZMQSocket::TYP_DEALER, this);
    m_commandSocket->setLinger(0);
    m_commandSocket->setIdentity(QString("%1-%2").arg(m_commandIdentity).arg(QCoreApplication::applicationPid()).toLocal8Bit());

    m_subscribeSocket = m_context->createSocket(ZMQSocket::TYP_SUB, this);
    m_subscribeSocket->setLinger(0);

    try {
        m_commandSocket->connectTo(m_commandUri);
        m_subscribeSocket->connectTo(m_subscribeUri);
    }
    catch (const zmq::error_t &e) {
        QString errorString;
        errorString = QString("Error %1: ").arg(e.num()) + QString(e.what());
        updateState(Service::Error, Service::SocketError, errorString);
        return false;
    }

    connect(m_subscribeSocket, SIGNAL(messageReceived(QList<QByteArray>)),
            this, SLOT(subscribeMessageReceived(QList<QByteArray>)));
    connect(m_commandSocket, SIGNAL(messageReceived(QList<QByteArray>)),
            this, SLOT(commandMessageReceived(QList<QByteArray>)));

#ifdef QT_DEBUG
    DEBUG_TAG(1, m_commandIdentity, "sockets connected" << m_subscribeUri << m_commandUri)
#endif

    return true;
}

/** Disconnects the 0MQ sockets */
void QApplicationLauncher::disconnectSockets()
{
    m_commandSocketState = Service::Down;
    m_subscribeSocketState = Service::Down;

    if (m_commandSocket != nullptr)
    {
        m_commandSocket->close();
        m_commandSocket->deleteLater();
        m_commandSocket = nullptr;
    }

    if (m_subscribeSocket != nullptr)
    {
        m_subscribeSocket->close();
        m_subscribeSocket->deleteLater();
        m_subscribeSocket = nullptr;
    }

    if (m_context != nullptr)
    {
        m_context->stop();
        m_context->deleteLater();
        m_context = nullptr;
    }
}

void QApplicationLauncher::subscribe(const QString &topic)
{
    m_subscribeSocketState = Service::Trying;
    m_subscribeSocket->subscribeTo(topic.toLocal8Bit());
}

void QApplicationLauncher::unsubscribe(const QString &topic)
{
    m_subscribeSocketState = Service::Down;
    m_subscribeSocket->unsubscribeFrom(topic.toLocal8Bit());
}

void QApplicationLauncher::start()
{
#ifdef QT_DEBUG
   DEBUG_TAG(1, m_commandIdentity, "start")
#endif
    m_commandSocketState = Service::Trying;
    updateState(Service::Connecting);

    if (connectSockets())
    {
        subscribe("launcher");
        startCommandHeartbeat();
        sendCommandMessage(pb::MT_PING);
    }
}

void QApplicationLauncher::stop()
{
#ifdef QT_DEBUG
    DEBUG_TAG(1, m_commandIdentity, "stop")
#endif

    cleanup();

    updateState(Service::Disconnected);  // clears also the error
}

void QApplicationLauncher::cleanup()
{
    if (m_connected)
    {
        unsubscribe("launcher");
    }
    stopCommandHeartbeat();
    disconnectSockets();
}

void QApplicationLauncher::startCommandHeartbeat()
{
    m_commandPingOutstanding = false;

    if (m_heartbeatPeriod > 0)
    {
        m_commandHeartbeatTimer->setInterval(m_heartbeatPeriod);
        m_commandHeartbeatTimer->start();
    }
}

void QApplicationLauncher::stopCommandHeartbeat()
{
    m_commandHeartbeatTimer->stop();
}

void QApplicationLauncher::startSubscribeHeartbeat(int interval)
{
    m_subscribeHeartbeatTimer->stop();

    if (interval > 0)
    {
        m_subscribeHeartbeatTimer->setInterval(interval);
        m_subscribeHeartbeatTimer->start();
    }
}

void QApplicationLauncher::stopSubscribeHeartbeat()
{
    m_subscribeHeartbeatTimer->stop();
}

void QApplicationLauncher::refreshSubscribeHeartbeat()
{
    if (m_subscribeHeartbeatTimer->isActive())
    {
        m_subscribeHeartbeatTimer->stop();
        m_subscribeHeartbeatTimer->start();
    }
}

void QApplicationLauncher::updateState(Service::State state)
{
    updateState(state, Service::NoError, "");
}

void QApplicationLauncher::updateState(Service::State state, Service::ConnectionError error, QString errorString)
{
    if (state != m_connectionState)
    {
        if (m_connected) // we are not connected anymore
        {
            stopSubscribeHeartbeat();
            clearSync();
            m_connected = false;
            emit connectedChanged(false);
        }
        else if (state == Service::Connected) {
            m_connected = true;
            emit connectedChanged(true);
        }

        m_connectionState = state;
        emit connectionStateChanged(m_connectionState);

        if ((state == Service::Disconnected) || (state == Service::Error)) {
            initializeObject();
        }
    }

    updateError(error, errorString);
}

void QApplicationLauncher::updateError(Service::ConnectionError error, QString errorString)
{
    if (m_errorString != errorString)
    {
        m_errorString = errorString;
        emit errorStringChanged(m_errorString);
    }

    if (m_error != error)
    {
        if (error != Service::NoError)
        {
            cleanup();
        }
        m_error = error;
        emit errorChanged(m_error);
    }
}

void QApplicationLauncher::pollError(int errorNum, const QString &errorMsg)
{
    QString errorString;
    errorString = QString("Error %1: ").arg(errorNum) + errorMsg;
    updateState(Service::Error, Service::SocketError, errorString);
}

/** Processes all message received on the update 0MQ socket */
void QApplicationLauncher::subscribeMessageReceived(QList<QByteArray> messageList)
{
    QByteArray topic;

    topic = messageList.at(0);
    m_rx.ParseFromArray(messageList.at(1).data(), messageList.at(1).size());

#ifdef QT_DEBUG
    std::string s;
    gpb::TextFormat::PrintToString(m_rx, &s);
    DEBUG_TAG(3, m_commandIdentity, "launcher update" << topic << QString::fromStdString(s))
#endif

    if (m_rx.type() == pb::MT_LAUNCHER_FULL_UPDATE) //value update
    {
        m_launchers = QJsonValue(QJsonArray()); // clear old value
        Service::updateValue(m_rx, &m_launchers, "launcher", "launcher"); // launcher protobuf value, launcher temp path
        emit launchersChanged(m_launchers);

        if (m_subscribeSocketState != Service::Up)
        {
            m_subscribeSocketState = Service::Up;
            updateState(Service::Connected);
        }

        updateSync();

        if (m_rx.has_pparams())
        {
            pb::ProtocolParameters pparams = m_rx.pparams();
            startSubscribeHeartbeat(pparams.keepalive_timer() * 2);  // wait double the time of the hearbeat interval
        }
    }
    else if (m_rx.type() == pb::MT_LAUNCHER_INCREMENTAL_UPDATE){
        Service::updateValue(m_rx, &m_launchers, "launcher", "launcher"); // launcher protobuf value, launcher temp path
        emit launchersChanged(m_launchers);

        refreshSubscribeHeartbeat();
    }
    else if (m_rx.type() == pb::MT_PING)
    {
        if (m_subscribeSocketState == Service::Up)
        {
            refreshSubscribeHeartbeat();
        }
        else
        {
            updateState(Service::Connecting);
            unsubscribe("launcher");  // clean up previous subscription
            subscribe("launcher");    // trigger a fresh subscribe -> full update
        }
    }
    else if (m_rx.type() == pb::MT_LAUNCHER_ERROR)
    {
        QString errorString;

        for (int i = 0; i < m_rx.note_size(); ++i)
        {
            errorString.append(QString::fromStdString(m_rx.note(i)) + "\n");
        }

        m_subscribeSocketState = Service::Down;
        updateState(Service::Error, Service::CommandError, errorString);

#ifdef QT_DEBUG
        DEBUG_TAG(1, m_commandIdentity, "proto error on subscribe" << errorString)
#endif
    }
    else
    {
#ifdef QT_DEBUG
        gpb::TextFormat::PrintToString(m_rx, &s);
        DEBUG_TAG(1, m_commandIdentity, "status_update: unknown message type: " << QString::fromStdString(s))
#endif
    }
}

/** Processes all message received on the command 0MQ socket */
void QApplicationLauncher::commandMessageReceived(QList<QByteArray> messageList)
{
    m_rx.ParseFromArray(messageList.at(0).data(), messageList.at(0).size());

#ifdef QT_DEBUG
    std::string s;
    gpb::TextFormat::PrintToString(m_rx, &s);
    DEBUG_TAG(3, m_commandIdentity, "server message" << QString::fromStdString(s))
#endif

    if (m_rx.type() == pb::MT_PING_ACKNOWLEDGE)
    {
        m_commandPingOutstanding = false;

        if (m_commandSocketState != Service::Up)
        {
            m_commandSocketState = Service::Up;
            updateState(Service::Connected);
        }

#ifdef QT_DEBUG
        DEBUG_TAG(2, m_commandIdentity, "ping ack")
#endif

        return;
    }
    else if (m_rx.type() == pb::MT_ERROR)
    {
        QString errorString;

        for (int i = 0; i < m_rx.note_size(); ++i)
        {
            errorString.append(QString::fromStdString(m_rx.note(i)) + "\n");
        }

        m_commandSocketState = Service::Down;
        updateState(Service::Error, Service::ServiceError, errorString);

#ifdef QT_DEBUG
        DEBUG_TAG(1, "command", "error" << errorString)
#endif

        return;
    }
    else
    {
#ifdef QT_DEBUG
        DEBUG_TAG(1, m_commandIdentity, "UNKNOWN server message type")
#endif
    }
}

void QApplicationLauncher::sendCommandMessage(pb::ContainerType type)
{
    if (m_commandSocket == nullptr) {  // disallow sending messages when not connected
        return;
    }

    try {
        m_tx.set_type(type);
        m_commandSocket->sendMessage(QByteArray(m_tx.SerializeAsString().c_str(), m_tx.ByteSize()));
        m_tx.Clear();
    }
    catch (const zmq::error_t &e) {
        QString errorString;
        errorString = QString("Error %1: ").arg(e.num()) + QString(e.what());
        updateState(Service::Error, Service::SocketError, errorString);
    }
}

void QApplicationLauncher::updateSync()
{
    m_synced = true;
    emit syncedChanged(m_synced);
}

void QApplicationLauncher::clearSync()
{
    m_synced = false;
    emit syncedChanged(m_synced);
    initializeObject();
}

void QApplicationLauncher::initializeObject()
{
    m_launchers = QJsonValue(QJsonArray());
    emit launchersChanged(m_launchers);
}

void QApplicationLauncher::commandHeartbeatTimerTick()
{
    if (m_commandPingOutstanding)
    {
        m_commandSocketState = Service::Trying;
        updateState(Service::Timeout);

#ifdef QT_DEBUG
        DEBUG_TAG(1, m_commandIdentity, "launchercmd timeout")
#endif
    }

    sendCommandMessage(pb::MT_PING);

    m_commandPingOutstanding = true;

#ifdef QT_DEBUG
    DEBUG_TAG(2, m_commandIdentity, "ping")
#endif
}

void QApplicationLauncher::subscribeHeartbeatTimerTick()
{
    m_subscribeSocketState = Service::Down;
    updateState(Service::Timeout);

#ifdef QT_DEBUG
    DEBUG_TAG(1, m_commandIdentity, "launchercmd timeout")
#endif
}
