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
#ifndef APPLICATIONPLUGINITEM_H
#define APPLICATIONPLUGINITEM_H

#include <QObject>
#include <QUrl>

namespace qtquickvcp {

class ApplicationPluginItem : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString name READ name WRITE setName NOTIFY nameChanged)
    Q_PROPERTY(QString description READ description WRITE setDescription NOTIFY descriptionChanged)
    Q_PROPERTY(PluginType type READ type WRITE setType NOTIFY typeChanged)
    Q_PROPERTY(QUrl mainFile READ mainFile WRITE setMainFile NOTIFY mainFileChanged)

public:
    explicit ApplicationPluginItem(QObject *parent = 0);

    enum PluginType {
        Qt5QmlPlugin = 0,
        PythonPlugin = 1
    };

    QString name() const
    {
        return m_name;
    }

    QString description() const
    {
        return m_description;
    }

    PluginType type() const
    {
        return m_type;
    }

    QUrl mainFile() const
    {
        return m_mainFile;
    }

public slots:
    void setName(QString name)
    {
        if (m_name == name)
            return;

        m_name = name;
        emit nameChanged(name);
    }
    void setDescription(QString description)
    {
        if (m_description == description)
            return;

        m_description = description;
        emit descriptionChanged(description);
    }
    void setType(PluginType type)
    {
        if (m_type == type)
            return;

        m_type = type;
        emit typeChanged(type);
    }
    void setMainFile(QUrl mainFile)
    {
        if (m_mainFile == mainFile)
            return;

        m_mainFile = mainFile;
        emit mainFileChanged(mainFile);
    }

private:
    QString m_name;
    QString m_description;
    PluginType m_type;
    QUrl m_mainFile;

signals:
    void nameChanged(QString name);
    void descriptionChanged(QString description);
    void typeChanged(PluginType type);
    void mainFileChanged(QUrl mainFile);
}; // class ApplicationPluginItem
} // namespace qtquickvcp

#endif // APPLICATIONPLUGINITEM_H
