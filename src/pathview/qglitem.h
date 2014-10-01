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

#ifndef QGLITEM_H
#define QGLITEM_H

#include <QObject>
#include "qglview.h"

class QGLView;

class QGLItem : public QQuickItem
{
    Q_OBJECT
    Q_PROPERTY(QVector3D position READ position WRITE setPosition NOTIFY positionChanged)
    Q_PROPERTY(QVector3D scale READ scale WRITE setScale NOTIFY scaleChanged)
    Q_PROPERTY(QQuaternion rotation READ rotation WRITE setRotation NOTIFY rotationChanged)
    Q_PROPERTY(float rotationAngle READ rotationAngle WRITE setRotationAngle NOTIFY rotationAngleChanged)
    Q_PROPERTY(QVector3D rotationAxis READ rotationAxis WRITE setRotationAxis NOTIFY rotationAxisChanged)

public:
    explicit QGLItem(QQuickItem *parent = 0);

    virtual void paint(QGLView *glView) = 0; // must be implemented

    QVector3D position() const
    {
        return m_position;
    }

    QVector3D scale() const
    {
        return m_scale;
    }

    QQuaternion rotation() const
    {
        return m_rotation;
    }

    float rotationAngle() const
    {
        return m_rotationAngle;
    }

    QVector3D rotationAxis() const
    {
        return m_rotationAxis;
    }

signals:
    void needsUpdate();
    void modelIdChanged(quint32 arg);
    void positionChanged(QVector3D arg);
    void scaleChanged(QVector3D arg);
    void rotationChanged(QQuaternion arg);
    void rotationAngleChanged(float arg);
    void rotationAxisChanged(QVector3D arg);

public slots:
    void requestPaint();
    virtual void selectDrawable(void *pointer) = 0; // must be implemented

    void setPosition(float x, float y, float z)
    {
        setPosition(QVector3D(x,y,z));
    }

    void setPosition(QVector3D arg)
    {
        if (m_position != arg) {
            m_position = arg;
            emit positionChanged(arg);
        }
    }

    void setScale(float x, float y, float z)
    {
        setScale(QVector3D(x,y,z));
    }

    void setScale(QVector3D arg)
    {
        if (m_scale != arg) {
            m_scale = arg;
            emit scaleChanged(arg);
        }
    }

    void setRotation(float angle, float x, float y, float z)
    {
        setRotation(angle, QVector3D(x,y,z));
    }

    void setRotation(float angle, QVector3D axis)
    {
        setRotation(QQuaternion(angle, axis));
    }

    void setRotation(QQuaternion arg)
    {
        if (m_rotation != arg) {
            m_rotation = arg;
            emit rotationChanged(arg);
        }
    }

    void setRotationAngle(float arg)
    {
        if (m_rotationAngle != arg) {
            m_rotationAngle = arg;
            emit rotationAngleChanged(arg);
            setRotation(QQuaternion::fromAxisAndAngle(m_rotationAxis, m_rotationAngle));
        }
    }

    void setRotationAxis(QVector3D arg)
    {
        if (m_rotationAxis != arg) {
            m_rotationAxis = arg;
            emit rotationAxisChanged(arg);
            setRotation(QQuaternion::fromAxisAndAngle(m_rotationAxis, m_rotationAngle));
        }
    }

private:
    QVector3D m_position;
    QVector3D m_scale;
    QQuaternion m_rotation;
    float m_rotationAngle;
    QVector3D m_rotationAxis;
};

#endif // QGLITEM_H
