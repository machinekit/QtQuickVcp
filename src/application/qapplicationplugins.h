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
#ifndef QAPPLICATIONPLUGINS_H
#define QAPPLICATIONPLUGINS_H

#include <QQuickItem>
#include <QQmlListProperty>
#include <QDir>
#include <QDirIterator>
#include <QSettings>
#include "qapplicationpluginitem.h"

class QApplicationPlugins : public QQuickItem
{
    Q_OBJECT
    Q_PROPERTY(QQmlListProperty<QApplicationPluginItem> plugins READ plugins NOTIFY pluginsChanged)
    Q_PROPERTY(QStringList searchPaths READ searchPaths WRITE setSearchPaths NOTIFY searchPathsChanged)

public:
    QApplicationPlugins(QQuickItem *parent = 0);

    QQmlListProperty<QApplicationPluginItem> plugins();
    int pluginCount() const;
    QApplicationPluginItem *plugin(int index) const;

    QStringList searchPaths() const
    {
        return m_searchPaths;
    }

public slots:
    void updatePlugins();
    void clearPlugins();

    void setSearchPaths(QStringList searchPaths)
    {
        if (m_searchPaths == searchPaths)
            return;

        m_searchPaths = searchPaths;
        emit searchPathsChanged(searchPaths);
    }

private:
    QList<QApplicationPluginItem*> m_plugins;
    QStringList m_searchPaths;

    void readPluginFile(QString filePath);

signals:
    void searchPathsChanged(QStringList searchPaths);
    void pluginsChanged(QQmlListProperty<QApplicationPluginItem> arg);
};

#endif // QAPPLICATIONPLUGINS_H
