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

#ifndef APPLICATIONLOGMESSAGE_H
#define APPLICATIONLOGMESSAGE_H

#include <QObject>
#include <QDateTime>
#include <QJsonObject>
#include "applicationlog.h"

namespace qtquickvcp {

class ApplicationLogMessage : public QObject
{
    Q_OBJECT
    Q_PROPERTY(ApplicationLog::LogLevel level READ level WRITE setLevel NOTIFY levelChanged)
    Q_PROPERTY(ApplicationLog::LogOrigin origin READ origin WRITE setOrigin NOTIFY originChanged)
    Q_PROPERTY(QString tag READ tag WRITE setTag NOTIFY tagChanged)
    Q_PROPERTY(int pid READ pid WRITE setPid NOTIFY pidChanged)
    Q_PROPERTY(QString text READ text WRITE setText NOTIFY textChanged)
    Q_PROPERTY(QDateTime timestamp READ timestamp WRITE setTimestamp NOTIFY timestampChanged)

public:
    explicit ApplicationLogMessage(QObject *parent = nullptr);

    ApplicationLog::LogLevel level() const;
    ApplicationLog::LogOrigin origin() const;
    QString tag() const;
    int pid() const;
    QString text() const;
    QDateTime timestamp() const;

    QJsonObject toJsonObject() const;

signals:
    void levelChanged(ApplicationLog::LogLevel level);
    void originChanged(ApplicationLog::LogOrigin origin);
    void tagChanged(const QString &tag);
    void pidChanged(int pid);
    void textChanged(const QString &text);
    void timestampChanged(const QDateTime &timestamp);

public slots:
    void setLevel(ApplicationLog::LogLevel level);
    void setOrigin(ApplicationLog::LogOrigin origin);
    void setTag(const QString &tag);
    void setPid(int pid);
    void setText(const QString &text);
    void setTimestamp(QDateTime timestamp);

private:
    ApplicationLog::LogLevel m_level;
    ApplicationLog::LogOrigin m_origin;
    QString m_tag;
    int m_pid;
    QString m_text;
    QDateTime m_timestamp;

}; // class ApplicationLogMessage
} // namespace qtquickvcp

#endif // APPLICATIONLOGMESSAGE_H
