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
#include "qpreviewclient.h"
#include "qglitem.h"
#include "qglview.h"
#include "qglcubeitem.h"
#include "qglcylinderitem.h"
#include "qglsphereitem.h"
#include "qglcamera.h"
#include "qglpathitem.h"
#include "qgllight.h"
#include "qglcanvas.h"
#include "qgcodeprogrammodel.h"
#include "qgcodeprogramloader.h"

static void initResources()
{
    Q_INIT_RESOURCE(pathview);
}

static const struct {
    const char *type;
    int major, minor;
} qmldir [] = {
    { "BoundingBox3D", 1, 0 },
    { "Coordinate3D", 1, 0 },
    { "GCodeSync", 1, 0 },
    { "Grid3D", 1, 0 },
    { "PathView3D", 1, 0 },
    { "PathViewCore", 1, 0 },
    { "PathViewObject", 1, 0 },
    { "ProgramExtents3D", 1, 0 },
    { "ProgramOffsets3D", 1, 0 },
    { "SourceView", 1, 0 },
    { "ViewModeAction", 1, 0 },
    { "ZoomInAction", 1, 0 },
    { "ZoomOutAction", 1, 0 },
    { "ZoomOriginalAction", 1, 0 }
};

void MachinekitPathViewPlugin::registerTypes(const char *uri)
{
    initResources();

    // @uri Machinekit.PathView
    Q_ASSERT(uri == QLatin1String("Machinekit.PathView"));
    qmlRegisterType<QGLCamera>(uri, 1, 0, "Camera3D");
    qmlRegisterType<QGLLight>(uri, 1, 0, "Light3D");
    qmlRegisterType<QGLView>(uri, 1, 0, "GLView3D");
    qmlRegisterType<QGLCubeItem>(uri, 1, 0, "Cube3D");
    qmlRegisterType<QGLCylinderItem>(uri, 1, 0, "Cylinder3D");
    qmlRegisterType<QGLSphereItem>(uri, 1, 0, "Sphere3D");
    qmlRegisterType<QGLPathItem>(uri, 1, 0, "Path3D");
    qmlRegisterType<QGLCanvas>(uri, 1, 0, "Canvas3D");
    qmlRegisterType<QPreviewClient>(uri, 1, 0, "PreviewClient");
    qmlRegisterType<QGCodeProgramModel>(uri, 1, 0, "GCodeProgramModel");
    qmlRegisterType<QGCodeProgramLoader>(uri, 1, 0, "GCodeProgramLoader");

    const QString filesLocation = fileLocation();
    for (int i = 0; i < int(sizeof(qmldir)/sizeof(qmldir[0])); i++) {
        qmlRegisterType(QUrl(filesLocation + "/" + qmldir[i].type + ".qml"), uri, qmldir[i].major, qmldir[i].minor, qmldir[i].type);
    }
}

void MachinekitPathViewPlugin::initializeEngine(QQmlEngine *engine, const char *uri)
{
    Q_UNUSED(uri);

    if (isLoadedFromResource())
        engine->addImportPath(QStringLiteral("qrc:/"));

    const char *private_uri = "Machinekit.PathView.Private";
    qmlRegisterUncreatableType<AbstractServiceImplementation>(private_uri, 1, 0, "AnstractServiceImplementation",
                                                              QLatin1String("Do not create objects of type AbstractServiceImplementation"));
}

QString MachinekitPathViewPlugin::fileLocation() const
{
    if (isLoadedFromResource())
        return "qrc:/Machinekit/PathView";
    return baseUrl().toString();
}

bool MachinekitPathViewPlugin::isLoadedFromResource() const
{
    // If one file is missing, it will load all the files from the resource
    QFile file(baseUrl().toLocalFile() + "/PathView3D.qml");
    if (!file.exists())
        return true;
    return false;
}
