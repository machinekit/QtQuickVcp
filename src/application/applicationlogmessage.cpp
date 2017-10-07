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

#include "applicationlogmessage.h"
#include <QVariant>

namespace qtquickvcp {

ApplicationLogMessage::ApplicationLogMessage(QObject *parent)
    : QObject(parent)
{

}

ApplicationLog::LogLevel ApplicationLogMessage::level() const
{
    return m_level;
}

ApplicationLog::LogOrigin ApplicationLogMessage::origin() const
{
    return m_origin;
}

QString ApplicationLogMessage::tag() const
{
    return m_tag;
}

int ApplicationLogMessage::pid() const
{
    return m_pid;
}

QString ApplicationLogMessage::text() const
{
    return m_text;
}

QDateTime ApplicationLogMessage::timestamp() const
{
    return m_timestamp;
}

QJsonObject ApplicationLogMessage::toJsonObject() const
{
    QJsonObject object;
    object["level"] = m_level;
    object["origin"] = m_origin;
    object["pid"] = m_pid;
    object["tag"] = m_tag;
    object["text"] = m_text;
    object["timestamp"] = QJsonValue::fromVariant(m_timestamp);
    return object;
}

void ApplicationLogMessage::setLevel(ApplicationLog::LogLevel level)
{
    if (m_level == level) {
        return;
    }

    m_level = level;
    emit levelChanged(m_level);
}

void ApplicationLogMessage::setOrigin(ApplicationLog::LogOrigin origin)
{
    if (m_origin == origin) {
        return;
    }

    m_origin = origin;
    emit originChanged(m_origin);
}

void ApplicationLogMessage::setTag(const QString &tag)
{
    if (m_tag == tag) {
        return;
    }

    m_tag = tag;
    emit tagChanged(m_tag);
}

void ApplicationLogMessage::setPid(int pid)
{
    if (m_pid == pid) {
        return;
    }

    m_pid = pid;
    emit pidChanged(m_pid);
}

void ApplicationLogMessage::setText(const QString &text)
{
    if (m_text == text) {
        return;
    }

    m_text = text;
    emit textChanged(m_text);
}

void ApplicationLogMessage::setTimestamp(QDateTime timestamp)
{
    if (m_timestamp == timestamp) {
        return;
    }

    m_timestamp = timestamp;
    emit timestampChanged(m_timestamp);
}

} // qtquickvcp
