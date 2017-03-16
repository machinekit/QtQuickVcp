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
#include "applicationdescription.h"
#include <QSettings>
#include <QDir>

namespace qtquickvcp {

/*!
    \qmltype ApplicationDescription
    \instantiates QApplicationDescription
    \inqmlmodule Machinekit.HalRemote
    \brief Application description
    \ingroup halremote

    This component loads the description of an application
    and determines the main QML file.

    \qml
    ApplicationDescription {
        sourceDir: "qrc:/AppDiscover.ServiceDisplay/"
    }
    \endqml
*/

/*! \qmlproperty string ApplicationDescription::sourceDir

    This property holds the source directory of the application.
*/

/*! \qmlproperty string ApplicationDescription::mainFile

    This property holds the main file of the application.
*/

/*! \qmlproperty string ApplicationDescription::translationsPath

    This property holds the translations path of the application.
*/

/*! \qmlproperty string ApplicationDescription::name

    This property holds the name of the application.
*/

/*! \qmlproperty string ApplicationDescription::description

    This property holds the description of the application.
*/

/*! \qmlproperty string ApplicationDescription::valid

    This property holds wheter the application configuration is valid or not.
*/

ApplicationDescription::ApplicationDescription(QObject *parent) :
    QObject(parent),
    m_componentCompleted(false),
    m_sourceDir(""),
    m_mainFile(""),
    m_name(""),
    m_description(""),
    m_valid(false)
{
}

bool ApplicationDescription::refresh()
{
    const QString iniName = "description.ini";
    QString sourcePath;
    QStringList fileList;
    QString baseFilePath;
    QString name;
    QString description;
    QString mainFile;
    QString translationsPath;

    // for local operation we must convert the url
    if (m_sourceDir.isLocalFile())
    {
        sourcePath = m_sourceDir.toLocalFile();
    }
    else
    {
        sourcePath = m_sourceDir.toString(QUrl::RemoveScheme);
    }

    if (m_sourceDir.scheme() == "qrc")  // if qrc we must prepend the :
    {
        sourcePath.prepend(":");
    }

    QDir sourceDir(sourcePath);

    if (!sourceDir.exists())    // the source directory is wrong
    {
        return false;
    }

    baseFilePath = sourceDir.path() + "/";

    fileList = sourceDir.entryList(QDir::Files);

    if ((fileList.count() < 2) || !fileList.contains(iniName, Qt::CaseSensitive)) // minimum of 2 files and ini file
    {
        return false;
    }
    else
    {
        fileList.removeAll(iniName);   // remove the ini file from the list
    }

    for (int i = 0; i < fileList.size(); ++i)
    {
        fileList[i].prepend(baseFilePath);
    }

    QSettings settings(baseFilePath + iniName, QSettings::IniFormat);

    name = settings.value("Default/name", QString("")).toString();
    description = settings.value("Default/description", QString("")).toString();
    mainFile = settings.value("Default/mainFile", QString("")).toString();
    translationsPath = settings.value("Default/translationsPath", QString("")).toString();

    if (name.isEmpty() || description.isEmpty())
    {
        return false;
    }

    if (mainFile.isEmpty())
    {
        mainFile = getMainFile(fileList, baseFilePath, name);   // if no main file is specified we try to figure it out
    }

    if (translationsPath.isEmpty())
    {
        translationsPath = getTranslationsPath(baseFilePath);
    }

    if (m_sourceDir.isLocalFile())
    {
        m_mainFile = QUrl::fromLocalFile(mainFile);
        m_translationsPath = QUrl::fromLocalFile(translationsPath);
    }
    else
    {
        m_mainFile = QUrl(m_sourceDir.scheme() + mainFile);
        m_translationsPath = QUrl(m_sourceDir.scheme() + translationsPath);
    }

    m_name = name;
    m_description = description;
    emit nameChanged(name);
    emit descriptionChanged(description);
    emit mainFileChanged(mainFile);
    emit translationsPathChanged(translationsPath);

    return true;
}

QString ApplicationDescription::getMainFile(const QStringList &fileList, const QString &baseFilePath, const QString &applicationName) const
{
    QString mainFileName;
    QString preferredName;

    // Number 1 priority main QML file has name main.qml
    preferredName = baseFilePath + "main.qml";
    for (const QString &fileName: fileList)
    {
        if (fileName == preferredName)
        {
            mainFileName = preferredName;
            break;
        }
    }

    // Number 2 priority main QML file is named like the config name
    if (mainFileName.isEmpty())
    {
        preferredName = baseFilePath + applicationName + ".qml";
        for (const QString &fileName: fileList)
        {
            if (fileName == preferredName)
            {
                mainFileName = preferredName;
                break;
            }
        }
    }

    // Number 3 priority main QML file is the first one of the file list
    if (mainFileName.isEmpty() && (fileList.size() > 0))
    {
        mainFileName = fileList.at(0);
    }

    return mainFileName;
}

QString ApplicationDescription::getTranslationsPath(const QString &baseFilePath) const
{
    // translations have to be placed in translations dir
    return baseFilePath + "translations";
}

} // namespace qtquickvcp
