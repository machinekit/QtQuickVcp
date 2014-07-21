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

#include "qglview.h"

#include <QtQuick/qquickwindow.h>
#include <QtGui/QOpenGLShaderProgram>
#include <QtGui/QOpenGLContext>
#include <QtCore/qmath.h>
#include <QDateTime>

QGLView::QGLView()
    : m_program(0)
    , m_t(0)
    , m_thread_t(0)
{
    connect(this, SIGNAL(windowChanged(QQuickWindow*)), this, SLOT(handleWindowChanged(QQuickWindow*)));
}

void QGLView::setT(qreal t)
{
    if (t == m_t)
        return;
    m_t = t;
    emit tChanged();
    if (window())
        window()->update();
}

void QGLView::readPixel(int x, int y)
{
    GLubyte pixel[3];
    quint32 id;
    glReadPixels(x,y,1,1,GL_RGB,GL_UNSIGNED_BYTE, pixel);
    id = ((quint32)pixel[0] << 16) + ((quint32)pixel[1] << 8) + (quint32)pixel[2];
    qDebug() << id;
}

void QGLView::handleWindowChanged(QQuickWindow *win)
{
    if (win) {
        // Connect the beforeRendering signal to our paint function.
        // Since this call is executed on the rendering thread it must be
        // a Qt::DirectConnection
        connect(win, SIGNAL(beforeRendering()), this, SLOT(paint()), Qt::DirectConnection);
        connect(win, SIGNAL(beforeSynchronizing()), this, SLOT(sync()), Qt::DirectConnection);
        connect(win, SIGNAL(frameSwapped()), win, SLOT(update()));  // repaint every frame

        // If we allow QML to do the clearing, they would clear what we paint
        // and nothing would show.
        win->setClearBeforeRendering(false);
    }
}

void QGLView::clearModels(QGLView::ModelType type = NoType)
{
    if (type == NoType)
    {
        QMapIterator<ModelType, QList<ModelState>* > i(m_modelMap);
        while (i.hasNext()) {
            i.next();
            i.value()->clear();
        }
    }
    else
    {
        m_modelMap[type]->clear();
    }
}

void QGLView::drawModels(QGLView::ModelType type = NoType)
{
    if (type == NoType)
    {
        QMapIterator<ModelType, QList<ModelState>* > i(m_modelMap);
        while (i.hasNext()) {
            i.next();
            drawModelVertices(i.key());
        }
    }
    else
    {
        drawModelVertices(type);
    }
}

void QGLView::initializeVertexBuffer(ModelType type, const QVector<ModelVertex> &vertices)
{
    initializeVertexBuffer(type, vertices.data(), vertices.length() * sizeof(ModelVertex));
}

void QGLView::initializeVertexBuffer(ModelType type, const void *bufferData, int bufferLength)
{
    QOpenGLBuffer *glBuffer = new QOpenGLBuffer(QOpenGLBuffer::VertexBuffer);
    glBuffer->create();
    glBuffer->setUsagePattern(QOpenGLBuffer::StaticDraw);
    glBuffer->bind();
    glBuffer->allocate(bufferData, bufferLength);
    glBuffer->release();

    m_vertexBufferMap.insert(type, glBuffer);
}

void QGLView::setupVBOs()
{
    setupCube();
    setupCylinder(1.0, QVector3D(0,0,0),
                  1.0, QVector3D(0,0,1),
                  16, Cylinder);
    setupCylinder(1.0, QVector3D(0,0,0),
                  0.0, QVector3D(0,0,1),
                  16, Cone);
    setupSphere(16);
}

