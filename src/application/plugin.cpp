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
#include "qapplicationconfig.h"
#include "qapplicationconfigitem.h"
#include "qapplicationconfigfilter.h"
#include "qapplicationdescription.h"
#include "qapplicationstatus.h"
#include "qapplicationcommand.h"
#include "qapplicationerror.h"
#include "qapplicationfile.h"

#include <qqml.h>

void MachinekitApplicationPlugin::registerTypes(const char *uri)
{
    // Verify that the version of the library that we linked against is
    // compatible with the version of the headers we compiled against.
    GOOGLE_PROTOBUF_VERIFY_VERSION;

    // @uri Machinekit.Application
    qmlRegisterType<QApplicationConfig>(uri, 1, 0, "ApplicationConfig");
    qmlRegisterType<QApplicationConfigItem>(uri, 1, 0, "ApplicationConfigItem");
    qmlRegisterType<QApplicationConfigFilter>(uri, 1, 0, "ApplicationConfigFilter");
    qmlRegisterType<QApplicationDescription>(uri, 1, 0, "ApplicationDescription");
    qmlRegisterType<QApplicationStatus>(uri, 1, 0, "ApplicationStatus");
    qmlRegisterType<QApplicationCommand>(uri, 1, 0, "ApplicationCommand");
    qmlRegisterType<QApplicationError>(uri, 1, 0, "ApplicationError");
    qmlRegisterType<QApplicationFile>(uri, 1, 0, "ApplicationFile");
}


