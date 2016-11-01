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

#include "halgroup.h"
#include "debughelper.h"

#if defined(Q_OS_IOS)
namespace gpb = google_public::protobuf;
#else
namespace gpb = google::protobuf;
#endif

using namespace nzmqt;

namespace qtquickvcp {

/*! \qmlproperty bool HalGroup::connected

    This property hold wheter the HAL group is connected or not. This is the
    same as \l{connectionState} == \c{HalGroup.Connected}.
 */

HalGroup::HalGroup(QObject *parent) :
    AbstractServiceImplementation(parent),
    m_halgroupUri(""),
    m_name("default"),
    m_connected(false),
    m_halgroupSocketState(Down),
    m_connectionState(Disconnected),
    m_error(NoError),
    m_errorString(""),
    m_containerItem(this),
    m_context(nullptr),
    m_halgroupSocket(nullptr),
    m_halgroupHeartbeatTimer(new QTimer(this))
{
    connect(m_halgroupHeartbeatTimer, &QTimer::timeout,
            this, &HalGroup::halgroupHeartbeatTimerTick);
}

/** Recurses through a list of objects */
QList<HalSignal *> HalGroup::recurseObjects(const QObjectList &list) const
{
    QList<HalSignal*> halSignals;

    foreach (QObject *object, list)
    {
        HalSignal *halSignal;
        halSignal = qobject_cast<HalSignal*>(object);
        if (halSignal != nullptr)
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

void HalGroup::start()
{
#ifdef QT_DEBUG
   DEBUG_TAG(1, m_name, "start")
#endif
    updateState(Connecting);


    if (connectSockets())
    {
        addSignals();
        subscribe();
    }
}

void HalGroup::stop()
{
#ifdef QT_DEBUG
    DEBUG_TAG(1, m_name, "stop")
#endif

    cleanup();

    updateState(Disconnected);  // clears also the error
}

void HalGroup::cleanup()
{
    if (m_connected)
    {
        unsubscribe();
    }
    disconnectSockets();
    removeSignals();
}

void HalGroup::startHalgroupHeartbeat(int interval)
{
    m_halgroupHeartbeatTimer->stop();

    if (interval > 0)
    {
        m_halgroupHeartbeatTimer->setInterval(interval);
        m_halgroupHeartbeatTimer->start();
    }
}

void HalGroup::stopHalgroupHeartbeat()
{
    m_halgroupHeartbeatTimer->stop();
}

void HalGroup::refreshHalgroupHeartbeat()
{
    if (m_halgroupHeartbeatTimer->isActive())
    {
        m_halgroupHeartbeatTimer->stop();
        m_halgroupHeartbeatTimer->start();
    }
}

void HalGroup::updateState(HalGroup::State state)
{
    updateState(state, NoError, "");
}

void HalGroup::updateState(HalGroup::State state, ConnectionError error, const QString &errorString)
{
    if (state != m_connectionState)
    {
        if (m_connectionState == Connected) // we are not connected anymore
        {
            unsyncSignals();
            stopHalgroupHeartbeat();
            if (m_connected != false) {
                m_connected = false;
                emit connectedChanged(m_connected);
            }
        }
        else
        {
            if (m_connected != true) {
                m_connected = true;
                emit connectedChanged(m_connected);
            }
        }

        m_connectionState = state;
        emit connectionStateChanged(m_connectionState);
    }

    updateError(error, errorString);
}

void HalGroup::updateError(HalGroup::ConnectionError error, const QString &errorString)
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

/** Updates a local signal with the value of a remote signal */
void HalGroup::signalUpdate(const pb::Signal &remoteSignal, HalSignal *localSignal)
{
    bool updated;
#ifdef QT_DEBUG
    DEBUG_TAG(2, m_name,  "signal update" << localSignal->name() << remoteSignal.halfloat() << remoteSignal.halbit() << remoteSignal.hals32() << remoteSignal.halu32())
#endif

    updated = false;

    if (remoteSignal.type() == pb::HAL_FLOAT)
    {
        localSignal->setType(HalSignal::Float);
        localSignal->setValue(QVariant(remoteSignal.halfloat()));
        m_values[localSignal->name()] = remoteSignal.halfloat();
        updated =  true;
    }
    else if (remoteSignal.type() == pb::HAL_BIT)
    {
        localSignal->setType(HalSignal::Bit);
        localSignal->setValue(QVariant(remoteSignal.halbit()));
        m_values[localSignal->name()] = remoteSignal.halbit();
        updated =  true;
    }
    else if (remoteSignal.type() == pb::HAL_S32)
    {
        localSignal->setType(HalSignal::S32);
        localSignal->setValue(QVariant(remoteSignal.hals32()));
        m_values[localSignal->name()] = remoteSignal.hals32();
        updated =  true;
    }
    else if (remoteSignal.type() == pb::HAL_U32)
    {
        localSignal->setType(HalSignal::U32);
        localSignal->setValue(QVariant(remoteSignal.halu32()));
        m_values[localSignal->name()] = (int)remoteSignal.halu32();
        updated =  true;
    }

    if (updated)
    {
        localSignal->setSynced(true);   // when the signal is updated we are synced
        emit valuesChanged(m_values);
    }
}

void HalGroup::halgroupMessageReceived(const QList<QByteArray> &messageList)
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
            HalSignal *localSignal = m_signalsByHandle.value(remoteSignal.handle(), nullptr);
            if (localSignal != nullptr) // in case we received a wrong signal handle
            {
                signalUpdate(remoteSignal, localSignal);
            }
        }

        refreshHalgroupHeartbeat();

        return;
    }
    else if (m_rx.type() == pb::MT_HALGROUP_FULL_UPDATE) // full update
    {
        for (int i = 0; i < m_rx.group_size(); ++i)
        {
            pb::Group group = m_rx.group(i);
            for (int j = 0; j < group.member_size(); ++j)
            {
                pb::Member member = group.member(j);
                if (member.has_signal())
                {
                    pb::Signal remoteSignal = member.signal();
                    QString name = QString::fromStdString(remoteSignal.name());
                    int dotIndex = name.indexOf(".");
                    if (dotIndex != -1) // strip comp prefix
                    {
                        name = name.mid(dotIndex + 1);
                    }
                    HalSignal *localSignal = m_signalsByName.value(name, nullptr);
                    if (localSignal == nullptr)
                    {
                        localSignal = new HalSignal(this); // create a local signal
                        localSignal->setName(name);
                        m_localSignals.append(localSignal);
                        m_signalsByName.insert(name, localSignal);
                    }
                    localSignal->setHandle(remoteSignal.handle());
                    m_signalsByHandle.insert(remoteSignal.handle(), localSignal);
                    signalUpdate(remoteSignal, localSignal);
                }
            }

            if (m_halgroupSocketState != Up) // will be executed only once
            {
                m_halgroupSocketState = Up;
                updateState(Connected);
            }
        }

        if (m_rx.has_pparams())
        {
            pb::ProtocolParameters pparams = m_rx.pparams();
            startHalgroupHeartbeat(pparams.keepalive_timer() * 2); // wait double the time of the hearbeat interval
        }

        return;
    }
    else if (m_rx.type() == pb::MT_PING)
    {
        if (m_halgroupSocketState == Up)
        {
            refreshHalgroupHeartbeat();
        }
        else
        {
            updateState(Connecting);
            unsubscribe();  // clean up previous subscription
            subscribe();    // trigger a fresh subscribe -> full update
        }

        return;
    }
    else if (m_rx.type() == pb::MT_HALGROUP_ERROR) // error
    {
        QString errorString;

        for (int i = 0; i < m_rx.note_size(); ++i)
        {
            errorString.append(QString::fromStdString(m_rx.note(i)) + "\n");
        }

        m_halgroupSocketState = Down;
        updateState(Error, HalGroupError, errorString);

#ifdef QT_DEBUG
        DEBUG_TAG(1, m_name, "proto error on subscribe" << errorString)
#endif

        return;
    }

#ifdef QT_DEBUG
    gpb::TextFormat::PrintToString(m_rx, &s);
    DEBUG_TAG(1, m_name, "halgroup_update: unknown message type: " << QString::fromStdString(s))
#endif
}

