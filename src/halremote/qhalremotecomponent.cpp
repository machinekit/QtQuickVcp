/****************************************************************************
**
** Copyright (C) 2014 Alexander Rössler
** License: LGPL version 2.1
**
** This file is part of QtQuickVcp.
**
** All rights reserved. This program and the accompanying materials
** are made available under the terms of the GNU Lesser General Public License
** (LGPL) version 2.1 which accompanies this distribution, and is available at
** http://www.gnu.org/licenses/lgpl-2.1.html
**
** This library is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
** Lesser General Public License for more details.
**
** Contributors:
** Alexander Rössler @ The Cool Tool GmbH <mail DOT aroessler AT gmail DOT com>
**
****************************************************************************/
#include "qhalremotecomponent.h"
#include "debughelper.h"

/*!
    \qmltype HalRemoteComponent
    \instantiates QHalRemoteComponent
    \inqmlmodule Machinekit.HalRemote
    \brief A HAL remote component.
    \ingroup halremote

    This component provides the counterpart of a HAL
    remote component in the HAL real-time environment.
    The HalRemoteComponent connects to a remote HAL instance
    using the halrcmd and halrcomp services provided by
    a Haltalk instance running on the remote host.

    A HalRemoteComponent needs the \l{halrcmdUri},
    \l{halrcompUri} and \l containerItem set in order
    to work.

    The HalRemoteComponent scans the \l containerItem
    and its children for \l{HalPin}s when \l ready is set
    to \c true.

    The following example creates a HAL remote component
    \c myComponent with one pin \c myPin. The resulting
    name for the pin inside HAL is \c myComponent.myPin.

    \qml
    Item {
        Item {
            id: container

            HalPin {
                name: "myPin"
                type: HalPin.Float
                direction: HalPin.Out
            }
        }

        HalRemoteComponent {
            id: halRemoteComponent

            name: "myComponent"
            halrcmdUri: "tcp://192.168.1.2:5001"
            halrcompUri: "tcp://192.168.1.2:5002"
            containerItem: container
            ready: true
        }
    }
    \endqml
*/

/*! \qmlproperty string HalRemoteComponent::halrcmdUri

    This property holds the halrcmd service uri.
*/

/*! \qmlproperty string HalRemoteComponent::halrcompUri

    This property holds the halrcomp service uri.
*/

/*! \qmlproperty string HalRemoteComponent::name

    This property holds the name of the remote component.
*/

/*! \qmlproperty int HalRemoteComponent::heartbeadPeriod

    This property holds the period time of the heartbeat timer in ms.
    Set this property to \c{0} to disable the hearbeat.

    The default value is \c{3000}.
*/

/*! \qmlproperty bool HalRemoteComponent::ready

    This property holds whether the HalRemoteComponent is ready or not.
    If the property is set to \c true the component will try to connect. If the
    property is set to \c false all connections will be closed.

    The default value is \c{false}.
*/

/*! \qmlproperty bool HalRemoteComponent::connected

    This property holds wheter the HAL remote component is connected or not. This is the
    same as \l{connectionState} == \c{HalRemoteComponent.Connected}.
 */

/*! \qmlproperty enumeration HalRemoteComponent::connectionState

    This property holds the connection state of the HAL remote component.

    \list
    \li HalRemoteComponent.Disconnected - The component is not connected.
    \li HalRemoteComponent.Connecting - The component is trying to connect to the remote component.
    \li HalRemoteComponent.Connected - The component is connected and pin changes are accepted.
    \li HalRemoteComponent.Error - An error has happened. See \l error and \l errorString for details about the error.
    \endlist
*/

/*! \qmlproperty enumeration HalRemoteComponent::error

    This property holds the currently active error. See \l errorString
    for a description of the active error.

    \list
    \li HalRemoteComponent.NoError - No error happened.
    \li HalRemoteComponent.BindError - Binding the remote component failed.
    \li HalRemoteComponent.PinChangeError - A pin change was rejected.
    \li HalRemoteComponent.CommandError - A command was rejected.
    \li HalRemoteComponent.TimeoutError - The connection timed out.
    \li HalRemoteComponent.SocketError - An error related to the sockets happened.
    \endlist

    \sa errorString
*/

