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
** Alexander Rössler @ The Cool Tool GmbH <mail DOT aroessler AT gmail DOT com>
**
****************************************************************************/
#ifndef APPLICATIONLAUNCHER_H
#define APPLICATIONLAUNCHER_H

#include <QObject>
#include <QJsonValue>
#include <QTemporaryDir>
#include <memory>
#include <machinetalk/protobuf/message.pb.h>
#include <application/launcherbase.h>

namespace qtquickvcp {

class ApplicationLauncher : public machinetalk::application::LauncherBase
{
    Q_OBJECT
    Q_PROPERTY(QVariant launchers READ launchers NOTIFY launchersChanged)
    Q_PROPERTY(bool synced READ isSynced NOTIFY syncedChanged)

public:
    explicit ApplicationLauncher(QObject *parent = 0);
    ~ApplicationLauncher();

    QVariant launchers() const;
    bool isSynced() const;

public slots:
    void start(int index);
    void terminate(int index);
    void kill(int index);
    void writeToStdin(int index, const QString &data);
    void call(const QString &command);
    void shutdown();
    void setImportance(int index, int importance);

private:
    QObject* m_launcherObject;
    bool m_synced;

    std::unique_ptr<QTemporaryDir> m_temporaryDir;
    // more efficient to reuse a protobuf Message
    machinetalk::Container   m_tx;

    void initializeObject();
    void createTemporaryDir();

private slots:
    void handleLauncherFullUpdateMessage(const QByteArray &topic, const machinetalk::Container &rx);
    void handleLauncherIncrementalUpdateMessage(const QByteArray &topic, const machinetalk::Container &rx);
    void syncStatus();
    void unsyncStatus();

signals:
    void launchersChanged();
    void syncedChanged(bool arg);
}; // class ApplicationLauncher
} // namespace qtquickvcp

#endif // APPLICATIONLAUNCHER_H
