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

#include "glitem.h"

namespace qtquickvcp {

GLItem::GLItem(QQuickItem *parent) :
    QQuickItem(parent),
    m_position(QVector3D(0,0,0)),
    m_scale(QVector3D(1,1,1)),
    m_rotation(QQuaternion()),
    m_rotationAngle(0),
    m_rotationAxis(QVector3D())
{
    connect(this, &GLItem::positionChanged,
            this, &GLItem::needsUpdate);
    connect(this, &GLItem::scaleChanged,
            this, &GLItem::needsUpdate);
    connect(this, &GLItem::rotationChanged,
            this, &GLItem::needsUpdate);
    connect(this, &GLItem::rotationAngleChanged,
            this, &GLItem::needsUpdate);
    connect(this, &GLItem::rotationAxisChanged,
            this, &GLItem::needsUpdate);
    connect(this, &GLItem::visibleChanged,
            this, &GLItem::needsUpdate);
}

void GLItem::requestPaint()
{
    emit needsUpdate();
}
} // namespace qtquickvcp
