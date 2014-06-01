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

/*!
    \qmltype HalRemoteComponent
    \instantiates QHalRemoteComponent
    \inqmlmodule Machinekit.HalRemote
    \brief A HAL remote component.
    \ingroup halremote


*/
/** Remote HAL Component implementation for use with C++ and QML */
QHalRemoteComponent::QHalRemoteComponent(QQuickItem *parent) :
    QQuickItem(parent),
    m_cmdUri(""),
    m_updateUri(""),
    m_name("default"),
    m_heartbeatPeriod(3000),
    m_synced(false),
    m_sState(Down),
    m_cState(Down),
    m_connectionState(Disconnected),
    m_error(NoError),
    m_errorString(""),
    m_ready(false),
    m_containerItem(NULL),
    m_componentCompleted(false),
    m_context(NULL),
    m_updateSocket(NULL),
    m_cmdSocket(NULL),
    m_heartbeatTimer(new QTimer(this))
{
    connect(m_heartbeatTimer, SIGNAL(timeout()),
            this, SLOT(hearbeatTimerTick()));
}

QHalRemoteComponent::~QHalRemoteComponent()
{
    disconnectSockets();
}

/** componentComplete is executed when the QML component is fully loaded */
void QHalRemoteComponent::componentComplete()
{
    m_componentCompleted = true;

    if (m_ready == true)    // the component was set to ready before it was completed
    {
        start();
    }

    QQuickItem::componentComplete();
}

/** Scans all children of the container item for pins and adds them to a map */
void QHalRemoteComponent::addPins()
{
    QObjectList halObjects;

    if (m_containerItem == NULL)
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
        connect(pin, SIGNAL(valueChanged(QVariant)),
                this, SLOT(pinChange(QVariant)));
#ifdef QT_DEBUG
        qDebug() << "pin added: " << pin->name();
#endif
    }
}

/** Removes all previously added pins */
void QHalRemoteComponent::removePins()
{
    foreach (QHalPin *pin, m_pinsByName)
    {
        disconnect(pin, SIGNAL(valueChanged(QVariant)),
                this, SLOT(pinChange(QVariant)));
    }

    m_pinsByHandle.clear();
    m_pinsByName.clear();
}

/** Connects the 0MQ sockets */
bool QHalRemoteComponent::connectSockets()
{
    m_context = createDefaultContext(this);
    m_context->start();

    m_cmdSocket = m_context->createSocket(ZMQSocket::TYP_DEALER, this);
    m_cmdSocket->setLinger(0);
    m_cmdSocket->setIdentity(QString("%1-%2").arg(m_name).arg(QCoreApplication::applicationPid()).toLocal8Bit());


    m_updateSocket = m_context->createSocket(ZMQSocket::TYP_SUB, this);
    m_updateSocket->setLinger(0);

    try {
        m_cmdSocket->connectTo(m_cmdUri);
        m_updateSocket->connectTo(m_updateUri);
    }
    catch (zmq::error_t e) {
        QString errorString;
        errorString = QString("Error %1: ").arg(e.num()) + QString(e.what());
        updateError(SocketError, errorString);
        updateState(Error);
        return false;
    }

    connect(m_updateSocket, SIGNAL(messageReceived(QList<QByteArray>)),
            this, SLOT(updateMessageReceived(QList<QByteArray>)));
    connect(m_cmdSocket, SIGNAL(messageReceived(QList<QByteArray>)),
            this, SLOT(cmdMessageReceived(QList<QByteArray>)));

#ifdef QT_DEBUG
    qDebug() << "sockets connected" << m_updateUri << m_cmdUri;
#endif

    return true;
}

/** Disconnects the 0MQ sockets */
void QHalRemoteComponent::disconnectSockets()
{
    if (m_cmdSocket != NULL)
    {
        m_cmdSocket->close();
        m_cmdSocket->deleteLater();
        m_cmdSocket = NULL;
    }

    if (m_updateSocket != NULL)
    {
        m_updateSocket->close();
        m_updateSocket->deleteLater();
        m_updateSocket = NULL;
    }

    if (m_context != NULL)
    {
        m_context->stop();
        m_context->deleteLater();
        m_context = NULL;
    }
}

