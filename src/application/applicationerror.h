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

#include <QObject>
#include <machinetalk/protobuf/message.pb.h>
#include <application/errorbase.h>

namespace qtquickvcp {

class ApplicationError : public machinetalk::application::ErrorBase
{
    Q_OBJECT
    Q_PROPERTY(bool connected READ isConnected NOTIFY connectedChanged)
    Q_PROPERTY(ErrorChannels channels READ channels WRITE setChannels NOTIFY channelsChanged)
    Q_ENUMS(ErrorType)
    Q_FLAGS(ErrorChannels)

public:
    explicit ApplicationError(QObject *parent = 0);

    enum ErrorType {
        NmlError = machinetalk::MT_EMC_NML_ERROR,
        NmlText = machinetalk::MT_EMC_NML_TEXT,
        NmlDisplay = machinetalk::MT_EMC_NML_DISPLAY,
        OperatorError = machinetalk::MT_EMC_OPERATOR_ERROR,
        OperatorText = machinetalk::MT_EMC_OPERATOR_TEXT,
        OperatorDisplay = machinetalk::MT_EMC_OPERATOR_DISPLAY
    };

    enum ErrorChannelEnum {
        NoChannel      = 0x0,
        ErrorChannel   = 0x1,
        TextChannel    = 0x2,
        DisplayChannel = 0x4,
    };
    Q_DECLARE_FLAGS(ErrorChannels, ErrorChannelEnum)

    ErrorChannels channels() const
    {
        return m_channels;
    }

    bool isConnected() const
    {
        return m_connected;
    }

public slots:
    void setChannels(ErrorChannels arg)
    {
        if (m_channels == arg)
            return;

        m_channels = arg;
        emit channelsChanged(arg);
    }

private:
    bool            m_connected;
    ErrorChannels   m_channels;

    void errorMessageReceived(const machinetalk::Container &rx);

private slots:
    void emcNmlErrorReceived(const QByteArray &topic, const machinetalk::Container &rx);
    void emcNmlTextReceived(const QByteArray &topic, const machinetalk::Container &rx);
    void emcNmlDisplayReceived(const QByteArray &topic, const machinetalk::Container &rx);
    void emcOperatorTextReceived(const QByteArray &topic, const machinetalk::Container &rx);
    void emcOperatorErrorReceived(const QByteArray &topic, const machinetalk::Container &rx);
    void emcOperatorDisplayReceived(const QByteArray &topic, const machinetalk::Container &rx);
    void updateTopics();
    void setConnected();
    void clearConnected();

signals:
    void channelsChanged(ErrorChannels arg);
    void messageReceived(ErrorType type, const QString &text);
    void connectedChanged(bool arg);
}; // class ApplicationError
} // namespace qtquickvcp

#endif // APPLICATIONERROR_H
