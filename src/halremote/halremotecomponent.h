/****************************************************************************
**
** Copyright (C) 2014-2016 Alexander Rössler
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
** Alexander Rössler @ Roessler Systems <mail AT roessler DOT systems>
**
****************************************************************************/
#ifndef HALREMOTECOMPONENT_H
#define HALREMOTECOMPONENT_H

#include <QObject>
#include <QQuickItem>
#include <QQmlListProperty>
#include <machinetalk/protobuf/message.pb.h>
#include <google/protobuf/text_format.h>
#include <halremote/remotecomponentbase.h>
#include "halpin.h"

namespace qtquickvcp {

class HalRemoteComponent : public halremote::RemoteComponentBase
{
    Q_OBJECT
    Q_PROPERTY(QString name READ name WRITE setName NOTIFY nameChanged)
    Q_PROPERTY(bool connected READ isConnected NOTIFY connectedChanged)
    Q_PROPERTY(ConnectionError error READ error NOTIFY errorChanged)
    Q_PROPERTY(QObject *containerItem READ containerItem WRITE setContainerItem NOTIFY containerItemChanged)
    Q_PROPERTY(bool create READ create WRITE setCreate NOTIFY createChanged)
    Q_PROPERTY(bool bind READ bind WRITE setBind NOTIFY bindChanged)
    Q_PROPERTY(QQmlListProperty<qtquickvcp::HalPin> pins READ pins NOTIFY pinsChanged)
    Q_ENUMS(ConnectionError)

public:
    explicit HalRemoteComponent(QObject *parent  = 0);

    enum ConnectionError {
        NoError = 0,
        BindError = 1,
        PinChangeError = 2,
        CommandError = 3,
        SocketError = 4
    };

    QString name() const
    {
        return m_name;
    }

    bool isConnected() const
    {
        return m_connected;
    }

    ConnectionError error() const
    {
        return m_error;
    }

    QString errorString() const
    {
        return m_errorString;
    }

    QObject *containerItem() const
    {
        return m_containerItem;
    }

    bool create() const
    {
        return m_create;
    }

    bool bind() const
    {
        return m_bind;
    }

public slots:
    void setName(QString name)
    {
        if (this->state() != Down) {
            return;
        }

        if (m_name != name) {
            m_name = name;
            emit nameChanged(name);
        }
    }

    void setContainerItem(QObject * containerItem)
    {
        if (m_containerItem == containerItem) {
            return;
        }

        m_containerItem = containerItem;
        emit containerItemChanged(containerItem);
    }

    void setCreate(bool create)
    {
        if (m_create == create) {
            return;
        }

        m_create = create;
        emit createChanged(create);
    }

    void setBind(bool bind)
    {
        if (m_bind == bind) {
            return;
        }

        m_bind = bind;
        emit bindChanged(bind);
    }

    QQmlListProperty<HalPin> pins()
    {
        return QQmlListProperty<HalPin>(this, m_pins);
    }
    int pinCount() const
    {
        return m_pins.count();
    }
    HalPin *pin(int index) const
    {
        return m_pins.at(index);
    }

    void pinChange(QVariant value);

private:
    QString         m_name;
    bool            m_connected;
    ConnectionError m_error;
    QString         m_errorString;
    QObject*        m_containerItem;
    bool            m_create;
    bool            m_bind;

    // more efficient to reuse a protobuf Message
    pb::Container          m_tx;
    QMap<QString, HalPin*> m_pinsByName;
    QHash<int, HalPin*>    m_pinsByHandle;
    QList<HalPin*>         m_pins;

    QObjectList recurseObjects(const QObjectList &list);
    void bindPins();
    static QString splitPinFromHalName(const QString &name);

    void pinUpdate(const pb::Pin &remotePin, HalPin *localPin);
    HalPin *addLocalPin(const pb::Pin &remotePin);

    // RemoteComponentBase interface
private slots:
    void addPins();
    void removePins();
    void unsyncPins();
    void halrcompFullUpdateReceived(const QByteArray &topic, pb::Container *rx);
    void halrcompIncrementalUpdateReceived(const QByteArray &topic, pb::Container *rx);
    void halrcompErrorReceived(const QByteArray &topic, pb::Container *rx);
    void bindComponent();
    void setConnected();
    void setError();
    void setDisconnected();
    void setConnecting();
    void setTimeout();

signals:
    void nameChanged(QString name);
    void connectedChanged(bool connected);
    void errorChanged(ConnectionError error);
    void errorStringChanged(QString errorString);
    void containerItemChanged(QObject * containerItem);
    void createChanged(bool create);
    void bindChanged(bool bind);
    void pinsChanged(QQmlListProperty<HalPin> arg);
}; // class HalRemoteComponent
}; // namespace qtquickvcp

#endif // HALREMOTECOMPONENT_H
