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
#ifndef APPLICATIONERROR_H
#define APPLICATIONERROR_H

#include <abstractserviceimplementation.h>
#include <QStringList>
#include <QTimer>
#include <nzmqt/nzmqt.hpp>
#include <google/protobuf/text_format.h>
#include <machinetalk/protobuf/message.pb.h>

namespace qtquickvcp {

class ApplicationError : public AbstractServiceImplementation
{
    Q_OBJECT
    Q_PROPERTY(QString errorUri READ errorUri WRITE setErrorUri NOTIFY errorUriChanged)
    Q_PROPERTY(bool connected READ isConnected NOTIFY connectedChanged)
    Q_PROPERTY(State connectionState READ connectionState NOTIFY connectionStateChanged)
    Q_PROPERTY(ConnectionError error READ error NOTIFY errorChanged)
    Q_PROPERTY(QString errorString READ errorString NOTIFY errorStringChanged)
    Q_PROPERTY(ErrorChannels channels READ channels WRITE setChannels NOTIFY channelsChanged)
    Q_ENUMS(State ConnectionError ErrorType)
    Q_FLAGS(ErrorChannels)

public:
    explicit ApplicationError(QObject *parent = 0);

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
        ServiceError = 1,
        SocketError = 2
    };

    enum ErrorType {
        NmlError = pb::MT_EMC_NML_ERROR,
        NmlText = pb::MT_EMC_NML_TEXT,
        NmlDisplay = pb::MT_EMC_NML_DISPLAY,
        OperatorError = pb::MT_EMC_OPERATOR_ERROR,
        OperatorText = pb::MT_EMC_OPERATOR_TEXT,
        OperatorDisplay = pb::MT_EMC_OPERATOR_DISPLAY
    };

    enum ErrorChannelEnum {
        ErrorChannel   = 0x1,
        TextChannel    = 0x2,
        DisplayChannel = 0x4,
    };
    Q_DECLARE_FLAGS(ErrorChannels, ErrorChannelEnum)

    QString errorUri() const
    {
        return m_errorUri;
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

    ErrorChannels channels() const
    {
        return m_channels;
    }

    bool isConnected() const
    {
        return m_connected;
    }

public slots:

    void setErrorUri(const QString &arg)
    {
        if (m_errorUri == arg)
            return;

        m_errorUri = arg;
        emit errorUriChanged(arg);
    }

    void setChannels(ErrorChannels arg)
    {
        if (m_channels == arg)
            return;

        m_channels = arg;
        emit channelsChanged(arg);
    }

private:
    QString         m_errorUri;
    bool            m_connected;
    SocketState     m_errorSocketState;
    State           m_connectionState;
    ConnectionError m_error;
    QString         m_errorString;
    ErrorChannels   m_channels;

    nzmqt::PollingZMQContext *m_context;
    nzmqt::ZMQSocket *m_errorSocket;
    QStringList  m_subscriptions;
    QTimer      *m_errorHeartbeatTimer;
    // more efficient to reuse a protobuf Message
    pb::Container   m_rx;

    void start();
    void stop();
    void cleanup();
    void startErrorHeartbeat(int interval);
    void stopErrorHeartbeat();
    void refreshErrorHeartbeat();
    void updateState(State state);
    void updateState(State state, ConnectionError error, const QString &errorString);
    void updateError(ConnectionError error, const QString &errorString);

private slots:
    void errorMessageReceived(const QList<QByteArray> &messageList);
    void pollError(int errorNum, const QString &errorMsg);
    void errorHeartbeatTimerTick();

    bool connectSockets();
    void disconnectSockets();
    void subscribe();
    void unsubscribe();

signals:
    void errorUriChanged(const QString &arg);
    void connectionStateChanged(State arg);
    void errorChanged(ConnectionError arg);
    void errorStringChanged(const QString &arg);
    void channelsChanged(ErrorChannels arg);
    void messageReceived(ErrorType type, const QString &text);
    void connectedChanged(bool arg);
}; // class ApplicationError
} // namespace qtquickvcp

#endif // APPLICATIONERROR_H
