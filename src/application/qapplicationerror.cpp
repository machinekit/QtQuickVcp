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

#include "qapplicationerror.h"
#include "debughelper.h"

QApplicationError::QApplicationError(QObject *parent) :
    AbstractServiceImplementation(parent),
    m_errorUri(""),
    m_connected(false),
    m_errorSocketState(Down),
    m_connectionState(Disconnected),
    m_error(NoError),
    m_errorString(""),
    m_channels(ErrorChannel | TextChannel | DisplayChannel),
    m_context(nullptr),
    m_errorSocket(nullptr),
    m_errorHeartbeatTimer(new QTimer(this))
{
   connect(m_errorHeartbeatTimer, SIGNAL(timeout()),
           this, SLOT(errorHeartbeatTimerTick()));
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

    cleanup();
    updateState(Disconnected);  // clears also the error
}

void QApplicationError::cleanup()
{
    if (m_connected)
    {
        unsubscribe();
    }
    disconnectSockets();
    m_subscriptions.clear();
}

void QApplicationError::startErrorHeartbeat(int interval)
{
    m_errorHeartbeatTimer->stop();

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
    updateState(state, NoError, "");
}

void QApplicationError::updateState(QApplicationError::State state, QApplicationError::ConnectionError error, const QString &errorString)
{
    if (state != m_connectionState)
    {
        if (m_connectionState == Connected) // we are not connected anymore
        {
            stopErrorHeartbeat();
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
        else if (m_connected != false) {
            m_connected = false;
            emit connectedChanged(false);
        }
    }

    updateError(error, errorString);
}

void QApplicationError::updateError(QApplicationError::ConnectionError error, const QString &errorString)
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
        || (m_rx.type() == pb::MT_EMC_OPERATOR_DISPLAY))
    {
        for (int i = 0; i < m_rx.note_size(); ++i)
        {
            messageReceived((ErrorType)m_rx.type(), QString::fromStdString(m_rx.note(i)));
        }

        refreshErrorHeartbeat();

        return;
    }
    else if (m_rx.type() == pb::MT_PING)
    {
        if (m_errorSocketState == Up)
        {
            refreshErrorHeartbeat();
        }
        else
        {
            if (m_connectionState == Timeout) // waiting for the ping
            {
                updateState(Connecting);
                unsubscribe();  // clean up previous subscription
                subscribe();    // trigger a fresh subscribe
            }
            else    // ping as result from subscription received
            {
                m_errorSocketState = Up;
                updateState(Connected);
            }
        }

        if (m_rx.has_pparams())
        {
            pb::ProtocolParameters pparams = m_rx.pparams();
            startErrorHeartbeat(pparams.keepalive_timer() * 2); // wait double the time of the hearbeat interval
        }

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
    updateState(Error, SocketError, errorString);
}

void QApplicationError::errorHeartbeatTimerTick()
{
    m_errorSocketState = Down;
    updateState(Timeout);

#ifdef QT_DEBUG
    DEBUG_TAG(1, "error", "timeout")
#endif
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
    catch (const zmq::error_t &e) {
        QString errorString;
        errorString = QString("Error %1: ").arg(e.num()) + QString(e.what());
        updateState(Error, SocketError, errorString);
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
    m_errorSocketState = Down;

    if (m_errorSocket != nullptr)
    {
        m_errorSocket->close();
        m_errorSocket->deleteLater();
        m_errorSocket = nullptr;
    }

    if (m_context != nullptr)
    {
        m_context->stop();
        m_context->deleteLater();
        m_context = nullptr;
    }
}

void QApplicationError::subscribe()
{
    m_errorSocketState = Trying;

    if (m_channels & ErrorChannel) {
        m_errorSocket->subscribeTo("error");
        m_subscriptions.append("error");
    }
    if (m_channels & TextChannel) {
        m_errorSocket->subscribeTo("text");
        m_subscriptions.append("text");
    }
    if (m_channels & DisplayChannel) {
        m_errorSocket->subscribeTo("display");
        m_subscriptions.append("display");
    }
}

void QApplicationError::unsubscribe()
{
    m_errorSocketState = Down;
    foreach (QString subscription, m_subscriptions)
    {
        m_errorSocket->unsubscribeFrom(subscription);
    }
    m_subscriptions.clear();
}