/*! \qmlproperty string HalRemoteComponent::errorString

    This property holds a text description of the last error that occured.
    If \l error holds a error value check this property for the description.

    \sa error
*/

/*! \qmlproperty Item HalRemoteComponent::containerItem

    This property holds the item that should be scanned for
    \l{HalPin}s.

    The default value is \c{nullptr}.
*/

/*! \qmlproperty Item HalRemoteComponent::create

    Specifies wether the component should be created on bind if it
    does not exist on the remote host.

    The default value is \c{true}.
*/

/*! \qmlproperty Item HalRemoteComponent::bind

    Specifies wether the component should be bound when connecting. If not the
    remote representation of the pins is refrelcted by \c{pins}.

    The default value is \c{true}.
*/

/*! \qmlproperty list<HalPin> HalRemoteComponent::pins

    This property holds a list of HAL pins when bound or connected.
*/

/** Remote HAL Component implementation for use with C++ and QML */
QHalRemoteComponent::QHalRemoteComponent(QObject *parent) :
    AbstractServiceImplementation(parent),
    m_halrcmdUri(""),
    m_halrcompUri(""),
    m_name("default"),
    m_heartbeatPeriod(3000),
    m_connected(false),
    m_halrcompSocketState(Down),
    m_halrcmdSocketState(Down),
    m_connectionState(Disconnected),
    m_error(NoError),
    m_errorString(""),
    m_containerItem(this),
    m_create(true),
    m_bind(true),
    m_context(nullptr),
    m_halrcompSocket(nullptr),
    m_halrcmdSocket(nullptr),
    m_halrcmdHeartbeatTimer(new QTimer(this)),
    m_halrcompHeartbeatTimer(new QTimer(this)),
    m_halrcmdPingOutstanding(false)
{
    m_uuid = QUuid::createUuid();

    connect(m_halrcmdHeartbeatTimer, SIGNAL(timeout()),
            this, SLOT(halrcmdHeartbeatTimerTick()));
    connect(m_halrcompHeartbeatTimer, SIGNAL(timeout()),
            this, SLOT(halrcompHeartbeatTimerTick()));
}

/** Scans all children of the container item for pins and adds them to a map */
void QHalRemoteComponent::addPins()
{
    QObjectList halObjects;

    if (m_containerItem == nullptr)
    {
        return;
    }

    halObjects = recurseObjects(m_containerItem->children());
    foreach (QObject *object, halObjects)
    {
        QHalPin *pin = static_cast<QHalPin *>(object);
        if (pin->name().isEmpty()  || (pin->enabled() == false))    // ignore pins with empty name and disabled pins
        {
            continue;
        }
        m_pinsByName[pin->name()] = pin;
        m_pins.append(pin);
        connect(pin, SIGNAL(valueChanged(QVariant)),
                this, SLOT(pinChange(QVariant)));
#ifdef QT_DEBUG
        DEBUG_TAG(1, m_name, "pin added: " << pin->name())
#endif
    }

    emit pinsChanged(pins());
}

/** Removes all previously added pins */
void QHalRemoteComponent::removePins()
{
    foreach (QHalPin *pin, m_pinsByName)
    {
        disconnect(pin, SIGNAL(valueChanged(QVariant)),
                this, SLOT(pinChange(QVariant)));

        if (pin->parent() == this) // pin was created this class
        {
            pin->deleteLater();
        }
    }

    m_pinsByHandle.clear();
    m_pinsByName.clear();
    m_pins.clear();
    emit pinsChanged(pins());
}

/** Sets synced of all pins to false */
void QHalRemoteComponent::unsyncPins()
{
    QMapIterator<QString, QHalPin*> i(m_pinsByName);
    while (i.hasNext()) {
        i.next();
        i.value()->setSynced(false);
    }
}

