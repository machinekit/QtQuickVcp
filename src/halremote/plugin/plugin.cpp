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
#include "plugin.h"
#include "halpin.h"
#include "halsignal.h"
#include "halremotecomponent.h"

void MachinekitHalRemotePlugin::registerTypes(const char *uri)
{
    // Verify that the version of the library that we linked against is
    // compatible with the version of the headers we compiled against.
    GOOGLE_PROTOBUF_VERIFY_VERSION;

    // @uri Machinekit.HalRemote
    Q_ASSERT(uri == QLatin1String("Machinekit.HalRemote"));
    qmlRegisterType<qtquickvcp::HalRemoteComponent>(uri, 1, 0, "HalRemoteComponent");
    qmlRegisterType<qtquickvcp::HalPin>(uri, 1, 0, "HalPin");
    qmlRegisterType<qtquickvcp::HalSignal>(uri, 1, 0, "HalSignal");
}

void MachinekitHalRemotePlugin::initializeEngine(QQmlEngine *engine, const char *uri)
{
    Q_UNUSED(uri);
    Q_UNUSED(engine);
}