void QGLView::setupCube()
{
    static const ModelVertex vertices[] = {
        // Front face
        {{0.0f, 1.0f, 1.0f}, {0.0f, 0.0f, 1.0f}},
        {{0.0f, 0.0f, 1.0f}, {0.0f, 0.0f, 1.0f}},
        {{1.0f, 1.0f, 1.0f}, {0.0f, 0.0f, 1.0f}},
        {{0.0f, 0.0f, 1.0f}, {0.0f, 0.0f, 1.0f}},
        {{1.0f, 0.0f, 1.0f}, {0.0f, 0.0f, 1.0f}},
        {{1.0f, 1.0f, 1.0f}, {0.0f, 0.0f, 1.0f}},

        // Right face
        {{1.0f, 1.0f, 1.0f}, {1.0f, 0.0f, 0.0f}},
        {{1.0f, 0.0f, 1.0f}, {1.0f, 0.0f, 0.0f}},
        {{1.0f, 1.0f, 0.0f}, {1.0f, 0.0f, 0.0f}},
        {{1.0f, 0.0f, 1.0f}, {1.0f, 0.0f, 0.0f}},
        {{1.0f, 0.0f, 0.0f}, {1.0f, 0.0f, 0.0f}},
        {{1.0f, 1.0f, 0.0f}, {1.0f, 0.0f, 0.0f}},

        // Back face
        {{1.0f, 1.0f, 0.0f}, {0.0f, 0.0f, -1.0f}},
        {{1.0f, 0.0f, 0.0f}, {0.0f, 0.0f, -1.0f}},
        {{0.0f, 1.0f, 0.0f}, {0.0f, 0.0f, -1.0f}},
        {{1.0f, 0.0f, 0.0f}, {0.0f, 0.0f, -1.0f}},
        {{0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, -1.0f}},
        {{0.0f, 1.0f, 0.0f}, {0.0f, 0.0f, -1.0f}},

        // Left face
        {{0.0f, 1.0f, 0.0f}, {-1.0f, 0.0f, 0.0f}},
        {{0.0f, 0.0f, 0.0f}, {-1.0f, 0.0f, 0.0f}},
        {{0.0f, 1.0f, 1.0f}, {-1.0f, 0.0f, 0.0f}},
        {{0.0f, 0.0f, 0.0f}, {-1.0f, 0.0f, 0.0f}},
        {{0.0f, 0.0f, 1.0f}, {-1.0f, 0.0f, 0.0f}},
        {{0.0f, 1.0f, 1.0f}, {-1.0f, 0.0f, 0.0f}},

        // Top face
        {{0.0f, 1.0f, 0.0f}, {0.0f, 1.0f, 0.0f}},
        {{0.0f, 1.0f, 1.0f}, {0.0f, 1.0f, 0.0f}},
        {{1.0f, 1.0f, 0.0f}, {0.0f, 1.0f, 0.0f}},
        {{0.0f, 1.0f, 1.0f}, {0.0f, 1.0f, 0.0f}},
        {{1.0f, 1.0f, 1.0f}, {0.0f, 1.0f, 0.0f}},
        {{1.0f, 1.0f, 0.0f}, {0.0f, 1.0f, 0.0f}},

        // Bottom face
        {{1.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f}},
        {{1.0f, 0.0f, 1.0f}, {0.0f, 0.0f, 0.0f}},
        {{0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f}},
        {{1.0f, 0.0f, 1.0f}, {0.0f, 0.0f, 0.0f}},
        {{0.0f, 0.0f, 1.0f}, {0.0f, 0.0f, 0.0f}},
        {{0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f}}
    };

    initializeVertexBuffer(Cube, vertices, sizeof(vertices));

    QList<ModelState> *modelStates = new QList<ModelState>();
    m_modelMap.insert(Cube, modelStates);
}

