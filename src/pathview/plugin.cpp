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
#include "qpathview3d.h"
#include "qglitem.h"
#include "qglview.h"
#include "qglcubeitem.h"
#include "qglcylinderitem.h"
#include "qglgriditem.h"
#include "qglsphereitem.h"
#include "qglcamera.h"
#include "qglcoordinateitem.h"
#include "qglboundingboxitem.h"
#include "qglobjectdimensionsitem.h"
#include "qglpathitem.h"
#include "qgllight.h"
#include <qqml.h>

void MachinekitPathViewPlugin::registerTypes(const char *uri)
{
    // @uri Machinekit.PathView
    qmlRegisterType<QPathView3d>(uri, 1, 0, "PathView3D");
    qmlRegisterType<QGLCamera>(uri, 1, 0, "Camera3D");
    qmlRegisterType<QGLLight>(uri, 1, 0, "Light3D");
    qmlRegisterType<QGLView>(uri, 1, 0, "GLView3D");
    qmlRegisterType<QGLCubeItem>(uri, 1, 0, "Cube3D");
    qmlRegisterType<QGLCylinderItem>(uri, 1, 0, "Cylinder3D");
    qmlRegisterType<QGLSphereItem>(uri, 1, 0, "Sphere3D");
    qmlRegisterType<QGLGridItem>(uri, 1, 0, "Grid3D");
    qmlRegisterType<QGLCoordinateItem>(uri, 1, 0, "Coordintate3D");
    qmlRegisterType<QGLBoundingBoxItem>(uri, 1, 0, "BoundingBox3D");
    qmlRegisterType<QGLObjectDimensionsItem>(uri, 1, 0, "ObjectDimensions3D");
    qmlRegisterType<QGLPathItem>(uri, 1, 0, "Path3D");
}
