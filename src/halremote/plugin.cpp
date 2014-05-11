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
#include "qhalpin.h"
#include "qhalremotecomponent.h"
#include "qservice.h"
#include "qservicediscovery.h"
#include "qappconfig.h"
#include "qappconfigitem.h"
#include "qappconfigfilter.h"
#include "qappdiscovery.h"
#include "qappdiscoveryitem.h"

#include <qqml.h>

void MachinekitHalRemotePlugin::registerTypes(const char *uri)
{
    // Verify that the version of the library that we linked against is
    // compatible with the version of the headers we compiled against.
    GOOGLE_PROTOBUF_VERIFY_VERSION;

    // @uri Machinekit.HalRemote
    qmlRegisterType<QService>(uri, 1, 0, "Service");
    qmlRegisterType<QServiceDiscovery>(uri, 1, 0, "ServiceDiscovery");
    qmlRegisterType<QHalRemoteComponent>(uri, 1, 0, "HalRemoteComponent");
    qmlRegisterType<QAppConfig>(uri, 1, 0, "AppConfig");
    qmlRegisterType<QAppConfigItem>(uri, 1, 0, "AppConfigItem");
    qmlRegisterType<QAppConfigFilter>(uri, 1, 0, "AppConfigFilter");
    qmlRegisterType<QAppDiscovery>(uri, 1, 0, "AppDiscovery");
    qmlRegisterType<QAppDiscoveryItem>(uri, 1, 0, "AppDiscoveryItem");
    qmlRegisterType<QHalPin>(uri, 1, 0, "HalPin");
}


