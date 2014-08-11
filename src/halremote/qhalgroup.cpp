#include "qhalgroup.h"
#include "debughelper.h"

QHalGroup::QHalGroup(QQuickItem *parent) :
    QQuickItem(parent),
    m_halgroupUri(""),
    m_name("default"),
    m_ready(false),
    m_connectionState(Disconnected),
    m_error(NoError),
    m_errorString(""),
    m_containerItem(this),
    m_componentCompleted(false),
    m_context(NULL),
    m_halgroupSocket(NULL)
{
}

QHalGroup::~QHalGroup()
{
    disconnectSockets();
}

/** componentComplete is executed when the QML component is fully loaded */
void QHalGroup::componentComplete()
{
    m_componentCompleted = true;

    if (m_ready == true)    // the component was set to ready before it was completed
    {
        start();
    }

    QQuickItem::componentComplete();
}

/** Recurses through a list of objects */
QList<QHalSignal *> QHalGroup::recurseObjects(const QObjectList &list) const
{
    QList<QHalSignal*> halSignals;

    foreach (QObject *object, list)
    {
        QHalSignal *halSignal;
        halSignal = qobject_cast<QHalSignal*>(object);
        if (halSignal != NULL)
        {
            halSignals.append(halSignal);
        }

        if (object->children().size() > 0)
        {
            halSignals.append(recurseObjects(object->children()));
        }
    }

    return halSignals;
}

void QHalGroup::start()
{
#ifdef QT_DEBUG
   DEBUG_TAG(1, m_name, "start")
#endif
    updateState(Connecting);


    if (connectSockets())
    {
        addSignals();
    }
}

void QHalGroup::stop()
{
#ifdef QT_DEBUG
    DEBUG_TAG(1, m_name, "stop")
#endif

    // cleanup here
    disconnectSockets();
    removeSignals();

    updateState(Disconnected);
    updateError(NoError, "");   // clear the error here
}

void QHalGroup::updateState(QHalGroup::State state)
{
    if (state != m_connectionState)
    {
        m_connectionState = state;
        emit connectionStateChanged(m_connectionState);
    }
}

void QHalGroup::updateError(QHalGroup::ConnectionError error, const QString &errorString)
{
    m_error = error;
    m_errorString = errorString;

    emit errorStringChanged(m_errorString);
    emit errorChanged(m_error);
}

/** Updates a local signal with the value of a remote signal */
void QHalGroup::signalUpdate(const pb::Signal &remoteSignal, QHalSignal *localSignal)
{
#ifdef QT_DEBUG
    DEBUG_TAG(2, m_name,  "signal update" << localSignal->name() << remoteSignal.halfloat() << remoteSignal.halbit() << remoteSignal.hals32() << remoteSignal.halu32())
#endif

    if (remoteSignal.has_halfloat())
    {
        localSignal->setValue(QVariant(remoteSignal.halfloat()));
    }
    else if (remoteSignal.has_halbit())
    {
        localSignal->setValue(QVariant(remoteSignal.halbit()));
    }
    else if (remoteSignal.has_hals32())
    {
        localSignal->setValue(QVariant(remoteSignal.hals32()));
    }
    else if (remoteSignal.has_halu32())
    {
        localSignal->setValue(QVariant(remoteSignal.halu32()));
    }
}

void QHalGroup::halgroupMessageReceived(const QList<QByteArray> &messageList)
{
    QByteArray topic;

    topic = messageList.at(0);
    m_rx.ParseFromArray(messageList.at(1).data(), messageList.at(1).size());

#ifdef QT_DEBUG
    std::string s;
    gpb::TextFormat::PrintToString(m_rx, &s);
    DEBUG_TAG(3, m_name, "halgroup update" << topic << QString::fromStdString(s))
#endif

    if (m_rx.type() == pb::MT_HALGROUP_INCREMENTAL_UPDATE) // incremental update
    {
        for (int i = 0; i < m_rx.signal_size(); ++i)
        {
            pb::Signal remoteSignal = m_rx.signal(i);
            QHalSignal *localSignal = m_signalsByHandle.value(remotePin.handle());
            signalUpdate(remoteSignal, localSignal);
        }

        return;
    }
    else if (m_rx.type() == pb::MT_HALGROUP_FULL_UPDATE) // full update
    {
        return;
    }
    else if (m_rx.type() == pb::MT_HALGROUP_ERROR) // error
    {
        return;
    }

#ifdef QT_DEBUG
    gpb::TextFormat::PrintToString(m_rx, &s);
    DEBUG_TAG(1, m_name, "halgroup_update: unknown message type: " << QString::fromStdString(s))
#endif

    return;
}

void QHalGroup::pollError(int errorNum, const QString &errorMsg)
{
    QString errorString;
    errorString = QString("Error %1: ").arg(errorNum) + errorMsg;
    updateError(SocketError, errorString);
    updateState(Error);
}

/** Scans all children of the container item for signals and adds them to a map */
void QHalGroup::addSignals()
{
    QList<QHalSignal*> halSignals;

    if (m_containerItem == NULL)
    {
        return;
    }

    halSignals = recurseObjects(m_containerItem->children());
    foreach (QHalSignal *signal, halSignals)
    {
        if (signal->name().isEmpty() || (signal->enabled() == false))   // ignore signals with empty name or disabled
        {
            continue;
        }
        m_signalsByName.insert(signal->name(), signal);
#ifdef QT_DEBUG
        DEBUG_TAG(1, m_name, "signal added: " << signal->name())
#endif
    }
}

/** Removes all previously added signals */
void QHalGroup::removeSignals()
{
    m_signalsByHandle.clear();
    m_signalsByName.clear();
}

/** Connects the 0MQ sockets */
bool QHalGroup::connectSockets()
{
    m_context = new PollingZMQContext(this, 1);
    connect(m_context, SIGNAL(pollError(int,QString)),
            this, SLOT(pollError(int,QString)));
    m_context->start();

    m_halgroupSocket = m_context->createSocket(ZMQSocket::TYP_SUB, this);
    m_halgroupSocket->setLinger(0);

    try {
        m_halgroupSocket->connectTo(m_halgroupUri);
    }
    catch (zmq::error_t e) {
        QString errorString;
        errorString = QString("Error %1: ").arg(e.num()) + QString(e.what());
        updateError(SocketError, errorString);
        updateState(Error);
        return false;
    }

    connect(m_halgroupSocket, SIGNAL(messageReceived(QList<QByteArray>)),
            this, SLOT(halgroupMessageReceived(QList<QByteArray>)));

#ifdef QT_DEBUG
    DEBUG_TAG(1, m_name, "socket connected" << m_halgroupUri)
#endif

    return true;
}

/** Disconnects the 0MQ sockets */
void QHalGroup::disconnectSockets()
{
    if (m_halgroupSocket != NULL)
    {
        m_halgroupSocket->close();
        m_halgroupSocket->deleteLater();
        m_halgroupSocket = NULL;
    }

    if (m_context != NULL)
    {
        m_context->stop();
        m_context->deleteLater();
        m_context = NULL;
    }
}

/** If the ready property has a rising edge we try to connect
 *  if it is has a falling edge we disconnect and cleanup
 */
void QHalGroup::setReady(bool arg)
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
    }
}
