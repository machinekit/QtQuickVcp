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

#include "glsphereitem.h"

namespace qtquickvcp {

GLSphereItem::GLSphereItem(QQuickItem *parent) :
    GLItem(parent),
    m_spherePointer(nullptr),
    m_radius(1.0),
    m_color(QColor(Qt::yellow)),
    m_selected(false)
{
    connect(this, &GLSphereItem::radiusChanged,
            this, &GLSphereItem::needsUpdate);
    connect(this, &GLSphereItem::colorChanged,
            this, &GLSphereItem::needsUpdate);
}

void GLSphereItem::paint(GLView *glView)
{
    glView->prepare(this);

    glView->reset();
    glView->beginUnion();
    glView->color(m_color);
    m_spherePointer = glView->sphere(m_radius);
    glView->endUnion();
}

void GLSphereItem::selectDrawable(void *pointer)
{
    bool selected;

    if (m_spherePointer == nullptr)
    {
        return;
    }

    selected = (pointer == m_spherePointer);

    if (selected != m_selected)
    {
        m_selected = selected;
        emit selectedChanged(m_selected);
    }
}
}; // namespace qtquickvcp
