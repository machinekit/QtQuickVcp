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
#include "qapplicationfilemodel.h"
#include "qapplicationlauncher.h"
#include "qapplicationplugins.h"
#include "qapplicationpluginitem.h"
#include "qlocalsettings.h"

static void initResources()
{
    Q_INIT_RESOURCE(application);
}

static const struct {
    const char *type;
    int major, minor;
} qmldir [] = {
    { "ApplicationAction", 1, 0 },
    { "ApplicationCore", 1, 0 },
    { "ApplicationHelper", 1, 0 },
    { "ApplicationItem", 1, 0 },
    { "ApplicationObject", 1, 0 },
    { "ApplicationSettings", 1, 0 },
    { "MdiHistory", 1, 0 },
    { "HomeAllAxesHelper", 1, 0 }
};

void MachinekitApplicationPlugin::registerTypes(const char *uri)
{
    initResources();

    // Verify that the version of the library that we linked against is
    // compatible with the version of the headers we compiled against.
    GOOGLE_PROTOBUF_VERIFY_VERSION;

    // @uri Machinekit.Application
    Q_ASSERT(uri == QLatin1String("Machinekit.Application"));
    qmlRegisterType<QApplicationConfig>(uri, 1, 0, "ApplicationConfig");
    qmlRegisterType<QApplicationConfigItem>(uri, 1, 0, "ApplicationConfigItem");
    qmlRegisterType<QApplicationConfigFilter>(uri, 1, 0, "ApplicationConfigFilter");
    qmlRegisterType<QApplicationDescription>(uri, 1, 0, "ApplicationDescription");
    qmlRegisterType<QApplicationStatus>(uri, 1, 0, "ApplicationStatus");
    qmlRegisterType<QApplicationCommand>(uri, 1, 0, "ApplicationCommand");
    qmlRegisterType<QApplicationError>(uri, 1, 0, "ApplicationError");
    qmlRegisterType<QApplicationFile>(uri, 1, 0, "ApplicationFile");
    qmlRegisterType<QApplicationFileModel>(uri, 1, 0, "ApplicationFileModel");
    qmlRegisterType<QApplicationLauncher>(uri, 1, 0, "ApplicationLauncher");
    qmlRegisterType<QLocalSettings>(uri, 1, 0, "LocalSettings");
    qmlRegisterType<QApplicationPlugins>(uri, 1, 0, "ApplicationPlugins");
    qmlRegisterType<QApplicationPluginItem>(uri, 1, 0, "ApplicationPluginItem");

    const QString filesLocation = fileLocation();
    for (int i = 0; i < int(sizeof(qmldir)/sizeof(qmldir[0])); i++) {
        qmlRegisterType(QUrl(filesLocation + "/" + qmldir[i].type + ".qml"), uri, qmldir[i].major, qmldir[i].minor, qmldir[i].type);
    }
}

void MachinekitApplicationPlugin::initializeEngine(QQmlEngine *engine, const char *uri)
{
    Q_UNUSED(uri);

    if (isLoadedFromResource())
        engine->addImportPath(QStringLiteral("qrc:/"));

    const char *private_uri = "Machinekit.Application.Private";
    qmlRegisterUncreatableType<AbstractServiceImplementation>(private_uri, 1, 0, "AnstractServiceImplementation",
                                                              QLatin1String("Do not create objects of type AbstractServiceImplementation"));
}

QString MachinekitApplicationPlugin::fileLocation() const
{
    if (isLoadedFromResource())
        return "qrc:/Machinekit/Application";
    return baseUrl().toString();
}

bool MachinekitApplicationPlugin::isLoadedFromResource() const
{
    // If one file is missing, it will load all the files from the resource
    QFile file(baseUrl().toLocalFile() + "/ApplicationCore.qml");
    if (!file.exists())
        return true;
    return false;
}