void QGLView::drawModelVertices(ModelType type)
{
    QOpenGLBuffer *vertexBuffer = m_vertexBufferMap[type];
    QList<ModelState> *modelStates = m_modelMap[type];

    if (modelStates->isEmpty())
    {
        return;
    }

    vertexBuffer->bind();
    m_program->enableAttributeArray(m_positionLocation);
    m_program->enableAttributeArray(m_normalLocation);
    m_program->setAttributeBuffer(m_positionLocation, GL_FLOAT, 0, 3, sizeof(ModelVertex));
    m_program->setAttributeBuffer(m_normalLocation, GL_FLOAT, 3*sizeof(GLfloat), 3, sizeof(ModelVertex));

    for (int i = 0; i < modelStates->size(); ++i)
    {
        ModelState modelState = modelStates->at(i);
        m_program->setUniformValue(m_colorLocation, modelState.color);
        m_program->setUniformValue(m_modelMatrixLocation, modelState.modelMatrix);
        m_program->setUniformValue(m_idColorLocation, QColor(0xFF000000u + modelState.id));    // color for selection mode
        glDrawArrays(GL_TRIANGLES, 0, vertexBuffer->size()/sizeof(ModelVertex));
    }

    m_program->disableAttributeArray(m_positionLocation);
    m_program->disableAttributeArray(m_normalLocation);
    vertexBuffer->release();
}

void QGLView::paintItems()
{
    for (int i = 0; i < m_glItems.size(); ++i)
    {
        m_glItems.at(i)->paint(this);
    }
}

void QGLView::addItem(QGLItem *item)
{
    m_glItems.append(item);
}

void QGLView::removeItem(QGLItem *item)
{
    for (int i = 0; i < m_glItems.size(); ++i)
    {
        if (m_glItems.at(i) == item)
        {
            m_glItems.removeAt(i);
            return;
        }
    }
}

void QGLView::setupShaders()
{
    m_program = new QOpenGLShaderProgram();
    m_program->addShaderFromSourceFile(QOpenGLShader::Vertex, ":/shaders/SimpleVertex.glsl");
    m_program->addShaderFromSourceFile(QOpenGLShader::Fragment, ":/shaders/SimpleFragment.glsl");
    m_program->link();

    m_positionLocation = m_program->attributeLocation("position");
    m_normalLocation = m_program->attributeLocation("normal");
    m_colorLocation = m_program->uniformLocation("color");
    m_lightPosLocation = m_program->uniformLocation("lightPos");
    m_enableLightingLocation = m_program->uniformLocation("enableLighting");
    m_modelMatrixLocation = m_program->uniformLocation("modelMatrix");
    m_viewMatrixLocation = m_program->uniformLocation("viewMatrix");
    m_projectionMatrixLocation = m_program->uniformLocation("projectionMatrix");
    m_idColorLocation = m_program->uniformLocation("idColor");
    m_selectionModeLocation = m_program->uniformLocation("selectionMode");
}

void QGLView::setupWindow()
{
    connect(window()->openglContext(), SIGNAL(aboutToBeDestroyed()),
            this, SLOT(cleanup()), Qt::DirectConnection);

    QSurfaceFormat format = window()->format();
    format.setDepthBufferSize(24);
    format.setSamples(4);
    window()->setFormat(format);
}

