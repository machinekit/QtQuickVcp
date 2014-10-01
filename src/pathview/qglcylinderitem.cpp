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

#include "qglcylinderitem.h"

QGLCylinderItem::QGLCylinderItem(QQuickItem *parent) :
    QGLItem(parent),
    m_cylinderPointer(NULL),
    m_radius(1.0),
    m_height(1.0),
    m_color(QColor(Qt::yellow)),
    m_cone(false),
    m_selected(false)
{
    connect(this, SIGNAL(radiusChanged(float)),
            this, SIGNAL(needsUpdate()));
    connect(this, SIGNAL(heightChanged()),
            this, SIGNAL(needsUpdate()));
    connect(this, SIGNAL(colorChanged(QColor)),
            this, SIGNAL(needsUpdate()));
    connect(this, SIGNAL(coneChanged(bool)),
            this, SIGNAL(needsUpdate()));
}

void QGLCylinderItem::paint(QGLView *glView)
{
    glView->prepare(this);\

    glView->reset();
    glView->beginUnion();
    glView->color(m_color);
    if (!m_cone)
    {
        m_cylinderPointer = glView->cylinder(m_radius, m_height);
    }
    else
    {
        m_cylinderPointer = glView->cone(m_radius, m_height);
    }
    glView->endUnion();
}

void QGLCylinderItem::selectDrawable(void *pointer)
{
    bool selected;

    if (m_cylinderPointer == NULL)
    {
        return;
    }

    selected = (pointer == m_cylinderPointer);

    if (selected != m_selected)
    {
        m_selected = selected;
        emit selectedChanged(m_selected);
    }
}
