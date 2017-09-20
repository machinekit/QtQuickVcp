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

#include "applicationlog.h"
#include "applicationlogmessage.h"
#include <QDateTime>
#include <QJsonObject>

using namespace machinetalk;

namespace qtquickvcp {

ApplicationLog::ApplicationLog(QObject *parent)
    : application::LogBase(parent)
    , m_connected(false)
    , m_logLevel(Warning)
{

}

bool ApplicationLog::isConnected() const
{
    return m_connected;
}

ApplicationLog::LogLevel ApplicationLog::logLevel() const
{
    return m_logLevel;
}

void ApplicationLog::setLogLevel(LogLevel logLevel)
{
    if (m_logLevel == logLevel) {
        return;
    }

    m_logLevel = logLevel;
    emit logLevelChanged(m_logLevel);
}


void ApplicationLog::handleLogMessageMessage(const QByteArray &topic, const Container &rx)
{
    Q_UNUSED(topic)

    const auto &logMessage = rx.log_message();
    if (static_cast<int>(logMessage.level()) <= static_cast<int>(m_logLevel)) {
        ApplicationLogMessage log;
        log.setLevel(static_cast<LogLevel>(logMessage.level()));
        log.setOrigin(static_cast<LogOrigin>(logMessage.origin()));
        log.setPid(logMessage.pid());
        log.setTag(QString::fromStdString(logMessage.tag()));
        log.setText(QString::fromStdString(logMessage.text()));
        qint64 msecs = static_cast<qint64>(rx.tv_sec()) * 1000L;
        msecs += static_cast<qint64>(rx.tv_nsec()) / 1000000L;
        log.setTimestamp(QDateTime::fromMSecsSinceEpoch(msecs));
        emit messageReceived(log.toJsonObject());
     }
}

void ApplicationLog::updateTopics()
{
    clearLogTopics();
    addLogTopic("log");
}

void ApplicationLog::setConnected()
{
    m_connected = true;
    emit connectedChanged(m_connected);
}

void ApplicationLog::clearConnected()
{
    m_connected = false;
    emit connectedChanged(m_connected);
}

} // qtquickvcp
