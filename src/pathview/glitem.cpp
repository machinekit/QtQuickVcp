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
    m_rotationAngle(0.0),
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

QVector3D GLItem::position() const
{
    return m_position;
}

QVector3D GLItem::scale() const
{
    return m_scale;
}

QQuaternion GLItem::rotation() const
{
    return m_rotation;
}

float GLItem::rotationAngle() const
{
    return m_rotationAngle;
}

QVector3D GLItem::rotationAxis() const
{
    return m_rotationAxis;
}

void GLItem::requestPaint()
{
    emit needsUpdate();
}

void GLItem::setPosition(float x, float y, float z)
{
    setPosition(QVector3D(x,y,z));
}

void GLItem::setPosition(const QVector3D &arg)
{
    if (m_position != arg) {
        m_position = arg;
        emit positionChanged(arg);
    }
}

void GLItem::setScale(float x, float y, float z)
{
    setScale(QVector3D(x,y,z));
}

void GLItem::setScale(const QVector3D &arg)
{
    if (m_scale != arg) {
        m_scale = arg;
        emit scaleChanged(arg);
    }
}

void GLItem::setRotation(float angle, float x, float y, float z)
{
    setRotation(angle, QVector3D(x,y,z));
}

void GLItem::setRotation(float angle, const QVector3D &axis)
{
    setRotation(QQuaternion(angle, axis));
}

void GLItem::setRotation(const QQuaternion &arg)
{
    if (m_rotation != arg) {
        m_rotation = arg;
        emit rotationChanged(arg);
    }
}

void GLItem::setRotationAngle(float arg)
{
    if (m_rotationAngle != arg) {
        m_rotationAngle = arg;
        emit rotationAngleChanged(arg);
        setRotation(QQuaternion::fromAxisAndAngle(m_rotationAxis, m_rotationAngle));
    }
}

void GLItem::setRotationAxis(const QVector3D &arg)
{
    if (m_rotationAxis != arg) {
        m_rotationAxis = arg;
        emit rotationAxisChanged(arg);
        setRotation(QQuaternion::fromAxisAndAngle(m_rotationAxis, m_rotationAngle));
    }
}

void GLItem::setRotationVector(const QVector3D &rotationVector)
{
    if (m_rotationVector == rotationVector) {
        return;
    }

    m_rotationVector = rotationVector;
    emit rotationVectorChanged(m_rotationVector);
    setRotation(QQuaternion::fromEulerAngles(m_rotationVector));
}

QVector3D qtquickvcp::GLItem::rotationVector() const
{
    return m_rotationVector;
}

} // namespace qtquickvcp
