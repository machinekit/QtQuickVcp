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

#include <QQuickItem>
#include <QHash>
#include <QTimer>
#include "qhalpin.h"
#include <nzmqt/nzmqt.hpp>
#include "message.pb.h"
#include <google/protobuf/text_format.h>

#if defined(Q_OS_IOS)
namespace gpb = google_public::protobuf;
#else
namespace gpb = google::protobuf;
#endif

using namespace nzmqt;

class QHalRemoteComponent : public QQuickItem
{
    Q_OBJECT
    Q_INTERFACES(QQmlParserStatus)
    Q_PROPERTY(QString cmdUri READ cmdUri WRITE setCmdUri NOTIFY cmdUriChanged)
    Q_PROPERTY(QString updateUri READ updateUri WRITE setUpdateUri NOTIFY updateUriChanged)
    Q_PROPERTY(QString name READ name WRITE setName NOTIFY nameChanged)
    Q_PROPERTY(int heartbeatPeriod READ heartbeatPeriod WRITE heartbeatPeriod NOTIFY heartbeatPeriodChanged)
    Q_PROPERTY(bool synced READ isSynced NOTIFY syncedChanged)
    Q_PROPERTY(bool ready READ ready WRITE setReady NOTIFY readyChanged)
    Q_PROPERTY(State connectionState READ connectionState NOTIFY connectionStateChanged)
    Q_PROPERTY(ConnectionError error READ error NOTIFY errorChanged)
    Q_PROPERTY(QString errorString READ errorString NOTIFY errorStringChanged)
    Q_PROPERTY(QQuickItem *containerItem READ containerItem WRITE setContainerItem NOTIFY containerItemChanged)
    Q_ENUMS(SocketState)
    Q_ENUMS(State)
    Q_ENUMS(ConnectionError)

public:
    explicit QHalRemoteComponent(QQuickItem *parent = 0);
    ~QHalRemoteComponent();

    enum SocketState {
        Down = 1,
        Trying = 2,
        Up = 3
    };

    enum State {
        Disconnected = 0,
        Connecting = 1,
        Connected = 2,
        Error = 3
    };

    enum ConnectionError {
        NoError = 0,
        BindError = 1,
        PinChangeError = 2,
        CommandError = 3,
        TimeoutError = 4,
        SocketError = 5
    };

    virtual void componentComplete();

    QString cmdUri() const
    {
        return m_cmdUri;
    }

    QString updateUri() const
    {
        return m_updateUri;
    }

    QString name() const
    {
        return m_name;
    }

    int heartbeatPeriod() const
    {
        return m_heartbeatPeriod;
    }

    bool isSynced() const
    {
        return m_synced;
    }

    bool ready() const
    {
        return m_ready;
    }

    QQuickItem *containerItem() const
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

public slots:
    void pinUpdate(pb::Pin remotePin, QHalPin *localPin);
    void pinChange(QVariant value);

    void setCmdUri(QString arg)
    {
        if (m_cmdUri != arg) {
            m_cmdUri = arg;
            emit cmdUriChanged(arg);
        }
    }

    void setUpdateUri(QString arg)
    {
        if (m_updateUri != arg) {
            m_updateUri = arg;
            emit updateUriChanged(arg);
        }
    }

    void setName(QString arg)
    {
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

    void setReady(bool arg);

    void setContainerItem(QQuickItem *arg)
    {
        if (m_containerItem != arg) {
            m_containerItem = arg;
            emit containerItemChanged(arg);
        }
    }

private:
    QString     m_cmdUri;
    QString     m_updateUri;
    QString     m_name;
    int         m_heartbeatPeriod;
    bool        m_synced;
    SocketState m_sState;
    SocketState m_cState;
    State       m_connectionState;
    ConnectionError       m_error;
    QString     m_errorString;
    bool        m_ready;
    QQuickItem  *m_containerItem;
    bool        m_componentCompleted;

    ZMQContext *m_context;
    ZMQSocket  *m_updateSocket;
    ZMQSocket  *m_cmdSocket;
    QTimer      *m_heartbeatTimer;
    int         m_pingOutstanding;
    // more efficient to reuse a protobuf Message
    pb::Container   m_rx;
    pb::Container   m_tx;
    QMap<QString, QHalPin*> m_pinsByName;
    QHash<int, QHalPin*>    m_pinsByHandle;


    QObjectList recurseObjects(const QObjectList &list);
    void start();
    void stop();
    void startHeartbeat();
    void stopHeartbeat();
    void updateState(State state);
    void updateError(ConnectionError error, QString errorString);

private slots:
    void updateMessageReceived(QList<QByteArray> messageList);
    void cmdMessageReceived(QList<QByteArray> messageList);
    void hearbeatTimerTick();

    void addPins();
    void removePins();
    void connectSockets();
    void disconnectSockets();
    void bind();

signals:
    void cmdUriChanged(QString arg);
    void updateUriChanged(QString arg);
    void nameChanged(QString arg);
    void heartbeatPeriodChanged(int arg);
    void syncedChanged(bool arg);
    void readyChanged(bool arg);
    void containerItemChanged(QQuickItem *arg);
    void connectionStateChanged(State arg);
    void errorChanged(ConnectionError arg);
    void errorStringChanged(QString arg);
};

#endif // QCOMPONENT_H
