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

#include "qapplicationstatus.h"
#include "debughelper.h"

QApplicationStatus::QApplicationStatus(QObject *parent) :
    AbstractServiceImplementation(parent),
    m_statusUri(""),
    m_statusSocketState(Down),
    m_connected(false),
    m_connectionState(Disconnected),
    m_error(NoError),
    m_errorString(""),
    m_running(false),
    m_synced(false),
    m_channels(MotionChannel | ConfigChannel | IoChannel | TaskChannel | InterpChannel),
    m_context(NULL),
    m_statusSocket(NULL),
    m_statusHeartbeatTimer(new QTimer(this))
{
    connect(m_statusHeartbeatTimer, SIGNAL(timeout()),
            this, SLOT(statusHeartbeatTimerTick()));

    connect(this, SIGNAL(taskChanged(QJsonObject)),
            this, SLOT(updateRunning(QJsonObject)));
    connect(this, SIGNAL(interpChanged(QJsonObject)),
            this, SLOT(updateRunning(QJsonObject)));


    initializeObject(MotionChannel);
    initializeObject(ConfigChannel);
    initializeObject(IoChannel);
    initializeObject(TaskChannel);
    initializeObject(InterpChannel);
}

void QApplicationStatus::start()
{
#ifdef QT_DEBUG
   DEBUG_TAG(1, "status", "start")
#endif
    updateState(Connecting);

    if (connectSockets())
    {
        subscribe();
    }
}

void QApplicationStatus::stop()
{
#ifdef QT_DEBUG
    DEBUG_TAG(1, "status", "stop")
#endif

    cleanup();
    updateState(Disconnected);  // clears also the error
}

void QApplicationStatus::cleanup()
{
    if (m_connected)
    {
        unsubscribe();
    }
    disconnectSockets();
    m_subscriptions.clear();
}

void QApplicationStatus::startStatusHeartbeat(int interval)
{
    m_statusHeartbeatTimer->stop();

    if (interval > 0)
    {
        m_statusHeartbeatTimer->setInterval(interval);
        m_statusHeartbeatTimer->start();
    }
}

void QApplicationStatus::stopStatusHeartbeat()
{
    m_statusHeartbeatTimer->stop();
}

void QApplicationStatus::refreshStatusHeartbeat()
{
    if (m_statusHeartbeatTimer->isActive())
    {
        m_statusHeartbeatTimer->stop();
        m_statusHeartbeatTimer->start();
    }
}

void QApplicationStatus::updateState(QApplicationStatus::State state)
{
    updateState(state, NoError, "");
}

void QApplicationStatus::updateState(QApplicationStatus::State state, QApplicationStatus::ConnectionError error, const QString &errorString)
{
    updateError(error, errorString);

    if (state != m_connectionState)
    {
        if (m_connected) // we are not connected anymore
        {
            stopStatusHeartbeat();
            clearSync();
            m_connected = false;
            emit connectedChanged(false);
        }
        else if (state == Connected) {
            m_connected = true;
            emit connectedChanged(true);
        }

        m_connectionState = state;
        emit connectionStateChanged(m_connectionState);

        if ((state == Disconnected) || (state == Error)) {
            initializeObject(MotionChannel);
            initializeObject(ConfigChannel);
            initializeObject(IoChannel);
            initializeObject(TaskChannel);
            initializeObject(InterpChannel);
        }
    }
}

void QApplicationStatus::updateError(QApplicationStatus::ConnectionError error, const QString &errorString)
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

void QApplicationStatus::updateSync(QApplicationStatus::StatusChannel channel)
{
    m_syncedChannels |= channel;

    if (m_syncedChannels == m_channels) {
        m_synced = true;
        emit syncedChanged(m_synced);
    }
}

void QApplicationStatus::clearSync()
{
    m_synced = false;
    m_syncedChannels = 0;
    emit syncedChanged(m_synced);
}

void QApplicationStatus::updateMotion(const pb::EmcStatusMotion &motion)
{
    Service::recurseMessage(motion, &m_motion);
    emit motionChanged(m_motion);
}

void QApplicationStatus::updateConfig(const pb::EmcStatusConfig &config)
{
    Service::recurseMessage(config, &m_config);
    emit configChanged(m_config);
}