/** Connects the 0MQ sockets */
bool QHalRemoteComponent::connectSockets()
{
    m_context = new PollingZMQContext(this, 1);
    connect(m_context, SIGNAL(pollError(int,QString)),
            this, SLOT(pollError(int,QString)));
    m_context->start();

    m_halrcmdSocket = m_context->createSocket(ZMQSocket::TYP_DEALER, this);
    m_halrcmdSocket->setLinger(0);
    m_halrcmdSocket->setIdentity(QString("%1-%2").arg(m_name).arg(m_uuid.toString()).toLocal8Bit());


    m_halrcompSocket = m_context->createSocket(ZMQSocket::TYP_SUB, this);
    m_halrcompSocket->setLinger(0);

    try {
        m_halrcmdSocket->connectTo(m_halrcmdUri);
        m_halrcompSocket->connectTo(m_halrcompUri);
    }
    catch (const zmq::error_t &e) {
        QString errorString;
        errorString = QString("Error %1: ").arg(e.num()) + QString(e.what());
        updateState(Error, SocketError, errorString);
        return false;
    }

    connect(m_halrcompSocket, SIGNAL(messageReceived(QList<QByteArray>)),
            this, SLOT(halrcompMessageReceived(QList<QByteArray>)));
    connect(m_halrcmdSocket, SIGNAL(messageReceived(QList<QByteArray>)),
            this, SLOT(halrcmdMessageReceived(QList<QByteArray>)));

#ifdef QT_DEBUG
    DEBUG_TAG(1, m_name, "sockets connected" << m_halrcompUri << m_halrcmdUri)
#endif

    return true;
}

/** Disconnects the 0MQ sockets */
void QHalRemoteComponent::disconnectSockets()
{
    m_halrcmdSocketState = Down;
    m_halrcompSocketState = Down;

    if (m_halrcmdSocket != nullptr)
    {
        m_halrcmdSocket->close();
        m_halrcmdSocket->deleteLater();
        m_halrcmdSocket = nullptr;
    }

    if (m_halrcompSocket != nullptr)
    {
        m_halrcompSocket->close();
        m_halrcompSocket->deleteLater();
        m_halrcompSocket = nullptr;
    }

    if (m_context != nullptr)
    {
        m_context->stop();
        m_context->deleteLater();
        m_context = nullptr;
    }
}

/** Generates a Bind messages and sends it over the suitable 0MQ socket */
void QHalRemoteComponent::bindPins()
{
    pb::Component *component;

    component = m_tx.add_comp();
    component->set_name(m_name.toStdString());
    component->set_no_create(!m_create);
    foreach (QHalPin *pin, m_pinsByName)
    {
        pb::Pin *halPin = component->add_pin();
        halPin->set_name(QString("%1.%2").arg(m_name).arg(pin->name()).toStdString());  // pin name is always component.name
        halPin->set_type((pb::ValueType)pin->type());
        halPin->set_dir((pb::HalPinDirection)pin->direction());
        if (pin->type() == QHalPin::Float)
        {
            halPin->set_halfloat(pin->value().toDouble());
        }
        else if (pin->type() == QHalPin::Bit)
        {
            halPin->set_halbit(pin->value().toBool());
        }
        else if (pin->type() == QHalPin::S32)
        {
            halPin->set_hals32(pin->value().toInt());
        }
        else if (pin->type() == QHalPin::U32)
        {
            halPin->set_halu32(pin->value().toUInt());
        }
    }

#ifdef QT_DEBUG
    std::string s;
    gpb::TextFormat::PrintToString(m_tx, &s);
    DEBUG_TAG(1, m_name, "bind")
    DEBUG_TAG(3, m_name, QString::fromStdString(s))
#endif

    sendHalrcmdMessage(pb::MT_HALRCOMP_BIND);
}

