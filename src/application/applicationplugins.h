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
#ifndef APPLICATIONPLUGINS_H
#define APPLICATIONPLUGINS_H

#include <QObject>
#include <QQmlListProperty>

#include "applicationpluginitem.h"

namespace qtquickvcp {

class ApplicationPlugins : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QQmlListProperty<qtquickvcp::ApplicationPluginItem> plugins READ plugins NOTIFY pluginsChanged)
    Q_PROPERTY(QStringList searchPaths READ searchPaths WRITE setSearchPaths NOTIFY searchPathsChanged)

public:
    ApplicationPlugins(QObject *parent = 0);

    QQmlListProperty<ApplicationPluginItem> plugins();
    int pluginCount() const;
    ApplicationPluginItem *plugin(int index) const;

    QStringList searchPaths() const
    {
        return m_searchPaths;
    }

public slots:
    void updatePlugins();
    void clearPlugins();

    void setSearchPaths(const QStringList &searchPaths)
    {
        if (m_searchPaths == searchPaths)
            return;

        m_searchPaths = searchPaths;
        emit searchPathsChanged(searchPaths);
    }

private:
    QList<ApplicationPluginItem*> m_plugins;
    QStringList m_searchPaths;

    void readPluginFile(QString filePath);

signals:
    void searchPathsChanged(const QStringList &searchPaths);
    void pluginsChanged(QQmlListProperty<ApplicationPluginItem> arg);
}; // class ApplicationPlugin
} // namespace qtquickvcp

#endif // APPLICATIONPLUGINS_H