void QApplicationStatus::updateIo(const pb::EmcStatusIo &io)
{
    Service::recurseMessage(io, &m_io);
    emit ioChanged(m_io);
}

void QApplicationStatus::updateTask(const pb::EmcStatusTask &task)
{
    Service::recurseMessage(task, &m_task);
    emit taskChanged(m_task);
}

void QApplicationStatus::updateInterp(const pb::EmcStatusInterp &interp)
{
    Service::recurseMessage(interp, &m_interp);
    emit interpChanged(m_interp);
}

void QApplicationStatus::statusMessageReceived(const QList<QByteArray> &messageList)
{
    QByteArray topic;

    topic = messageList.at(0);
    m_rx.ParseFromArray(messageList.at(1).data(), messageList.at(1).size());

#ifdef QT_DEBUG
    std::string s;
    gpb::TextFormat::PrintToString(m_rx, &s);
    DEBUG_TAG(3, "status", "update" << topic << QString::fromStdString(s))
#endif

    if ((m_rx.type() == pb::MT_EMCSTAT_FULL_UPDATE)
        || (m_rx.type() == pb::MT_EMCSTAT_INCREMENTAL_UPDATE))
    {
        if ((topic == "motion") && m_rx.has_emc_status_motion()) {
            updateMotion(m_rx.emc_status_motion());
            if (m_rx.type() == pb::MT_EMCSTAT_FULL_UPDATE) {
                updateSync(MotionChannel);
            }
        }

        if ((topic == "config") && m_rx.has_emc_status_config()) {
            updateConfig(m_rx.emc_status_config());
            if (m_rx.type() == pb::MT_EMCSTAT_FULL_UPDATE) {
                updateSync(ConfigChannel);
            }
        }

        if ((topic == "io") && m_rx.has_emc_status_io()) {
            updateIo(m_rx.emc_status_io());
            if (m_rx.type() == pb::MT_EMCSTAT_FULL_UPDATE) {
                updateSync(IoChannel);
            }
        }

        if ((topic == "task") && m_rx.has_emc_status_task()) {
            updateTask(m_rx.emc_status_task());
            if (m_rx.type() == pb::MT_EMCSTAT_FULL_UPDATE) {
                updateSync(TaskChannel);
            }
        }

        if ((topic == "interp") && m_rx.has_emc_status_interp()) {
            updateInterp(m_rx.emc_status_interp());
            if (m_rx.type() == pb::MT_EMCSTAT_FULL_UPDATE) {
                updateSync(InterpChannel);
            }
        }

        if (m_rx.type() == pb::MT_EMCSTAT_FULL_UPDATE)
        {
            if (m_statusSocketState != Up)
            {
                m_statusSocketState = Up;
                updateState(Connected);
            }

            if (m_rx.has_pparams())
            {
                pb::ProtocolParameters pparams = m_rx.pparams();
                startStatusHeartbeat(pparams.keepalive_timer() * 2); // wait double the time of the hearbeat interval
            }
        }
        else
        {
            refreshStatusHeartbeat();
        }

        return;
    }
    else if (m_rx.type() == pb::MT_PING)
    {
        if (m_statusSocketState == Up)
        {
            refreshStatusHeartbeat();
        }
        else
        {
            updateState(Connecting);
            unsubscribe();  // clean up previous subscription
            subscribe();    // trigger a fresh subscribe
        }

        return;
    }

#ifdef QT_DEBUG
    gpb::TextFormat::PrintToString(m_rx, &s);
    DEBUG_TAG(1, "status", "update: unknown message type: " << QString::fromStdString(s))
#endif
}

void QApplicationStatus::pollError(int errorNum, const QString &errorMsg)
{
    QString errorString;
    errorString = QString("Error %1: ").arg(errorNum) + errorMsg;
    updateState(Error, SocketError, errorString);
}

void QApplicationStatus::statusHeartbeatTimerTick()
{
    m_statusSocketState = Down;
    updateState(Timeout);

#ifdef QT_DEBUG
    DEBUG_TAG(1, "status", "timeout")
#endif
}