void QHalRemoteComponent::subscribe()
{
    m_halrcompSocketState = Trying;
    m_halrcompSocket->subscribeTo(m_name.toLocal8Bit());
}

void QHalRemoteComponent::unsubscribe()
{
    m_halrcompSocketState = Down;
    m_halrcompSocket->unsubscribeFrom(m_name.toLocal8Bit());
}

/** Updates a local pin with the value of a remote pin */
void QHalRemoteComponent::pinUpdate(const pb::Pin &remotePin, QHalPin *localPin)
{
#ifdef QT_DEBUG
    DEBUG_TAG(2, m_name,  "pin update" << localPin->name() << remotePin.halfloat() << remotePin.halbit() << remotePin.hals32() << remotePin.halu32())
#endif

    if (remotePin.has_halfloat())
    {
        localPin->setValue(QVariant(remotePin.halfloat()), true);
    }
    else if (remotePin.has_halbit())
    {
        localPin->setValue(QVariant(remotePin.halbit()), true);
    }
    else if (remotePin.has_hals32())
    {
        localPin->setValue(QVariant(remotePin.hals32()), true);
    }
    else if (remotePin.has_halu32())
    {
        localPin->setValue(QVariant(remotePin.halu32()), true);
    }
}

/** Adds a local pin based on remote pin representation **/
QHalPin *QHalRemoteComponent::addLocalPin(const pb::Pin &remotePin)
{
    QString name = QString::fromStdString(remotePin.name());
    name = splitPinFromHalName(name);
    QHalPin *localPin = new QHalPin(this);
    localPin->setName(name);
    localPin->setType(static_cast<QHalPin::HalPinType>(remotePin.type()));
    localPin->setDirection(static_cast<QHalPin::HalPinDirection>(remotePin.dir()));
    m_pinsByName[name] = localPin;
    m_pins.append(localPin);

    return localPin;
}

/** Updates a remote pin witht the value of a local pin */
void QHalRemoteComponent::pinChange(QVariant value)
{
    Q_UNUSED(value)
    QHalPin *pin;
    pb::Pin *halPin;

    if (m_connectionState != Connected) // only accept pin changes if we are connected
    {
        return;
    }

    pin = static_cast<QHalPin *>(QObject::sender());

    if (pin->direction() == QHalPin::In)   // Only update Output or IO pins
    {
        return;
    }

#ifdef QT_DEBUG
    DEBUG_TAG(2, m_name,  "pin change" << pin->name() << pin->value())
#endif

    // This message MUST carry a Pin message for each pin which has
    // changed value since the last message of this type.
    // Each Pin message MUST carry the handle field.
    // Each Pin message MAY carry the name field.
    // Each Pin message MUST carry the type field
    // Each Pin message MUST - depending on pin type - carry a halbit,
    // halfloat, hals32, or halu32 field.
    halPin = m_tx.add_pin();

    halPin->set_handle(pin->handle());
    halPin->set_type((pb::ValueType)pin->type());
    if (pin->type() == QHalPin::Float)
    {
        halPin->set_halfloat(pin->value().toDouble());
    }
    else if (pin->type() == QHalPin::Bit)
    {
        halPin->set_halbit(pin->value().toBool());
    }
    else if (pin->type() == QHalPin::S32)
    {
        halPin->set_hals32(pin->value().toInt());
    }
    else if (pin->type() == QHalPin::U32)
    {
        halPin->set_halu32(pin->value().toUInt());
    }

    sendHalrcmdMessage(pb::MT_HALRCOMP_SET);
}

QQmlListProperty<QHalPin> QHalRemoteComponent::pins()
{
    return QQmlListProperty<QHalPin>(this, m_pins);
}

int QHalRemoteComponent::pinCount() const
{
    return m_pins.count();
}

QHalPin *QHalRemoteComponent::pin(int index) const
{
    return m_pins.at(index);
}

