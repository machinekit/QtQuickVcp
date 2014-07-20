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
/****************************************************************************
**
** Copyright (C) 2013 Digia Plc and/or its subsidiary(-ies).
** Contact: http://www.qt-project.org/legal
**
** This file is part of the demonstration applications of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:LGPL$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and Digia.  For licensing terms and
** conditions see http://qt.digia.com/licensing.  For further information
** use the contact form at http://qt.digia.com/contact-us.
**
** GNU Lesser General Public License Usage
** Alternatively, this file may be used under the terms of the GNU Lesser
** General Public License version 2.1 as published by the Free Software
** Foundation and appearing in the file LICENSE.LGPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU Lesser General Public License version 2.1 requirements
** will be met: http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
**
** In addition, as a special exception, Digia gives you certain additional
** rights.  These rights are described in the Digia Qt LGPL Exception
** version 1.1, included in the file LGPL_EXCEPTION.txt in this package.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 3.0 as published by the Free Software
** Foundation and appearing in the file LICENSE.GPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU General Public License version 3.0 requirements will be
** met: http://www.gnu.org/copyleft/gpl.html.
**
**
** $QT_END_LICENSE$
**
****************************************************************************/

#ifndef SQUIRCLE_H
#define SQUIRCLE_H

#include <QtQuick/QQuickItem>
#include <QtGui/QOpenGLShaderProgram>
#include <QTimer>
#include <QOpenGLBuffer>

class PathView3d : public QQuickItem
{
    Q_OBJECT

    Q_PROPERTY(qreal t READ t WRITE setT NOTIFY tChanged)
    Q_ENUMS(ModelType)

public:
    PathView3d();

    typedef struct {
        GLfloat x;
        GLfloat y;
        GLfloat z;
    } Vector3D;

    typedef struct {
        Vector3D position;
        Vector3D normal;
    } ModelVertex;

    typedef struct {
        QMatrix4x4 modelMatrix;
        QColor color;
    } ModelState;

    enum ModelType {
        Cube = 0,
        Cylinder = 1,
        Sphere = 2
    };

    qreal t() const { return m_t; }
    void setT(qreal t);

signals:
    void tChanged();

public slots:
    void paint();
    void cleanup();
    void sync();

private slots:
    void handleWindowChanged(QQuickWindow *win);

private:
    // the shader program
    QOpenGLShaderProgram *m_program;

    // vertex buffers
    QOpenGLBuffer *m_cubeVertexBuffer;
    QOpenGLBuffer *m_cylinderVertexBuffer;

    // transformation matrices
    QMatrix4x4 m_viewMatrix;
    QMatrix4x4 m_modelMatrix;
    QMatrix4x4 m_projectionMatrix;

    // shader program location ids
    int m_positionLocation;
    int m_normalLocation;
    int m_colorLocation;
    int m_lightPosLocation;
    int m_projectionMatrixLocation;
    int m_viewMatrixLocation;
    int m_modelMatrixLocation;

    // thread secure properties
    qreal m_t;
    qreal m_thread_t;

    // setup functions
    void initializeVertexBuffer(QOpenGLBuffer **glBuffer, const QVector<ModelVertex> & vertices);
    void initializeVertexBuffer(QOpenGLBuffer **glBuffer, const void *bufferData, int bufferLength);
    void setupVBOs();
    void setupCube();
    void setupShaders();
    void setupWindow();
    void setupCylinder(GLfloat originRadius, QVector3D originPoint, GLfloat endRadius, QVector3D endPoint, int precision);

    void drawCube();
    void drawCube(QVector3D size);
    void drawCube(QVector3D size, QVector3D position);
    void drawCube(QVector3D size, QVector3D position, float rotationAngle, QVector3D rotationVector);
    void drawCylinder(float radius, float height, QVector3D position, float rotationAngle, QVector3D rotationVector);

    // model manipulation functions
    void color(float r, float g, float b, float a);
    void color(QColor color);
    void translate(float x, float y, float z);
    void translate(QVector3D vector);
    void rotate(float angle, float x, float y, float z);
    void rotate(float angle, QVector3D axis);
    void rotate(QQuaternion quaternion);
    void scale(float x, float y, float z);
    void scale(QVector3D vector);
    void mirror(float x, float y, float z);
    void mirror(QVector3D vector);

    // model functions
    void cube(float w, float l, float h, bool center = false);
    void cube(QVector3D size, bool center = false);
    void cylinder(float h, float r1);
    void cylinder(float h, float r1, float r2);
    void sphere(float r);

    // grouping functions
    void beginUnion();
    void endUnion();
};

#endif // SQUIRCLE_H
