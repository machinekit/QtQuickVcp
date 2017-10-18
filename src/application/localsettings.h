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
#ifndef LOCALSETTINGS_H
#define LOCALSETTINGS_H

#include <QObject>
#include <QJsonObject>

namespace qtquickvcp {

class LocalSettings : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QJsonObject values READ values WRITE setValues NOTIFY valuesChanged)
    Q_PROPERTY(QString name READ name WRITE setName NOTIFY nameChanged)
    Q_PROPERTY(QString application READ application WRITE setApplication NOTIFY applicatioChanged)
    Q_PROPERTY(QString filePath READ filePath WRITE setFilePath NOTIFY filePathChanged)

public:
    explicit LocalSettings(QObject *parent = 0);
    ~LocalSettings();

    QJsonObject values() const
    {
        return m_values;
    }

    QString application() const
    {
        return m_application;
    }

    QString name() const
    {
        return m_name;
    }

    QString filePath() const
    {
        return m_filePath;
    }

signals:
    void valuesChanged(const QJsonObject &arg);
    void applicatioChanged(const QString &arg);
    void nameChanged(const QString &arg);
    void filePathChanged(const QString &arg);

public slots:
    void setValues(const QJsonObject &arg);
    void setValue(const QString &key, const QJsonValue &value, bool overwrite);
    void setApplication(const QString &arg);
    void setName(const QString &arg);
    void setFilePath(const QString &arg);
    void save();
    void load();
    void setValue(const QString &key, const QJsonValue &value);
    QJsonValue value(const QString &key);

private:
    QJsonObject m_values;
    QString     m_application;
    QString     m_name;
    QString     m_filePath;

    void loadSettings();
    void saveSettings();
    void updateFilePath();
}; // class LocalSettings
} // namespace qtquickvcp

#endif // LOCALSETTINGS_H
