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

#ifndef QLOCALSETTINGS_H
#define QLOCALSETTINGS_H

#include <QObject>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonValueRef>
#include <QJsonDocument>
#include <QFileInfo>
#include <QDir>
#include <QStandardPaths>

class QLocalSettings : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QJsonObject values READ values WRITE setValues NOTIFY valuesChanged)
    Q_PROPERTY(QString name READ name WRITE setName NOTIFY nameChanged)
    Q_PROPERTY(QString application READ application WRITE setApplication NOTIFY applicatioChanged)

public:
    explicit QLocalSettings(QObject *parent = 0);
    ~QLocalSettings();

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

signals:
    void valuesChanged(QJsonObject arg);
    void applicatioChanged(QString arg);
    void nameChanged(QString arg);

public slots:
    void setValues(QJsonObject arg);
    void setValue(const QString &key, const QJsonValue &value, bool overwrite);
    void setApplication(QString arg);
    void setName(QString arg);
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
};
#endif // QLOCALSETTINGS_H