void QGLView::setupCylinder(GLfloat r2, QVector3D P2, GLfloat r1, QVector3D P1, int detail, ModelType type)
{
    QVector<ModelVertex> vertices;
    QVector3D normal;

    // normal pointing from origin point to end point
    normal = P2 - P1;

    // create two perpendicular vectors - perp and q
    QVector3D perp = normal;
    if ((normal.x() == 0) && (normal.z() == 0)) {
        perp.setX(perp.x() + 1);
    } else {
        perp.setY(perp.y() + 1);
    }

    // cross product
    QVector3D q = QVector3D::crossProduct(perp, normal);
    perp = QVector3D::crossProduct(normal, q);

    // normalize vectors
    perp.normalize();
    q.normalize();

    // calculate vertices
    GLfloat twoPi = 2 * M_PI;
    for (int i = 0; i < detail + 1; ++i)
    {
        GLfloat theta1 = (GLfloat)i / (GLfloat)detail * twoPi; // go around circle and get points
        GLfloat theta2 = (GLfloat)(i+1) / (GLfloat)detail * twoPi;
        ModelVertex vertex[6];

        QVector3D upVector(0,0,1);
        QVector3D downVector(0,0,-1);
        QVector3D resultVector;

        // normals
        normal.setX(qCos(theta1) * perp.x() + qSin(theta1) * q.x());
        normal.setY(qCos(theta1) * perp.y() + qSin(theta1) * q.y());
        normal.setZ(qCos(theta1) * perp.z() + qSin(theta1) * q.z());

        // top vertex
        vertex[0].position.x = P1.x() + r1 * normal.x();
        vertex[0].position.y = P1.y() + r1 * normal.y();
        vertex[0].position.z = P1.z() + r1 * normal.z();
        resultVector = (upVector + normal).normalized();
        vertex[0].normal.x = resultVector.x();
        vertex[0].normal.y = resultVector.y();
        vertex[0].normal.z = resultVector.z();


        // bottom vertex
        vertex[1].position.x = P2.x() + r2 * normal.x();
        vertex[1].position.y = P2.y() + r2 * normal.y();
        vertex[1].position.z = P2.z() + r2 * normal.z();
        resultVector = (downVector + normal).normalized();
        vertex[1].normal.x = resultVector.x();
        vertex[1].normal.y = resultVector.y();
        vertex[1].normal.z = resultVector.z();

        // normals
        normal.setX(qCos(theta2) * perp.x() + qSin(theta2) * q.x());
        normal.setY(qCos(theta2) * perp.y() + qSin(theta2) * q.y());
        normal.setZ(qCos(theta2) * perp.z() + qSin(theta2) * q.z());

        vertex[2].position.x = P2.x() + r2 * normal.x();
        vertex[2].position.y = P2.y() + r2 * normal.y();
        vertex[2].position.z = P2.z() + r2 * normal.z();
        resultVector = (downVector + normal).normalized();
        vertex[2].normal.x = resultVector.x();
        vertex[2].normal.y = resultVector.y();
        vertex[2].normal.z = resultVector.z();

        vertex[3].position.x = P1.x() + r1 * normal.x();
        vertex[3].position.y = P1.y() + r1 * normal.y();
        vertex[3].position.z = P1.z() + r1 * normal.z();
        resultVector = (upVector + normal).normalized();
        vertex[3].normal.x = resultVector.x();
        vertex[3].normal.y = resultVector.y();
        vertex[3].normal.z = resultVector.z();

        if (r2 != 0.0)
        {
            vertex[5].position.x = P2.x();
            vertex[5].position.y = P2.y();
            vertex[5].position.z = P2.z();
            vertex[5].normal.x = downVector.x();
            vertex[5].normal.y = downVector.y();
            vertex[5].normal.z = downVector.z();

            vertices.append(vertex[5]);
            vertices.append(vertex[2]);
            vertices.append(vertex[1]);
        }

        if (r1 != 0.0)
        {
            vertex[4].position.x = P1.x();
            vertex[4].position.y = P1.y();
            vertex[4].position.z = P1.z();
            vertex[4].normal.x = upVector.x();
            vertex[4].normal.y = upVector.y();
            vertex[4].normal.z = upVector.z();

            vertices.append(vertex[4]);
            vertices.append(vertex[0]);
            vertices.append(vertex[3]);
        }

         // append vertex
        vertices.append(vertex[0]);
        vertices.append(vertex[1]);
        vertices.append(vertex[2]);

        vertices.append(vertex[0]);
        vertices.append(vertex[2]);
        vertices.append(vertex[3]);
    }

    initializeVertexBuffer(type, vertices);

    QList<ModelState> *modelStates = new QList<ModelState>();
    m_modelMap.insert(type, modelStates);
}

