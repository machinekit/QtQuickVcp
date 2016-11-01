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

#include "localsettings.h"

namespace qtquickvcp {

LocalSettings::LocalSettings(QObject *parent) :
    QObject(parent),
    m_application("machinekit"),
    m_name("settings")
{
    updateFilePath();
}

LocalSettings::~LocalSettings()
{
}

void LocalSettings::loadSettings()
{
    QFile file(m_filePath);

    if (file.exists() && file.open(QIODevice::ReadOnly))
    {
        m_values = QJsonDocument::fromJson(file.readAll()).object();
        file.close();
    }
    else
    {
        m_values = QJsonObject();
    }

    emit valuesChanged(m_values);
}

void LocalSettings::saveSettings()
{
    QDir dir;
    QFileInfo fileInfo(m_filePath);

    if (dir.mkpath(fileInfo.path()))
    {
        QFile file(m_filePath);


        if (file.open(QIODevice::WriteOnly))
        {
            QJsonDocument jsonDocument(m_values);
            file.write(jsonDocument.toJson());
            file.close();
        }
    }
}

void LocalSettings::updateFilePath()
{
    QString basePath;
#ifndef PORTABLE
    basePath = QStandardPaths::writableLocation(QStandardPaths::ConfigLocation);
#else
    basePath = QDir::currentPath();
#endif
    m_filePath = QDir(basePath).filePath(m_application + "/" + m_name + ".json");
    emit filePathChanged(m_filePath);
}

void LocalSettings::save()
{
    saveSettings();
}

void LocalSettings::load()
{
    loadSettings();
}

QJsonValue LocalSettings::value(const QString &key)
{
    QStringList heritanceList;
    QJsonObject *parentObject;
    QList<QJsonObject> objectList;

    heritanceList = key.split('.');
    parentObject = &m_values;

    for (int i = 0; i < heritanceList.size(); ++i)
    {
        QString keyName = heritanceList.at(i);
        bool isValueKey = (i == (heritanceList.size() - 1));

        if (isValueKey)
        {
            return parentObject->value(keyName);
        }
        else
        {
            QJsonObject object;
            object = parentObject->value(keyName).toObject();

            if (object.isEmpty())
            {
                return QJsonValue();
            }
            else
            {
                objectList.append(object);
                parentObject = &objectList.last();
            }
        }
    }

    return QJsonValue();
}

void LocalSettings::setFilePath(QString arg)
{
    if (m_filePath == arg)
        return;

    m_filePath = arg;
    emit filePathChanged(arg);
}

void LocalSettings::setValue(const QString &key, const QJsonValue &value)
{
    setValue(key, value, true);
}

void LocalSettings::setValue(const QString &key, const QJsonValue &value, bool overwrite = true)
{
    QStringList heritanceList;
    QJsonObject *parentObject;
    QList<QJsonObject> objectList;

    heritanceList = key.split('.');
    parentObject = &m_values;

    for (int i = 0; i < heritanceList.size(); ++i)
    {
        QString keyName = heritanceList.at(i);
        QJsonValueRef valueRef = (*parentObject)[keyName];
        bool isValueKey = (i == (heritanceList.size() - 1));

        if (isValueKey)
        {
            if (!overwrite && !valueRef.isNull()) {
                continue;
            }

            valueRef = value;
        }
        else
        {
            if (!valueRef.isObject())
            {
                objectList.append(QJsonObject());
            }
            else
            {
                objectList.append(valueRef.toObject());
            }
            parentObject = &objectList.last();
        }
    }

    if (objectList.size() > 0)
    {
        for (int i = (objectList.size() - 1); i >= 1; --i)
        {
            objectList[i-1][heritanceList.at(i)] = objectList[i];
        }
        m_values[heritanceList.first()] = objectList.at(0);
    }

    emit valuesChanged(m_values);
}

void LocalSettings::setValues(QJsonObject arg)
{
    if (m_values == arg)
        return;

    m_values = arg;
    emit valuesChanged(arg);
}

void LocalSettings::setApplication(QString arg)
{
    if (m_application == arg)
        return;

    m_application = arg;
    emit applicatioChanged(arg);
    updateFilePath();
}

void LocalSettings::setName(QString arg)
{
    if (m_name == arg)
        return;

    m_name = arg;
    emit nameChanged(arg);
    updateFilePath();
}
}; // namespace qtquickvcp