/** Connects the 0MQ sockets */
bool QApplicationStatus::connectSockets()
{
    m_context = new PollingZMQContext(this, 1);
    connect(m_context, SIGNAL(pollError(int,QString)),
            this, SLOT(pollError(int,QString)));
    m_context->start();

    m_statusSocket = m_context->createSocket(ZMQSocket::TYP_SUB, this);
    m_statusSocket->setLinger(0);

    try {
        m_statusSocket->connectTo(m_statusUri);
    }
    catch (const zmq::error_t &e) {
        QString errorString;
        errorString = QString("Error %1: ").arg(e.num()) + QString(e.what());
        updateState(Error, SocketError, errorString);
        return false;
    }

    connect(m_statusSocket, SIGNAL(messageReceived(QList<QByteArray>)),
            this, SLOT(statusMessageReceived(QList<QByteArray>)));

#ifdef QT_DEBUG
    DEBUG_TAG(1, "status", "socket connected" << m_statusUri)
#endif

    return true;
}

/** Disconnects the 0MQ sockets */
void QApplicationStatus::disconnectSockets()
{
    m_statusSocketState = Down;

    if (m_statusSocket != NULL)
    {
        m_statusSocket->close();
        m_statusSocket->deleteLater();
        m_statusSocket = NULL;
    }

    if (m_context != NULL)
    {
        m_context->stop();
        m_context->deleteLater();
        m_context = NULL;
    }
}

void QApplicationStatus::subscribe()
{
    m_statusSocketState = Trying;

    if (m_channels | MotionChannel) {
        m_statusSocket->subscribeTo("motion");
        m_subscriptions.append("motion");
    }
    if (m_channels | ConfigChannel) {
        m_statusSocket->subscribeTo("config");
        m_subscriptions.append("config");
    }
    if (m_channels | TaskChannel) {
        m_statusSocket->subscribeTo("task");
        m_subscriptions.append("task");
    }
    if (m_channels | IoChannel) {
        m_statusSocket->subscribeTo("io");
        m_subscriptions.append("io");
    }
    if (m_channels | InterpChannel) {
        m_statusSocket->subscribeTo("interp");
        m_subscriptions.append("interp");
    }
}

void QApplicationStatus::unsubscribe()
{
    m_statusSocketState = Down;

    foreach (QString subscription, m_subscriptions)
    {
        m_statusSocket->unsubscribeFrom(subscription);

        if (subscription == "motion") {
            initializeObject(MotionChannel);
        }
        else if (subscription == "config") {
            initializeObject(ConfigChannel);
        }
        else if (subscription == "io") {
            initializeObject(IoChannel);
        }
        else if (subscription == "task") {
            initializeObject(TaskChannel);
        }
        else if (subscription == "interp") {
            initializeObject(InterpChannel);
        }
    }
    m_subscriptions.clear();
}

void QApplicationStatus::updateRunning(const QJsonObject &object)
{
    Q_UNUSED(object)

    bool running;

    running = m_task["taskMode"].isDouble() && m_interp["interpState"].isDouble()
            && ((static_cast<int>(m_task["taskMode"].toDouble()) == TaskModeAuto)
                || (static_cast<int>(m_task["taskMode"].toDouble()) == TaskModeMdi))
            && (static_cast<int>(m_interp["interpState"].toDouble()) != InterpreterIdle);

    if (running != m_running)
    {
        m_running = running;
        emit runningChanged(running);
    }
}

void QApplicationStatus::initializeObject(QApplicationStatus::StatusChannel channel)
{
    switch (channel)
    {
    case MotionChannel:
        m_motion = QJsonObject();
        Service::recurseDescriptor(statusMotion.GetDescriptor(), &m_motion);
        emit motionChanged(m_motion);
        return;
    case ConfigChannel:
        m_config = QJsonObject();
        Service::recurseDescriptor(statusConfig.GetDescriptor(), &m_config);
        emit configChanged(m_config);
        return;
    case IoChannel:
        m_io = QJsonObject();
        Service::recurseDescriptor(statusIo.GetDescriptor(), &m_io);
        emit ioChanged(m_io);
        return;
    case TaskChannel:
        m_task = QJsonObject();
        Service::recurseDescriptor(statusTask.GetDescriptor(), &m_task);
        emit taskChanged(m_task);
        return;
    case InterpChannel:
        m_interp = QJsonObject();
        Service::recurseDescriptor(statusInterp.GetDescriptor(), &m_interp);
        emit interpChanged(m_interp);
        return;
    }
}