void QHalRemoteComponent::start()
{
#ifdef QT_DEBUG
   DEBUG_TAG(1, m_name, "start")
#endif
    m_halrcmdSocketState = Trying;
    updateState(Connecting);

    if (connectSockets())
    {
        addPins();
        startHalrcmdHeartbeat();
        sendHalrcmdMessage(pb::MT_PING);
    }
}

void QHalRemoteComponent::stop()
{
#ifdef QT_DEBUG
    DEBUG_TAG(1, m_name, "stop")
#endif

    cleanup();

    updateState(Disconnected);  // clears also the error
}

void QHalRemoteComponent::cleanup()
{
    if (m_connected)
    {
        unsubscribe();
    }
    stopHalrcmdHeartbeat();
    disconnectSockets();
    removePins();
}

void QHalRemoteComponent::startHalrcmdHeartbeat()
{
    m_halrcmdPingOutstanding = false;

    if (m_heartbeatPeriod > 0)
    {
        m_halrcmdHeartbeatTimer->setInterval(m_heartbeatPeriod);
        m_halrcmdHeartbeatTimer->start();
    }
}

void QHalRemoteComponent::stopHalrcmdHeartbeat()
{
    m_halrcmdHeartbeatTimer->stop();
}

void QHalRemoteComponent::startHalrcompHeartbeat(int interval)
{
    m_halrcompHeartbeatTimer->stop();

    if (interval > 0)
    {
        m_halrcompHeartbeatTimer->setInterval(interval);
        m_halrcompHeartbeatTimer->start();
    }
}

void QHalRemoteComponent::stopHalrcompHeartbeat()
{
    m_halrcompHeartbeatTimer->stop();
}

void QHalRemoteComponent::refreshHalrcompHeartbeat()
{
    if (m_halrcompHeartbeatTimer->isActive())
    {
        m_halrcompHeartbeatTimer->stop();
        m_halrcompHeartbeatTimer->start();
    }
}

void QHalRemoteComponent::updateState(QHalRemoteComponent::State state)
{
    updateState(state, NoError, "");
}

void QHalRemoteComponent::updateState(State state, QHalRemoteComponent::ConnectionError error, QString errorString)
{
    if (state != m_connectionState)
    {
        if (m_connectionState == Connected) // we are not connected anymore
        {
            unsyncPins();
        }

        m_connectionState = state;
        emit connectionStateChanged(m_connectionState);

        if (m_connectionState == Connected)
        {
            if (m_connected != true) {
                m_connected = true;
                emit connectedChanged(true);
            }
        }
        else
        {
            stopHalrcompHeartbeat();
            if (m_connected != false) {
                m_connected = false;
                emit connectedChanged(false);
            }
        }
    }

    updateError(error, errorString);
}

void QHalRemoteComponent::updateError(QHalRemoteComponent::ConnectionError error, QString errorString)
{
    if (m_errorString != errorString)
    {
        m_errorString = errorString;
        emit errorStringChanged(m_errorString);
    }

    if (m_error != error)
    {
        if (error != NoError)
        {
            cleanup();
        }
        m_error = error;
        emit errorChanged(m_error);
    }
}

void QHalRemoteComponent::pollError(int errorNum, const QString &errorMsg)
{
    QString errorString;
    errorString = QString("Error %1: ").arg(errorNum) + errorMsg;
    updateState(Error, SocketError, errorString);
}

/** Recurses through a list of objects */
QObjectList QHalRemoteComponent::recurseObjects(const QObjectList &list)
{
    QObjectList halObjects;

    foreach (QObject *object, list)
    {
        QHalPin *halPin;
        halPin = qobject_cast<QHalPin *>(object);
        if (halPin != nullptr)
        {
            halObjects.append(object);
        }

        if (object->children().size() > 0)
        {
            halObjects.append(recurseObjects(object->children()));
        }
    }

    return halObjects;
}

