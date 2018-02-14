/****************************************************************************
**
** Copyright (C) 2017 Alexander Rössler
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
** Alexander Rössler <alexander AT roessler DOT systems>
**
****************************************************************************/

#ifndef APPLICATIONLOG_H
#define APPLICATIONLOG_H

#include <QObject>
#include <machinetalk/protobuf/message.pb.h>
#include <application/logbase.h>

namespace qtquickvcp {

class ApplicationLogMessage;

class ApplicationLog : public machinetalk::application::LogBase
{
    Q_OBJECT
    Q_PROPERTY(bool connected READ isConnected NOTIFY connectedChanged)
    Q_PROPERTY(LogLevel logLevel READ logLevel WRITE setLogLevel NOTIFY logLevelChanged)

public:
    ApplicationLog(QObject *parent = nullptr);

    enum LogLevel {
        None = machinetalk::RTAPI_MSG_NONE,
        Error = machinetalk::RTAPI_MSG_ERR,
        Warning = machinetalk::RTAPI_MSG_WARN,
        Info = machinetalk::RTAPI_MSG_INFO,
        Debug = machinetalk::RTAPI_MSG_DBG,
        All = machinetalk::RTAPI_MSG_ALL
    };
    Q_ENUM(LogLevel)

    enum LogOrigin {
        Kernel = machinetalk::MSG_KERNEL,
        RtUser = machinetalk::MSG_RTUSER,
        Ulapi = machinetalk::MSG_ULAPI
    };
    Q_ENUM(LogOrigin)

    bool isConnected() const;
    LogLevel logLevel() const;

public slots:
    void setLogLevel(LogLevel logLevel);

signals:
    void connectedChanged(bool connected);
    void logLevelChanged(LogLevel logLevel);
    void messageReceived(const QJsonObject &message);

private:
    bool m_connected;
    LogLevel m_logLevel;

private slots:
    void handleLogMessageMessage(const QByteArray &topic, const machinetalk::Container &rx);
    void updateTopics();
    void setConnected();
    void clearConnected();

}; // class ApplicationLog
} // namespace qtquickvcp

#endif // APPLICATIONLOG_H
