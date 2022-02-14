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
#include <QQmlListProperty>
#include <machinetalk/protobuf/message.pb.h>
#include <halremote/remotecomponentbase.h>
#include "halpin.h"

namespace qtquickvcp {

class HalRemoteComponent : public machinetalk::halremote::RemoteComponentBase
{
    Q_OBJECT
    Q_PROPERTY(QString name READ name WRITE setName NOTIFY nameChanged)
    Q_PROPERTY(bool connected READ isConnected NOTIFY connectedChanged)
    Q_PROPERTY(ConnectionError error READ error NOTIFY errorChanged)
    Q_PROPERTY(QObject *containerItem READ containerItem WRITE setContainerItem NOTIFY containerItemChanged)
    Q_PROPERTY(bool create READ create WRITE setCreate NOTIFY createChanged)
    Q_PROPERTY(bool bind READ bind WRITE setBind NOTIFY bindChanged)
    Q_PROPERTY(QQmlListProperty<qtquickvcp::HalPin> pins READ pins NOTIFY pinsChanged)

public:
    explicit HalRemoteComponent(QObject *parent  = nullptr);

    enum ConnectionError {
        NoError = 0,
        ComponentError  = 1
    };
    Q_ENUM(ConnectionError)

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

    Q_INVOKABLE int pinCount() const
    {
        return m_pins.count();
    }

    Q_INVOKABLE HalPin *pin(int index) const
    {
        return m_pins.at(index);
    }

public slots:
    void setName(QString name)
    {
        if (this->state() != State::Down) {
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
        return QQmlListProperty<HalPin>(this, &m_pins);
    }

    void pinChange(QVariant value);

private:
    QString         m_name;
    bool            m_connected;
    ConnectionError m_error;
    QObject*        m_containerItem;
    bool            m_create;
    bool            m_bind;

    // more efficient to reuse a protobuf Message
    machinetalk::Container          m_tx;
    QMap<QString, HalPin*> m_pinsByName;
    QHash<int, HalPin*>    m_pinsByHandle;
    QList<HalPin*>         m_pins;

    const QObjectList recurseObjects(const QObjectList &list);
    void bindPins();
    static QString splitPinFromHalName(const QString &name);

    void pinUpdate(const machinetalk::Pin &remotePin, HalPin *localPin);
    HalPin *addLocalPin(const machinetalk::Pin &remotePin);

    // RemoteComponentBase interface
private slots:
    void addPins();
    void removePins();
    void unsyncPins();
    void handleHalrcompFullUpdateMessage(const QByteArray &topic, const machinetalk::Container &rx);
    void handleHalrcompIncrementalUpdateMessage(const QByteArray &topic, const machinetalk::Container &rx);
    void handleHalrcompErrorMessage(const QByteArray &topic, const machinetalk::Container &rx);
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
    void containerItemChanged(QObject * containerItem);
    void createChanged(bool create);
    void bindChanged(bool bind);
    void pinsChanged(QQmlListProperty<HalPin> arg);
}; // class HalRemoteComponent
} // namespace qtquickvcp

#endif // HALREMOTECOMPONENT_H