/** Processes all message received on the update 0MQ socket */
void QHalRemoteComponent::halrcompMessageReceived(QList<QByteArray> messageList)
{
    QByteArray topic;

    topic = messageList.at(0);
    m_rx.ParseFromArray(messageList.at(1).data(), messageList.at(1).size());

#ifdef QT_DEBUG
    std::string s;
    gpb::TextFormat::PrintToString(m_rx, &s);
    DEBUG_TAG(3, m_name, "status update" << topic << QString::fromStdString(s))
#endif

    if (m_rx.type() == pb::MT_HALRCOMP_INCREMENTAL_UPDATE) //incremental update
    {
        for (int i = 0; i < m_rx.pin_size(); ++i)
        {
            pb::Pin remotePin = m_rx.pin(i);
            QHalPin *localPin = m_pinsByHandle.value(remotePin.handle(), nullptr);
            if (localPin != nullptr) // in case we received a wrong pin handle
            {
                pinUpdate(remotePin, localPin);
            }
        }

        refreshHalrcompHeartbeat();

        return;
    }
    else if (m_rx.type() == pb::MT_HALRCOMP_FULL_UPDATE)
    {
        bool pinsAdded = false;
#ifdef QT_DEBUG
        DEBUG_TAG(1, m_name, "full update")
#endif
        for (int i = 0; i < m_rx.comp_size(); ++i)
        {
            pb::Component component = m_rx.comp(i);
            for (int j = 0; j < component.pin_size(); j++)
            {
                pb::Pin remotePin = component.pin(j);
                QString name = QString::fromStdString(remotePin.name());
                name = splitPinFromHalName(name);

                QHalPin *localPin = m_pinsByName.value(name, nullptr);
                if (localPin == nullptr)
                {
                    localPin = addLocalPin(remotePin);
                    pinsAdded = true;
                }

                localPin->setHandle(remotePin.handle());
                m_pinsByHandle.insert(remotePin.handle(), localPin);
                pinUpdate(remotePin, localPin);
            }

            if (m_halrcompSocketState != Up) // will be executed only once
            {
                m_halrcompSocketState = Up;
                updateState(Connected);
            }
        }

        if (m_rx.has_pparams())
        {
            pb::ProtocolParameters pparams = m_rx.pparams();
            startHalrcompHeartbeat(pparams.keepalive_timer() * 2);  // wait double the time of the hearbeat interval
        }

        if (pinsAdded)
        {
            emit pinsChanged(pins());
        }

        return;
    }
    else if (m_rx.type() == pb::MT_PING)
    {
        if (m_halrcompSocketState == Up)
        {
            refreshHalrcompHeartbeat();
        }
        else
        {
            updateState(Connecting);
            unsubscribe();  // clean up previous subscription
            subscribe();    // trigger a fresh subscribe -> full update
        }

        return;
    }
    else if (m_rx.type() == pb::MT_HALRCOMMAND_ERROR)
    {
        QString errorString;

        for (int i = 0; i < m_rx.note_size(); ++i)
        {
            errorString.append(QString::fromStdString(m_rx.note(i)) + "\n");
        }

        m_halrcompSocketState = Down;
        updateState(Error, CommandError, errorString);

#ifdef QT_DEBUG
        DEBUG_TAG(1, m_name, "proto error on subscribe" << errorString)
#endif

        return;
    }

#ifdef QT_DEBUG
    gpb::TextFormat::PrintToString(m_rx, &s);
    DEBUG_TAG(1, m_name, "status_update: unknown message type: " << QString::fromStdString(s))
#endif
}

