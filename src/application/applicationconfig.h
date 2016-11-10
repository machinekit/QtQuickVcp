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
#ifndef APPLICATIONCONFIG_H
#define APPLICATIONCONFIG_H

#include <QQuickItem>
#include <QQmlListProperty>
#include <QCoreApplication>
#include <QDir>
#include <google/protobuf/text_format.h>
#include <machinetalk/protobuf/message.pb.h>
#include <machinetalk/protobuf/types.pb.h>
#include <application/configbase.h>
#include "applicationconfigitem.h"
#include "applicationconfigfilter.h"
#include "applicationdescription.h"

namespace qtquickvcp {

class ApplicationConfig : public application::ConfigBase
{
    Q_OBJECT
    Q_PROPERTY(bool synced READ isSynced NOTIFY syncedChanged)
    Q_PROPERTY(ApplicationConfigItem *selectedConfig READ selectedConfig NOTIFY selectedConfigChanged)
    Q_PROPERTY(QQmlListProperty<qtquickvcp::ApplicationConfigItem> configs READ configs NOTIFY configsChanged)
    Q_PROPERTY(ApplicationConfigFilter *filter READ filter WRITE setFilter NOTIFY filterChanged)

public:
    explicit ApplicationConfig(QQuickItem *parent = 0);
    ~ApplicationConfig();

    bool isSynced() const
    {
        return m_synced;
    }

    ApplicationConfigItem *selectedConfig() const
    {
        return m_selectedConfig;
    }

    ApplicationConfigFilter *filter() const
    {
        return m_filter;
    }

    QQmlListProperty<ApplicationConfigItem> configs()
    {
        return QQmlListProperty<ApplicationConfigItem>(this, m_configs);
    }
    int appConfigCount() const
    {
        return m_configs.count();
    }
    ApplicationConfigItem *appConfig(int index) const
    {
        return m_configs.at(index);
    }

public slots:
    void setSelectedConfig(ApplicationConfigItem * arg)
    {
        if (m_selectedConfig != arg) {
            m_selectedConfig = arg;
            emit selectedConfigChanged(arg);
        }
    }

    void setFilter(ApplicationConfigFilter * arg)
    {
        if (m_filter == arg)
            return;

        m_filter = arg;
        emit filterChanged(arg);
    }

    void selectConfig(QString name);
    void unselectConfig();

private:
    bool m_synced;

    ApplicationConfigItem *m_selectedConfig;
    QList<ApplicationConfigItem*> m_configs;
    ApplicationConfigFilter *m_filter;

    // more efficient to reuse a protobuf Message
    pb::Container m_tx;

    void cleanupFiles();

private slots:
    void describeApplicationReceived(pb::Container *rx);
    void applicationDetailReceived(pb::Container *rx);
    void errorReceived(pb::Container *rx);
    void syncConfig();
    void unsyncConfig();

signals:
    void selectedConfigChanged(ApplicationConfigItem * arg);
    void configsChanged(QQmlListProperty<ApplicationConfigItem> arg);
    void filterChanged(ApplicationConfigFilter * arg);
    void syncedChanged(bool synced);
}; // class ApplicationConfig
}; // namespace qtquickvcp

#endif // APPLICATIONCONFIG_H
