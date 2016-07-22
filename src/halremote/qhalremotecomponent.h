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
#ifndef QCOMPONENT_H
#define QCOMPONENT_H

#include <abstractserviceimplementation.h>
#include <QCoreApplication>
#include <QHash>
#include <QTimer>
#include <QUuid>
#include <QQmlListProperty>
#include "qhalpin.h"
#include <nzmqt/nzmqt.hpp>
#include <machinetalk/protobuf/message.pb.h>
#include <google/protobuf/text_format.h>

#if defined(Q_OS_IOS)
namespace gpb = google_public::protobuf;
#else
namespace gpb = google::protobuf;
#endif

using namespace nzmqt;

class QHalRemoteComponent : public AbstractServiceImplementation
{
    Q_OBJECT
    Q_INTERFACES(QQmlParserStatus)
    Q_PROPERTY(QString halrcmdUri READ halrcmdUri WRITE setHalrcmdUri NOTIFY halrcmdUriChanged)
    Q_PROPERTY(QString halrcompUri READ halrcompUri WRITE setHalrcompUri NOTIFY halrcompUriChanged)
    Q_PROPERTY(QString name READ name WRITE setName NOTIFY nameChanged)
    Q_PROPERTY(int heartbeatPeriod READ heartbeatPeriod WRITE heartbeatPeriod NOTIFY heartbeatPeriodChanged)
    Q_PROPERTY(bool connected READ isConnected NOTIFY connectedChanged)
    Q_PROPERTY(State connectionState READ connectionState NOTIFY connectionStateChanged)
    Q_PROPERTY(ConnectionError error READ error NOTIFY errorChanged)
    Q_PROPERTY(QString errorString READ errorString NOTIFY errorStringChanged)
    Q_PROPERTY(QObject *containerItem READ containerItem WRITE setContainerItem NOTIFY containerItemChanged)
    Q_PROPERTY(bool create READ create WRITE setCreate NOTIFY createChanged)
    Q_PROPERTY(QQmlListProperty<QHalPin> pins READ pins NOTIFY pinsChanged)
    Q_ENUMS(SocketState)
    Q_ENUMS(State)
    Q_ENUMS(ConnectionError)

public:
    explicit QHalRemoteComponent(QObject *parent = 0);

    enum SocketState {
        Down = 1,
        Trying = 2,
        Up = 3
    };

    enum State {
        Disconnected = 0,
        Connecting = 1,
        Connected = 2,
        Timeout = 3,
        Error = 4
    };

    enum ConnectionError {
        NoError = 0,
        BindError = 1,
        PinChangeError = 2,
        CommandError = 3,
        SocketError = 4
    };

    QString halrcmdUri() const
    {
        return m_halrcmdUri;
    }

    QString halrcompUri() const
    {
        return m_halrcompUri;
    }

    QString name() const
    {
        return m_name;
    }

    int heartbeatPeriod() const
    {
        return m_heartbeatPeriod;
    }

    QObject *containerItem() const
    {
        return m_containerItem;
    }

    State connectionState() const
    {
        return m_connectionState;
    }

    ConnectionError error() const
    {
        return m_error;
    }

    QString errorString() const
    {
        return m_errorString;
    }

    bool isConnected() const
    {
        return m_connected;
    }

    bool create() const
    {
        return m_create;
    }

public slots:
    void pinChange(QVariant value);

    void setHalrcmdUri(QString arg)
    {
        if (m_halrcmdUri != arg) {
            m_halrcmdUri = arg;
            emit halrcmdUriChanged(arg);
        }
    }

    void setHalrcompUri(QString arg)
    {
        if (m_halrcompUri != arg) {
            m_halrcompUri = arg;
            emit halrcompUriChanged(arg);
        }
    }

    void setName(QString arg)
    {
        if (m_connectionState != Disconnected)
            return;

        if (m_name != arg) {
            m_name = arg;
            emit nameChanged(arg);
        }
    }

    void heartbeatPeriod(int arg)
    {
        if (m_heartbeatPeriod != arg) {
            m_heartbeatPeriod = arg;
            emit heartbeatPeriodChanged(arg);
        }
    }

    void setContainerItem(QObject *arg)
    {
        if (m_containerItem != arg) {
            m_containerItem = arg;
            emit containerItemChanged(arg);
        }
    }

    void setCreate(bool arg)
    {
        if (m_create == arg)
            return;

        m_create = arg;
        emit createChanged(arg);
    }

    QQmlListProperty<QHalPin> pins();
    int pinCount() const;
    QHalPin *pin(int index) const;

private:
    QString     m_halrcmdUri;
    QString     m_halrcompUri;
    QString     m_name;
    int         m_heartbeatPeriod;
    bool        m_connected;
    SocketState m_halrcompSocketState;
    SocketState m_halrcmdSocketState;
    State       m_connectionState;
    ConnectionError       m_error;
    QString     m_errorString;
    QObject     *m_containerItem;
    bool        m_create;

    PollingZMQContext *m_context;
    ZMQSocket  *m_halrcompSocket;
    ZMQSocket  *m_halrcmdSocket;
    QTimer     *m_halrcmdHeartbeatTimer;
    QTimer     *m_halrcompHeartbeatTimer;
    bool        m_halrcmdPingOutstanding;
    QUuid       m_uuid;
    // more efficient to reuse a protobuf Message
    pb::Container   m_rx;
    pb::Container   m_tx;
    QMap<QString, QHalPin*> m_pinsByName;
    QHash<int, QHalPin*>    m_pinsByHandle;
    QList<QHalPin*>         m_pins;


    QObjectList recurseObjects(const QObjectList &list);
    void start();
    void stop();
    void cleanup();
    void startHalrcmdHeartbeat();
    void stopHalrcmdHeartbeat();
    void startHalrcompHeartbeat(int interval);
    void stopHalrcompHeartbeat();
    void refreshHalrcompHeartbeat();
    void updateState(State state);
    void updateState(State state, ConnectionError error, QString errorString);
    void updateError(ConnectionError error, QString errorString);
    void sendHalrcmdMessage(pb::ContainerType type);

private slots:
    void pinUpdate(const pb::Pin &remotePin, QHalPin *localPin);

    void halrcompMessageReceived(QList<QByteArray> messageList);
    void halrcmdMessageReceived(QList<QByteArray> messageList);
    void pollError(int errorNum, const QString& errorMsg);
    void halrcmdHeartbeatTimerTick();
    void halrcompHeartbeatTimerTick();

    void addPins();
    void removePins();
    void unsyncPins();
    bool connectSockets();
    void disconnectSockets();
    void bind();
    void subscribe();
    void unsubscribe();

signals:
    void halrcmdUriChanged(QString arg);
    void halrcompUriChanged(QString arg);
    void nameChanged(QString arg);
    void heartbeatPeriodChanged(int arg);
    void containerItemChanged(QObject *arg);
    void connectionStateChanged(State arg);
    void errorChanged(ConnectionError arg);
    void errorStringChanged(QString arg);
    void connectedChanged(bool arg);
    void createChanged(bool arg);
    void pinsChanged(QQmlListProperty<QHalPin> arg);
};

#endif // QCOMPONENT_H