/** Processes all message received on the command 0MQ socket */
void QHalRemoteComponent::halrcmdMessageReceived(QList<QByteArray> messageList)
{
    m_rx.ParseFromArray(messageList.at(0).data(), messageList.at(0).size());

#ifdef QT_DEBUG
    std::string s;
    gpb::TextFormat::PrintToString(m_rx, &s);
    DEBUG_TAG(3, m_name, "server message" << QString::fromStdString(s))
#endif

    if (m_rx.type() == pb::MT_PING_ACKNOWLEDGE)
    {
        m_halrcmdPingOutstanding = false;

        if (m_halrcmdSocketState == Trying)
        {
            updateState(Connecting);

            if (m_bind)
            {
                bindPins();
            }
            else
            {
#ifdef QT_DEBUG
                DEBUG_TAG(1, m_name,  "no bind")
#endif
                m_halrcmdSocketState = Up;
                unsubscribe();  // clear previous subscription
                subscribe();    // trigger full update
            }
        }

#ifdef QT_DEBUG
        DEBUG_TAG(2, m_name, "ping ack")
#endif

        return;
    }
    else if (m_rx.type() == pb::MT_HALRCOMP_BIND_CONFIRM)
    {
#ifdef QT_DEBUG
        DEBUG_TAG(1, m_name,  "bind confirmed")
#endif
        m_halrcmdSocketState = Up;
        unsubscribe();  // clear previous subscription
        subscribe();    // trigger full update

        return;
    }
    else if ((m_rx.type() == pb::MT_HALRCOMP_BIND_REJECT)
             || (m_rx.type() == pb::MT_HALRCOMP_SET_REJECT))
    {
        QString errorString;

        for (int i = 0; i < m_rx.note_size(); ++i)
        {
            errorString.append(QString::fromStdString(m_rx.note(i)) + "\n");
        }

        m_halrcmdSocketState = Down;

        if (m_rx.type() == pb::MT_HALRCOMP_BIND_REJECT)
        {
            updateState(Error, BindError, errorString);
        }
        else
        {
            updateState(Error, PinChangeError, errorString);
        }

#ifdef QT_DEBUG
        if (m_rx.type() == pb::MT_HALRCOMP_BIND_REJECT) {
            DEBUG_TAG(1, m_name, "bind rejected" << errorString)
        }
        else {
            DEBUG_TAG(1, m_name, "pin change rejected" << QString::fromStdString(m_rx.note(0)))
        }
#endif
        return;
    }
    else
    {
#ifdef QT_DEBUG
        DEBUG_TAG(1, m_name, "UNKNOWN server message type")
#endif
    }
}

void QHalRemoteComponent::sendHalrcmdMessage(pb::ContainerType type)
{
    if (m_halrcmdSocket == nullptr) {  // disallow sending messages when not connected
        return;
    }

    try {
        m_tx.set_type(type);
        m_halrcmdSocket->sendMessage(QByteArray(m_tx.SerializeAsString().c_str(), m_tx.ByteSize()));
        m_tx.Clear();
    }
    catch (const zmq::error_t &e) {
        QString errorString;
        errorString = QString("Error %1: ").arg(e.num()) + QString(e.what());
        updateState(Error, SocketError, errorString);
    }
}

/** splits HAL pin name from full HAL name **/
QString QHalRemoteComponent::splitPinFromHalName(const QString &name)
{
    QString newName;
    int dotIndex = name.indexOf(".");
    if (dotIndex != -1)    // strip comp prefix
    {
        newName = name.mid(dotIndex + 1);
    }
    else
    {
        newName = name;
    }
    return newName;
}

void QHalRemoteComponent::halrcmdHeartbeatTimerTick()
{
    if (m_halrcmdPingOutstanding)
    {
        m_halrcmdSocketState = Trying;
        updateState(Timeout);

#ifdef QT_DEBUG
        DEBUG_TAG(1, m_name, "halcmd timeout")
#endif
    }

    sendHalrcmdMessage(pb::MT_PING);

    m_halrcmdPingOutstanding = true;

#ifdef QT_DEBUG
    DEBUG_TAG(2, m_name, "ping")
#endif
}

void QHalRemoteComponent::halrcompHeartbeatTimerTick()
{
    m_halrcompSocketState = Down;
    updateState(Timeout);

#ifdef QT_DEBUG
    DEBUG_TAG(1, m_name, "halcmd timeout")
#endif
}

