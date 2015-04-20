/****************************************************************************
**
** Copyright (C) 2015 Alexander Rössler
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

#ifndef QLAUNCHER_H
#define QLAUNCHER_H

#include <QObject>
#include <QCoreApplication>
#include <QJsonValue>
#include <abstractserviceimplementation.h>
#include <service.h>
#include <nzmqt/nzmqt.hpp>
#include <google/protobuf/text_format.h>
#include <google/protobuf/message.h>
#include <google/protobuf/descriptor.h>
#include "message.pb.h"
#include "config.pb.h"

#if defined(Q_OS_IOS)
namespace gpb = google_public::protobuf;
#else
namespace gpb = google::protobuf;
#endif

using namespace nzmqt;

class QLauncher : public AbstractServiceImplementation
{
    Q_OBJECT
    Q_INTERFACES(QQmlParserStatus)
    Q_PROPERTY(QString commandUri READ commandUri WRITE setCommandUri NOTIFY commandUriChanged)
    Q_PROPERTY(QString subscribeUri READ subscribeUri WRITE setSubscribeUri NOTIFY subscribeUriChanged)
    Q_PROPERTY(int heartbeatPeriod READ heartbeatPeriod WRITE heartbeatPeriod NOTIFY heartbeatPeriodChanged)
    Q_PROPERTY(bool connected READ isConnected NOTIFY connectedChanged)
    Q_PROPERTY(Service::State connectionState READ connectionState NOTIFY connectionStateChanged)
    Q_PROPERTY(Service::ConnectionError error READ error NOTIFY errorChanged)
    Q_PROPERTY(QString errorString READ errorString NOTIFY errorStringChanged)
    Q_PROPERTY(QJsonValue launchers READ launchers NOTIFY launchersChanged)
    Q_PROPERTY(bool synced READ isSynced NOTIFY syncedChanged)

public:
    explicit QLauncher(QObject *parent = 0);
    ~QLauncher();

    QString commandUri() const
    {
        return m_commandUri;
    }

    QString subscribeUri() const
    {
        return m_subscribeUri;
    }

    int heartbeatPeriod() const
    {
        return m_heartbeatPeriod;
    }

    bool isConnected() const
    {
        return m_connected;
    }

    Service::State connectionState() const
    {
        return m_connectionState;
    }

    Service::ConnectionError error() const
    {
        return m_error;
    }

    QString errorString() const
    {
        return m_errorString;
    }

    QJsonValue launchers() const
    {
        return m_launchers;
    }

    bool isSynced() const
    {
        return m_synced;
    }

public slots:
    void setCommandUri(QString arg)
    {
        if (m_commandUri == arg)
            return;

        m_commandUri = arg;
        emit commandUriChanged(arg);
    }

    void setSubscribeUri(QString arg)
    {
        if (m_subscribeUri == arg)
            return;

        m_subscribeUri = arg;
        emit subscribeUriChanged(arg);
    }

    void heartbeatPeriod(int arg)
    {
        if (m_heartbeatPeriod == arg)
            return;

        m_heartbeatPeriod = arg;
        emit heartbeatPeriodChanged(arg);
    }

    void start(int index);
    void stop(int index);
    void writeToStdin(int index, const QString &data);
    void call(const QString &command);
    void shutdown();

private:
    QString m_subscribeUri;
    QString m_commandUri;
    QString m_commandIdentity;
    int m_heartbeatPeriod;
    bool m_connected;
    Service::SocketState m_subscribeSocketState;
    Service::SocketState m_commandSocketState;
    Service::State m_connectionState;
    Service::ConnectionError m_error;
    QString m_errorString;
    QJsonValue m_launchers;
    bool m_synced;

    PollingZMQContext *m_context;
    ZMQSocket  *m_subscribeSocket;
    ZMQSocket  *m_commandSocket;
    QTimer     *m_commandHeartbeatTimer;
    QTimer     *m_subscribeHeartbeatTimer;
    bool        m_commandPingOutstanding;
    // more efficient to reuse a protobuf Message
    pb::Container   m_rx;
    pb::Container   m_tx;

    void start();
    void stop();
    void cleanup();
    void startCommandHeartbeat();
    void stopCommandHeartbeat();
    void startSubscribeHeartbeat(int interval);
    void stopSubscribeHeartbeat();
    void refreshSubscribeHeartbeat();
    void updateState(Service::State state);
    void updateState(Service::State state, Service::ConnectionError error, QString errorString);
    void updateError(Service::ConnectionError error, QString errorString);
    void sendCommandMessage(pb::ContainerType type);
    void updateSync();
    void clearSync();
    void initializeObject();

private slots:
    void subscribeMessageReceived(QList<QByteArray> messageList);
    void commandMessageReceived(QList<QByteArray> messageList);
    void pollError(int errorNum, const QString& errorMsg);
    void commandHeartbeatTimerTick();
    void subscribeHeartbeatTimerTick();

    bool connectSockets();
    void disconnectSockets();
    void subscribe(const QString &topic);
    void unsubscribe(const QString &topic);

signals:
    void commandUriChanged(QString arg);
    void subscribeUriChanged(QString arg);
    void heartbeatPeriodChanged(int arg);
    void connectedChanged(bool arg);
    void connectionStateChanged(Service::State arg);
    void errorChanged(Service::ConnectionError arg);
    void errorStringChanged(QString arg);
    void launchersChanged(QJsonValue arg);
    void syncedChanged(bool arg);
};

#endif // QLAUNCHER_H