void HalGroup::pollError(int errorNum, const QString &errorMsg)
{
    QString errorString;
    errorString = QString("Error %1: ").arg(errorNum) + errorMsg;
    updateState(Error, SocketError, errorString);
}

void HalGroup::halgroupHeartbeatTimerTick()
{
    m_halgroupSocketState = Down;
    updateState(Timeout);

#ifdef QT_DEBUG
    DEBUG_TAG(1, m_name, "halcmd timeout")
#endif
}

/** Scans all children of the container item for signals and adds them to a map */
void HalGroup::addSignals()
{
    QList<HalSignal*> halSignals;

    if (m_containerItem == nullptr)
    {
        return;
    }

    halSignals = recurseObjects(m_containerItem->children());
    foreach (HalSignal *signal, halSignals)
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
void HalGroup::removeSignals()
{
    m_signalsByHandle.clear();
    m_signalsByName.clear();

    qDeleteAll(m_localSignals.begin(), m_localSignals.end());
    m_localSignals.clear();

    QStringList keyList = m_values.keys();
    for (int i = 0; i < keyList.size(); ++i)
    {
        m_values.remove(keyList.at(i));
    }
    emit valuesChanged(m_values);
}

/** Sets synced of all signals to false */
void HalGroup::unsyncSignals()
{
    QMapIterator<QString, HalSignal*> i(m_signalsByName);
    while (i.hasNext()) {
        i.next();
        i.value()->setSynced(false);
    }
}

/** Connects the 0MQ sockets */
bool HalGroup::connectSockets()
{
    m_context = new PollingZMQContext(this, 1);
    connect(m_context, &PollingZMQContext::pollError,
            this, &HalGroup::pollError);
    m_context->start();

    m_halgroupSocket = m_context->createSocket(ZMQSocket::TYP_SUB, this);
    m_halgroupSocket->setLinger(0);

    try {
        m_halgroupSocket->connectTo(m_halgroupUri);
    }
    catch (const zmq::error_t &e) {
        QString errorString;
        errorString = QString("Error %1: ").arg(e.num()) + QString(e.what());
        updateState(Error, SocketError, errorString);
        return false;
    }

    connect(m_halgroupSocket, &ZMQSocket::messageReceived,
            this, &HalGroup::halgroupMessageReceived);

#ifdef QT_DEBUG
    DEBUG_TAG(1, m_name, "socket connected" << m_halgroupUri)
#endif

    return true;
}

/** Disconnects the 0MQ sockets */
void HalGroup::disconnectSockets()
{
    m_halgroupSocketState = Down;

    if (m_halgroupSocket != nullptr)
    {
        m_halgroupSocket->close();
        m_halgroupSocket->deleteLater();
        m_halgroupSocket = nullptr;
    }

    if (m_context != nullptr)
    {
        m_context->stop();
        m_context->deleteLater();
        m_context = nullptr;
    }
}

void HalGroup::subscribe()
{
    m_halgroupSocketState = Trying;
    m_halgroupSocket->subscribeTo(m_name.toLocal8Bit());
}

void HalGroup::unsubscribe()
{
    m_halgroupSocketState = Down;
    m_halgroupSocket->unsubscribeFrom(m_name.toLocal8Bit());
}
}; // namespace qtquickvcp
