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

#ifndef GLITEM_H
#define GLITEM_H

#include <QObject>
#include "glview.h"

namespace qtquickvcp {

class GLView;

class GLItem : public QQuickItem
{
    Q_OBJECT
    Q_PROPERTY(QVector3D position READ position WRITE setPosition NOTIFY positionChanged)
    Q_PROPERTY(QVector3D scale READ scale WRITE setScale NOTIFY scaleChanged)
    Q_PROPERTY(QQuaternion rotation READ rotation WRITE setRotation NOTIFY rotationChanged)
    Q_PROPERTY(float rotationAngle READ rotationAngle WRITE setRotationAngle NOTIFY rotationAngleChanged)
    Q_PROPERTY(QVector3D rotationAxis READ rotationAxis WRITE setRotationAxis NOTIFY rotationAxisChanged)

public:
    explicit GLItem(QQuickItem *parent = 0);

    virtual void paint(GLView *glView) = 0; // must be implemented

    QVector3D position() const;
    QVector3D scale() const;
    QQuaternion rotation() const;
    float rotationAngle() const;
    QVector3D rotationAxis() const;

signals:
    void needsUpdate();
    void modelIdChanged(quint32 arg);
    void positionChanged(const QVector3D &arg);
    void scaleChanged(const QVector3D &arg);
    void rotationChanged(const QQuaternion &arg);
    void rotationAngleChanged(float arg);
    void rotationAxisChanged(const QVector3D &arg);

public slots:
    void requestPaint();
    virtual void selectDrawable(void *pointer) = 0; // must be implemented

    void setPosition(float x, float y, float z);
    void setPosition(const QVector3D &arg);
    void setScale(float x, float y, float z);
    void setScale(const QVector3D &arg);
    void setRotation(float angle, float x, float y, float z);
    void setRotation(float angle, const QVector3D &axis);
    void setRotation(const QQuaternion &arg);
    void setRotationAngle(float arg);
    void setRotationAxis(const QVector3D &arg);

private:
    QVector3D m_position;
    QVector3D m_scale;
    QQuaternion m_rotation;
    float m_rotationAngle;
    QVector3D m_rotationAxis;
}; // class GLItem
} // namespace qtquickvcp

#endif // GLITEM_H
