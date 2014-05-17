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
    Q_PROPERTY(State sState READ sState NOTIFY sStateChanged)
    Q_PROPERTY(State cState READ cState NOTIFY cStateChanged)
    Q_PROPERTY(bool ready READ ready WRITE setReady NOTIFY readyChanged)
    Q_PROPERTY(QQuickItem *containerItem READ containerItem WRITE setContainerItem NOTIFY containerItemChanged)
    Q_ENUMS(State)

public:
    explicit QHalRemoteComponent(QQuickItem *parent = 0);
    ~QHalRemoteComponent();

    enum State {
        STATE_DISCOVER = 0,
        STATE_DOWN = 1,
        STATE_TRYING = 2,
        STATE_UP = 3
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

    State sState() const
    {
        return m_sState;
    }

    State cState() const
    {
        return m_cState;
    }

    bool ready() const
    {
        return m_ready;
    }

    QQuickItem *containerItem() const
    {
        return m_containerItem;
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
    QString m_cmdUri;
    QString m_updateUri;
    QString m_name;
    int m_heartbeatPeriod;
    bool m_synced;
    State m_sState;
    State m_cState;
    bool m_ready;
    QQuickItem *m_containerItem;

    ZMQContext *m_context;
    ZMQSocket  *m_updateSocket;
    ZMQSocket  *m_cmdSocket;
    int         m_instanceCount;
    int         m_pingOutstanding;
    // more efficient to reuse a protobuf Message
    pb::Container m_rx;
    pb::Container m_tx;
    QMap<QString, QHalPin*> m_pinsByName;
    QHash<int, QHalPin*> m_pinsByHandle;
    QTimer      *m_heartbeatTimer;
    bool m_componentCompleted;

    QObjectList recurseObjects(const QObjectList &list);
    void start();
    void stop();

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
void protocolError(QStringList notes);
void sStateChanged(State arg);
void cStateChanged(State arg);
void readyChanged(bool arg);
void containerItemChanged(QQuickItem *arg);
};

#endif // QCOMPONENT_H
