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
#include "qhalsignal.h"
#include "qhalgroup.h"
#include "qhalremotecomponent.h"
#include "qservice.h"
#include "qservicelist.h"
#include "qapplicationconfig.h"
#include "qapplicationconfigitem.h"
#include "qapplicationconfigfilter.h"
#include "qapplicationdescription.h"
#include "qservicediscovery.h"
#include "qservicediscoveryitem.h"
#include "qservicediscoveryfilter.h"
#include "qnameserver.h"
#include "qemcstatus.h"
#include "qemccommand.h"

#include <qqml.h>

void MachinekitHalRemotePlugin::registerTypes(const char *uri)
{
    // Verify that the version of the library that we linked against is
    // compatible with the version of the headers we compiled against.
    GOOGLE_PROTOBUF_VERIFY_VERSION;

    // @uri Machinekit.HalRemote
    qmlRegisterType<QService>(uri, 1, 0, "Service");
    qmlRegisterType<QServiceList>(uri, 1, 0, "ServiceList");
    qmlRegisterType<QNameServer>(uri, 1, 0, "NameServer");
    qmlRegisterType<QServiceDiscovery>(uri, 1, 0, "ServiceDiscovery");
    qmlRegisterType<QServiceDiscoveryItem>(uri, 1, 0, "ServiceDiscoveryItem");
    qmlRegisterType<QServiceDiscoveryFilter>(uri, 1, 0, "ServiceDiscoveryFilter");
    qmlRegisterType<QApplicationConfig>(uri, 1, 0, "ApplicationConfig");
    qmlRegisterType<QApplicationConfigItem>(uri, 1, 0, "ApplicationConfigItem");
    qmlRegisterType<QApplicationConfigFilter>(uri, 1, 0, "ApplicationConfigFilter");
    qmlRegisterType<QApplicationDescription>(uri, 1, 0, "ApplicationDescription");
    qmlRegisterType<QHalRemoteComponent>(uri, 1, 0, "HalRemoteComponent");
    qmlRegisterType<QHalPin>(uri, 1, 0, "HalPin");
    qmlRegisterType<QHalGroup>(uri, 1, 0, "HalGroup");
    qmlRegisterType<QHalSignal>(uri, 1, 0, "HalSignal");
    qmlRegisterType<QEmcStatus>(uri, 1, 0, "EmcStatus");
    qmlRegisterType<QEmcCommand>(uri, 1, 0, "EmcCommand");
}


