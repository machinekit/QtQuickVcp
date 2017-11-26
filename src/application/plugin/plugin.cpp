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
#include "applicationconfig.h"
#include "applicationconfigitem.h"
#include "applicationconfigfilter.h"
#include "applicationdescription.h"
#include "applicationstatus.h"
#include "applicationcommand.h"
#include "applicationerror.h"
#include "applicationfile.h"
#include "applicationfilemodel.h"
#include "applicationlauncher.h"
#include "applicationplugins.h"
#include "applicationpluginitem.h"
#include "applicationtranslator.h"
#include "applicationlog.h"
#include "applicationlogmessage.h"
#include "localsettings.h"
#include "fileio.h"
#include "filewatcher.h"
#include "revisionsingleton.h"
#include "applicationhelpers.h"
#include "applicationfilesynchandler.h"

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
    { "HomeAllAxesHelper", 1, 0 },
    { "QueuedConnection",  1, 0 }
};

void MachinekitApplicationPlugin::registerTypes(const char *uri)
{
    initResources();

    // Verify that the version of the library that we linked against is
    // compatible with the version of the headers we compiled against.
    GOOGLE_PROTOBUF_VERIFY_VERSION;

    // @uri Machinekit.Application
    Q_ASSERT(uri == QLatin1String("Machinekit.Application"));
    qmlRegisterType<qtquickvcp::ApplicationConfig>(uri, 1, 0, "ApplicationConfig");
    qmlRegisterType<qtquickvcp::ApplicationConfigItem>(uri, 1, 0, "ApplicationConfigItem");
    qmlRegisterType<qtquickvcp::ApplicationConfigFilter>(uri, 1, 0, "ApplicationConfigFilter");
    qmlRegisterType<qtquickvcp::ApplicationDescription>(uri, 1, 0, "ApplicationDescription");
    qmlRegisterType<qtquickvcp::ApplicationStatus>(uri, 1, 0, "ApplicationStatus");
    qmlRegisterType<qtquickvcp::ApplicationCommand>(uri, 1, 0, "ApplicationCommand");
    qmlRegisterType<qtquickvcp::ApplicationError>(uri, 1, 0, "ApplicationError");
    qmlRegisterType<qtquickvcp::ApplicationFile>(uri, 1, 0, "ApplicationFile");
    qmlRegisterType<qtquickvcp::ApplicationFileModel>(uri, 1, 0, "ApplicationFileModel");
    qmlRegisterType<qtquickvcp::ApplicationLauncher>(uri, 1, 0, "ApplicationLauncher");
    qmlRegisterType<qtquickvcp::LocalSettings>(uri, 1, 0, "LocalSettings");
    qmlRegisterType<qtquickvcp::ApplicationPlugins>(uri, 1, 0, "ApplicationPlugins");
    qmlRegisterType<qtquickvcp::ApplicationPluginItem>(uri, 1, 0, "ApplicationPluginItem");
    qmlRegisterType<qtquickvcp::ApplicationTranslator>(uri, 1, 0, "ApplicationTranslator");
    qmlRegisterType<qtquickvcp::ApplicationLog>(uri, 1, 0, "ApplicationLog");
    qmlRegisterType<qtquickvcp::ApplicationLogMessage>(uri, 1, 0, "ApplicationLogMessage");
    qmlRegisterType<qtquickvcp::FileIO>(uri, 1, 0, "FileIO");
    qmlRegisterType<qtquickvcp::FileWatcher>(uri, 1, 0, "FileWatcher");
    qmlRegisterSingletonType<qtquickvcp::RevisionSingleton>(uri, 1, 0, "Revision", &qtquickvcp::RevisionSingleton::qmlInstance);
    qmlRegisterSingletonType<qtquickvcp::ApplicationHelpers>(uri, 1, 0, "ApplicationHelpers", &qtquickvcp::ApplicationHelpers::qmlSingletonProvider);
    qmlRegisterType<qtquickvcp::ApplicationFileSyncHandler>(uri, 1, 0, "ApplicationFileSyncHandler");

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


