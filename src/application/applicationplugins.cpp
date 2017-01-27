/****************************************************************************
**
** Copyright (C) 2015 Alexander Rössler
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
** Alexander Rössler <mail AT rossler DOT systems>
**
****************************************************************************/
#include "applicationplugins.h"
#include <QDir>
#include <QDirIterator>
#include <QSettings>

namespace qtquickvcp {

ApplicationPlugins::ApplicationPlugins(QObject *parent) :
    QObject(parent)
{

}

QQmlListProperty<ApplicationPluginItem> ApplicationPlugins::plugins()
{
    return QQmlListProperty<ApplicationPluginItem>(this, m_plugins);
}

int ApplicationPlugins::pluginCount() const
{
    return m_plugins.count();
}

ApplicationPluginItem *ApplicationPlugins::plugin(int index) const
{
    return m_plugins.at(index);
}

void ApplicationPlugins::readPluginFile(QString filePath)
{
    QSettings ini(filePath, QSettings::IniFormat);
    QFileInfo info(filePath);
    ApplicationPluginItem *item = new ApplicationPluginItem(this);
    QString typeString;
    QString name;
    QString description;
    QUrl mainFile;
    ApplicationPluginItem::PluginType type = ApplicationPluginItem::Qt5QmlPlugin;

    ini.beginGroup("Default");
    name = ini.value("name", "Unnamed").toString();
    description = ini.value("description", "").toString();
    typeString = ini.value("type", "QT5_QML").toString();
    ini.endGroup();
    if (typeString == "QT5_QML")
    {
        type = ApplicationPluginItem::Qt5QmlPlugin;
    }
    else if (typeString == "PYTHON")
    {
        type = ApplicationPluginItem::PythonPlugin;
    }
    mainFile = QUrl("file://" + info.path() + "/" + name + ".qml");

    item->setName(name);
    item->setDescription(description);
    item->setType(type);
    item->setMainFile(mainFile);
    m_plugins.append(item);
}

void ApplicationPlugins::updatePlugins()
{
    qDeleteAll(m_plugins);
    m_plugins.clear();

    foreach(QString path, m_searchPaths) {
        if (!QDir(path).exists())
        {
            continue;
        }

        QDirIterator dirIterator(path, QDirIterator::FollowSymlinks | QDirIterator::Subdirectories);
        while (dirIterator.hasNext())
        {
            dirIterator.next();
            QFileInfo info = dirIterator.fileInfo();
            if (info.isFile() && info.fileName() == "plugin.ini")
            {
                readPluginFile(info.filePath());
            }
        }
    }

    emit pluginsChanged(QQmlListProperty<ApplicationPluginItem>(this, m_plugins));
}

void ApplicationPlugins::clearPlugins()
{
    qDeleteAll(m_plugins);
    m_plugins.clear();

    emit pluginsChanged(QQmlListProperty<ApplicationPluginItem>(this, m_plugins));
}
} // namespace qtquickvcp