/** Generates a Bind messages and sends it over the suitable 0MQ socket */
void QHalRemoteComponent::bind()
{
    pb::Component *component;

    m_tx.set_type(pb::MT_HALRCOMP_BIND);
    component = m_tx.add_comp();
    component->set_name(m_name.toStdString());
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
    qDebug() << "bind:" << QString::fromStdString(s);
#endif

    m_cmdSocket->sendMessage(QByteArray(m_tx.SerializeAsString().c_str(), m_tx.ByteSize()));
    m_tx.Clear();
}

/** Updates a local pin with the value of a remote pin */
void QHalRemoteComponent::pinUpdate(pb::Pin remotePin, QHalPin *localPin)
{
#ifdef QT_DEBUG
    qDebug() << "pinupdate" << localPin->name();
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

/** Updates a remote pin witht the value of a local pin */
void QHalRemoteComponent::pinChange(QVariant value)
{
    Q_UNUSED(value)
    QHalPin *pin;
    pb::Pin *halPin;

#ifdef QT_DEBUG
    qDebug() << "pinchange" << m_synced;
#endif

    if (!m_synced)
    {
        return;
    }

    pin = static_cast<QHalPin *>(QObject::sender());

    if (pin->direction() == QHalPin::In)   // Only update IN or IO pins
    {
        return;
    }

    m_tx.set_type(pb::MT_HALRCOMP_SET);

    // This message MUST carry a Pin message for each pin which has
    // changed value since the last message of this type.
    // Each Pin message MUST carry the handle field.
    // Each Pin message MAY carry the name field.
    // Each Pin message MUST - depending on pin type - carry a halbit,
    // halfloat, hals32, or halu32 field.
    halPin = m_tx.add_pin();

    halPin->set_handle(pin->handle());
    halPin->set_name(QString("%1.%2").arg(m_name).arg(pin->name()).toStdString());
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

    m_cmdSocket->sendMessage(QByteArray(m_tx.SerializeAsString().c_str(), m_tx.ByteSize()));
    m_tx.Clear();
}

void QHalRemoteComponent::start()
{
#ifdef QT_DEBUG
            qDebug() << "ready" << m_name;
#endif
    m_cState = Trying;
    updateState(Connecting);


    if (connectSockets())
    {
        addPins();
        bind();
    }
}

void QHalRemoteComponent::stop()
{
#ifdef QT_DEBUG
            qDebug() << "stop" << m_name;
#endif

    // cleanup here
    stopHeartbeat();
    disconnectSockets();
    removePins();

    updateState(Disconnected);
    updateError(NoError, "");   // clear the error here
}

void QHalRemoteComponent::startHeartbeat()
{
    m_heartbeatTimer->setInterval(m_heartbeatPeriod);
    m_heartbeatTimer->start();
}

void QHalRemoteComponent::stopHeartbeat()
{
    m_heartbeatTimer->stop();
}

void QHalRemoteComponent::updateState(State state)
{
    if (state != m_connectionState)
    {
        m_connectionState = state;
        emit connectionStateChanged(m_connectionState);

        if (m_connectionState == Connected)
        {
            startHeartbeat();
        }
        else
        {
            stopHeartbeat();
        }
    }
}

void QHalRemoteComponent::updateError(QHalRemoteComponent::ConnectionError error, QString errorString)
{
    m_error = error;
    m_errorString = errorString;

    emit errorStringChanged(m_errorString);
    emit errorChanged(m_error);
}

/** If the ready property has a rising edge we try to connect
 *  if it is has a falling edge we disconnect and cleanup
 */
void QHalRemoteComponent::setReady(bool arg)
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

/** Recurses through a list of objects */
QObjectList QHalRemoteComponent::recurseObjects(const QObjectList &list)
{
    QObjectList halObjects;

    foreach (QObject *object, list)
    {
        if (object->inherits("QHalPin"))
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
void QHalRemoteComponent::updateMessageReceived(QList<QByteArray> messageList)
{
    QByteArray topic;

    topic = messageList.at(0);
    m_rx.ParseFromArray(messageList.at(1).data(), messageList.at(1).size());

#ifdef QT_DEBUG
    qDebug() << "status update" << topic << QString::fromStdString(m_rx.SerializeAsString());
#endif

    if (m_rx.type() == pb::MT_HALRCOMP_INCREMENTAL_UPDATE) //incremental update
    {
        for (int i = 0; i < m_rx.pin_size(); ++i)
        {
            pb::Pin remotePin = m_rx.pin(i);
            QHalPin *localPin = m_pinsByHandle[remotePin.handle()];
            pinUpdate(remotePin, localPin);
        }

        if (m_sState != Up)
        {
            m_sState = Up;
            updateError(NoError, "");
            updateState(Connected);
        }

        return;
    }
    else if (m_rx.type() == pb::MT_HALRCOMP_FULL_UPDATE)
    {
        for (int i = 0; i < m_rx.comp_size(); ++i)
        {
            pb::Component component = m_rx.comp(i);
            for (int j = 0; j < component.pin_size(); j++)
            {
                pb::Pin remotePin = component.pin(j);
                QString name = QString::fromStdString(remotePin.name());
                if (name.indexOf(".") != -1)    // strip comp prefix
                {
                    name = name.mid(name.indexOf(".")+1);
                }
                QHalPin *localPin = m_pinsByName[name];
                localPin->setHandle(remotePin.handle());
                m_pinsByHandle[remotePin.handle()] = localPin;
                pinUpdate(remotePin, localPin);
            }
            m_synced = true;
            emit syncedChanged(m_synced);

            if (m_sState != Up)
            {
                m_sState = Up;
                updateError(NoError, "");
                updateState(Connected);
            }
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

        m_sState = Down;
        updateError(CommandError, errorString);
        updateState(Error);

#ifdef QT_DEBUG
        qDebug() << "proto error on subscribe" << errorString;
#endif

        return;
    }

#ifdef QT_DEBUG
        qDebug() << "status_update: unknown message type: " << QString::fromStdString(m_rx.SerializeAsString());
#endif
}

/** Processes all message received on the command 0MQ socket */
void QHalRemoteComponent::cmdMessageReceived(QList<QByteArray> messageList)
{
    m_rx.ParseFromArray(messageList.at(0).data(), messageList.at(0).size());

#ifdef QT_DEBUG
    std::string s;
    gpb::TextFormat::PrintToString(m_rx, &s);
    qDebug() << "server message" << QString::fromStdString(s);
#endif

    if (m_rx.type() == pb::MT_PING_ACKNOWLEDGE)
    {
        m_cState = Up;
        m_pingOutstanding = false;

        if (m_sState == Up)
        {
            updateError(NoError, "");
            updateState(Connected);
        }

        return;
    }
    else if (m_rx.type() == pb::MT_HALRCOMP_BIND_CONFIRM)
    {
#ifdef QT_DEBUG
        qDebug() << "bind confirmed";
#endif
        m_cState = Up;
        m_sState = Trying;
        m_updateSocket->subscribeTo(m_name.toLocal8Bit());

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

        m_cState = Down;

        if (m_rx.type() == pb::MT_HALRCOMP_BIND_REJECT)
        {
            updateError(BindError, errorString);
        }
        else
        {
            updateError(PinChangeError, errorString);
        }
        updateState(Error);


#ifdef QT_DEBUG
        if (m_rx.type() == pb::MT_HALRCOMP_BIND_REJECT) {
            qDebug() << "bind rejected" << errorString;
        }
        else {
            qDebug() << "pin change rejected" << QString::fromStdString(m_rx.note(0));
        }
#endif
        return;
    }
    else
    {
#ifdef QT_DEBUG
        qDebug() << "UNKNOWN server message type";
#endif
    }
}

void QHalRemoteComponent::hearbeatTimerTick()
{
    if (m_pingOutstanding)
    {
        m_cState = Trying;
        updateError(TimeoutError, "Remote host timed out");
        updateState(Error);

#ifdef QT_DEBUG
        qDebug() << "timeout";
#endif
    }

    m_tx.set_type(pb::MT_PING);

    m_cmdSocket->sendMessage(QByteArray(m_tx.SerializeAsString().c_str(), m_tx.ByteSize()));
    m_tx.Clear();

    m_pingOutstanding = true;
}