void QGLView::setupSphere(int detail)
{
    QVector<ModelVertex> rawVertices;
    QVector<ModelVertex> vertices;
    int latitudeBands = detail;
    int longitudeBands = detail;
    GLfloat radius = 1.0;

    for (int latNumber = 0; latNumber <= latitudeBands; ++latNumber)
    {
        GLfloat theta = (GLfloat)latNumber * M_PI / (GLfloat)latitudeBands;
        GLfloat sinTheta = qSin(theta);
        GLfloat cosTheta = qCos(theta);

        for (int longNumber = 0; longNumber <= longitudeBands; ++longNumber)
        {
            GLfloat phi = (GLfloat)longNumber * 2.0 * M_PI / (GLfloat)longitudeBands;
            GLfloat sinPhi = qSin(phi);
            GLfloat cosPhi = qCos(phi);

            GLfloat x = cosPhi * sinTheta;
            GLfloat y = cosTheta;
            GLfloat z = sinPhi * sinTheta;

            ModelVertex modelVertex;

            modelVertex.normal.x = x;
            modelVertex.normal.y = y;
            modelVertex.normal.z = z;
            modelVertex.position.x = radius * x;
            modelVertex.position.y = radius * y;
            modelVertex.position.z = radius * z;

            rawVertices.append(modelVertex);
        }
    }

    for (int latNumber = 0; latNumber < latitudeBands; latNumber++) {
        for (int longNumber = 0; longNumber < longitudeBands; longNumber++) {
            int first = (latNumber * (longitudeBands + 1)) + longNumber;
            int second = first + longitudeBands + 1;
            vertices.append(rawVertices.at(first));
            vertices.append(rawVertices.at(first + 1));
            vertices.append(rawVertices.at(second));
            vertices.append(rawVertices.at(second));
            vertices.append(rawVertices.at(first + 1));
            vertices.append(rawVertices.at(second + 1));
        }
    }

    initializeVertexBuffer(Sphere, vertices);

    QList<ModelState> *modelStates = new QList<ModelState>();
    m_modelMap.insert(Sphere, modelStates);
}

void QGLView::setupStack()
{
    ModelState defaultModelState;

    defaultModelState.modelMatrix = QMatrix4x4();
    defaultModelState.color = QColor(Qt::yellow);
    defaultModelState.id = 0;

    m_modelStateStack.clear();
    m_modelStateStack.push(defaultModelState);
}

void QGLView::color(float r, float g, float b, float a)
{
    color(QColor((int)(255.0*r), (int)(255.0*g), (int)(255.0*b), (int)(255.0*a)));
}

void QGLView::color(QColor color)
{
    m_modelState.color = color;
}

void QGLView::translate(float x, float y, float z)
{
    translate(QVector3D(x, y, z));
}

void QGLView::translate(QVector3D vector)
{
    m_modelState.modelMatrix.translate(vector);
}

void QGLView::rotate(float angle, float x, float y, float z)
{
    rotate(angle, QVector3D(x, y, z));
}

void QGLView::rotate(float angle, QVector3D axis)
{
    m_modelState.modelMatrix.rotate(angle, axis);
}

void QGLView::rotate(QQuaternion quaternion)
{
    m_modelState.modelMatrix.rotate(quaternion);
}

void QGLView::scale(float x, float y, float z)
{
    scale(QVector3D(x, y, z));
}

void QGLView::scale(QVector3D vector)
{
    m_modelState.modelMatrix.scale(vector);
}

void QGLView::mirror(float x, float y, float z)
{
    mirror(QVector3D(x, y, z));
}

void QGLView::mirror(QVector3D vector)
{
    int x = (int)vector.x();
    int y = (int)vector.y();
    int z = (int)vector.z();

    m_modelState.modelMatrix.scale((x == 1) ? -1.0 : 1.0,
                                   (y == 1) ? -1.0 : 1.0,
                                   (z == 1) ? -1.0 : 1.0);
}

