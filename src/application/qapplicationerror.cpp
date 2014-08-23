#include "qapplicationerror.h"
#include "debughelper.h"

QApplicationError::QApplicationError(QQuickItem *parent) :
    QQuickItem(parent),
    m_errorUri(""),
    m_ready(false),
    m_sState(Down),
    m_connectionState(Disconnected),
    m_error(NoError),
    m_errorString(""),
    m_channels(ErrorChannel | TextChannel | DisplayChannel),
    m_componentCompleted(false),
    m_context(NULL),
    m_errorSocket(NULL),
    m_errorHeartbeatTimer(new QTimer(this)),
    m_errorPingOutstanding(false)
{
   connect(m_errorHeartbeatTimer, SIGNAL(timeout()),
           this, SLOT(errorHeartbeatTimerTick()));
}

QApplicationError::~QApplicationError()
{
    disconnectSockets();
}

/** componentComplete is executed when the QML component is fully loaded */
void QApplicationError::componentComplete()
{
   m_componentCompleted = true;

   if (m_ready == true)    // the component was set to ready before it was completed
   {
       start();
   }

   QQuickItem::componentComplete();
}

void QApplicationError::start()
{
#ifdef QT_DEBUG
   DEBUG_TAG(1, "error", "start")
#endif
    updateState(Connecting);


    if (connectSockets())
    {
        subscribe();
    }
}

void QApplicationError::stop()
{
#ifdef QT_DEBUG
    DEBUG_TAG(1, "error", "stop")
#endif

    // cleanup here
    stopErrorHeartbeat();
    disconnectSockets();

    updateState(Disconnected);
    updateError(NoError, "");   // clear the error here
}

void QApplicationError::startErrorHeartbeat(int interval)
{
    m_errorHeartbeatTimer->stop();
    m_errorPingOutstanding = false;

    if (interval > 0)
    {
        m_errorHeartbeatTimer->setInterval(interval);
        m_errorHeartbeatTimer->start();
    }
}

void QApplicationError::stopErrorHeartbeat()
{
    m_errorHeartbeatTimer->stop();
}

void QApplicationError::refreshErrorHeartbeat()
{
    if (m_errorHeartbeatTimer->isActive())
    {
        m_errorHeartbeatTimer->stop();
        m_errorHeartbeatTimer->start();
    }
}

void QApplicationError::updateState(QApplicationError::State state)
{
    if (state != m_connectionState)
    {
        if (m_connectionState == Connected) // we are not connected anymore
        {
            stopErrorHeartbeat();
        }

        m_connectionState = state;
        emit connectionStateChanged(m_connectionState);
    }
}

void QApplicationError::updateError(QApplicationError::ConnectionError error, const QString &errorString)
{
    m_error = error;
    m_errorString = errorString;

    emit errorStringChanged(m_errorString);
    emit errorChanged(m_error);
}

void QApplicationError::errorMessageReceived(const QList<QByteArray> &messageList)
{
    QByteArray topic;

    topic = messageList.at(0);
    m_rx.ParseFromArray(messageList.at(1).data(), messageList.at(1).size());

#ifdef QT_DEBUG
    std::string s;
    gpb::TextFormat::PrintToString(m_rx, &s);
    DEBUG_TAG(3, "error", "update" << topic << QString::fromStdString(s))
#endif

    if ((m_rx.type() == pb::MT_EMC_NML_ERROR)
        || (m_rx.type() == pb::MT_EMC_NML_TEXT)
        || (m_rx.type() == pb::MT_EMC_NML_DISPLAY)
        || (m_rx.type() == pb::MT_EMC_OPERATOR_TEXT)
        || (m_rx.type() == pb::MT_EMC_OPERATOR_ERROR)
        || (m_rx.type() == pb::MT_EMC_OPERATOR_TEXT))
    {
        for (int i = 0; i < m_rx.note_size(); ++i)
        {
            messageReceived((ErrorType)m_rx.type(), QString::fromStdString(m_rx.note(i)));
        }

        if (m_sState != Up)
        {
            m_sState = Up;
            updateError(NoError, "");
        }

        if (m_rx.has_pparams())
        {
            pb::ProtocolParameters pparams = m_rx.pparams();
            startErrorHeartbeat(pparams.keepalive_timer() * 2); // wait double the time of the hearbeat interval
        }

        return;
    }
    else if (m_rx.type() == pb::MT_PING)
    {
        refreshErrorHeartbeat();

        return;
    }

#ifdef QT_DEBUG
    gpb::TextFormat::PrintToString(m_rx, &s);
    DEBUG_TAG(1, "error", "update: unknown message type: " << QString::fromStdString(s))
#endif
}

void QApplicationError::pollError(int errorNum, const QString &errorMsg)
{
    QString errorString;
    errorString = QString("Error %1: ").arg(errorNum) + errorMsg;
    updateError(SocketError, errorString);
    updateState(Error);
}

void QApplicationError::errorHeartbeatTimerTick()
{
    unsubscribe();
    updateError(TimeoutError, "Error service timed out");
    updateState(Error);

#ifdef QT_DEBUG
    DEBUG_TAG(1, "error", "timeout")
#endif

    subscribe();    // trigger a fresh subscribe

    m_errorPingOutstanding = true;
}

/** Connects the 0MQ sockets */
bool QApplicationError::connectSockets()
{
    m_context = new PollingZMQContext(this, 1);
    connect(m_context, SIGNAL(pollError(int,QString)),
            this, SLOT(pollError(int,QString)));
    m_context->start();

    m_errorSocket = m_context->createSocket(ZMQSocket::TYP_SUB, this);
    m_errorSocket->setLinger(0);

    try {
        m_errorSocket->connectTo(m_errorUri);
    }
    catch (zmq::error_t e) {
        QString errorString;
        errorString = QString("Error %1: ").arg(e.num()) + QString(e.what());
        updateError(SocketError, errorString);
        updateState(Error);
        return false;
    }

    connect(m_errorSocket, SIGNAL(messageReceived(QList<QByteArray>)),
            this, SLOT(errorMessageReceived(QList<QByteArray>)));

#ifdef QT_DEBUG
    DEBUG_TAG(1, "error", "socket connected" << m_errorUri)
#endif

            return true;
}

/** Disconnects the 0MQ sockets */
void QApplicationError::disconnectSockets()
{
    if (m_errorSocket != NULL)
    {
        m_errorSocket->close();
        m_errorSocket->deleteLater();
        m_errorSocket = NULL;
    }

    if (m_context != NULL)
    {
        m_context->stop();
        m_context->deleteLater();
        m_context = NULL;
    }
}

void QApplicationError::subscribe()
{
    m_sState = Trying;

    if (m_channels | ErrorChannel) {
        m_errorSocket->subscribeTo("error");
        m_subscriptions.append("error");
    }
    if (m_channels | TextChannel) {
        m_errorSocket->subscribeTo("text");
        m_subscriptions.append("text");
    }
    if (m_channels | DisplayChannel) {
        m_errorSocket->subscribeTo("display");
        m_subscriptions.append("display");
    }
}

void QApplicationError::unsubscribe()
{
    m_sState = Down;
    foreach (QString subscription, m_subscriptions)
    {
        m_errorSocket->unsubscribeFrom(subscription);
    }
    m_subscriptions.clear();
}

/** If the ready property has a rising edge we try to connect
 *  if it is has a falling edge we disconnect and cleanup
 */
void QApplicationError::setReady(bool arg)
{
    if (m_ready != arg) {
        m_ready = arg;
        emit readyChanged(arg);

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
    };
}