void QGLView::resetTransformations()
{
    m_modelState = m_modelStateStack.top();
}

void QGLView::cube(float w, float l, float h, bool center)
{
    cube(QVector3D(w, l, h), center);
}

void QGLView::cube(QVector3D size, bool center)
{
    if (center)
    {
        m_modelState.modelMatrix.translate(-size/2.0);
    }
    m_modelState.modelMatrix.scale(size);

    m_modelMap[Cube]->append(m_modelState);

    resetTransformations();
}

void QGLView::cylinder(float h, float r)
{
    m_modelState.modelMatrix.scale(r, r, h);
    m_modelMap[Cylinder]->append(m_modelState);
    resetTransformations();
}

void QGLView::cone(float h, float r)
{
    m_modelState.modelMatrix.scale(r, r, h);
    m_modelMap[Cone]->append(m_modelState);
    resetTransformations();
}

void QGLView::sphere(float r)
{
    m_modelState.modelMatrix.scale(r,r,r);
    m_modelMap[Sphere]->append(m_modelState);
    resetTransformations();
}

void QGLView::beginUnion(quint32 id = 0)
{
    m_modelState.id = id;
    m_modelStateStack.push(m_modelState);
}

void QGLView::endUnion()
{
    m_modelStateStack.pop();
    m_modelState = m_modelStateStack.top();
}

void QGLView::paint()
{
    if (!m_program) {
        setupShaders();
        setupWindow();
        setupVBOs();
        setupStack();
    }

    m_program->bind();

    qreal ratio = window()->devicePixelRatio();
    int w = int(ratio * window()->width());
    int h = int(ratio * window()->height());
    float matrixH = 6.0f * h / w;
    glViewport(0, 0, w, h);

    m_projectionMatrix = QMatrix4x4();
    m_projectionMatrix.frustum(-3, 3, -matrixH/2, matrixH/2, 4, 10);

    m_viewMatrix = QMatrix4x4();
    m_viewMatrix.translate(0,0,0);

    m_program->setUniformValue(m_projectionMatrixLocation, m_projectionMatrix);
    m_program->setUniformValue(m_viewMatrixLocation, m_viewMatrix);
    m_program->setUniformValue(m_lightPosLocation, QVector3D(-1, -1, 1));
    m_program->setUniformValue(m_enableLightingLocation, true);
    m_program->setUniformValue(m_selectionModeLocation, false);

    glClearColor(0.0, 0.0, 0.0, 1.0);//(0, 104.0/255.0, 55.0/255.0, 1.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Enable depth test
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
    glDepthMask(true);

    // Enable back face culling
    glEnable(GL_CULL_FACE);

    clearModels();

    paintItems();

    /*float currentRotation;
    currentRotation = qSin((qreal)QDateTime::currentMSecsSinceEpoch()/1000.0) * 180;
    color(Qt::red);
    translate(qSin((qreal)QDateTime::currentMSecsSinceEpoch()/1000.0), 0, -7);
    rotate(currentRotation, 1, 1, 0);
    beginUnion(1);
        //cube(1.5, 1, 1, true);
        cone(2,0.9);
        //translate(0.2, 0.2, 0.2);
        color(Qt::blue);
        //cube(1.2, 1, 1);
        sphere(0.8);
    endUnion();*/

    //currentRotation = -1.0 * qSin((qreal)QDateTime::currentMSecsSinceEpoch()/1000.0) * 180.0;
    //color(Qt::gray);
    /*drawCylinder(0.5, 10,
             QVector3D(-1.0 * qSin((qreal)QDateTime::currentMSecsSinceEpoch()/1000.0), 0, -7),
             currentRotation, QVector3D(1,1,0));*/

    drawModels();

    m_program->release();
}

void QGLView::cleanup()
{
    if (m_program) {
        delete m_program;
        m_program = 0;
    }
}

void QGLView::sync()
{
    m_thread_t = m_t;
}

