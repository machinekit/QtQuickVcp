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
#include <QOpenGLTexture>
#include <QStack>
#include <QStaticText>
#include <QPainter>
#include "qglitem.h"

class QGLItem;

class QGLView : public QQuickItem
{
    Q_OBJECT

    Q_PROPERTY(qreal t READ t WRITE setT NOTIFY tChanged)
    Q_ENUMS(ModelType)
    Q_ENUMS(PaintMode)

public:
    QGLView();

    typedef struct {
        GLfloat x;
        GLfloat y;
    } Vector2D;

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
        Vector3D position;
        Vector2D texCoordinate;
    } TextVertex;

    typedef struct {
        QMatrix4x4 modelMatrix;
        QColor color;
        quint32 id;
    } ModelParameters;

    typedef struct {
        QMatrix4x4 modelMatrix;
        Vector3D vector;
        QColor color;
        GLfloat width;
        bool stipple;
        GLfloat stippleLength;
    } LineParameters;

    typedef struct {
        QMatrix4x4 modelMatrix;
        QStaticText staticText;
        QColor color;
    } TextParameters;

    enum ModelType {
        NoType = 0,
        Cube = 1,
        Cylinder = 2,
        Sphere = 3,
        Cone = 4
    };

    qreal t() const { return m_t; }
    void setT(qreal t);

    Q_INVOKABLE void readPixel(int x, int y);

    // model and line transformation functions
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
    void resetTransformations(bool hard = false);

    // model functions
    void cube(float w, float l, float h, bool center = false);
    void cube(QVector3D size, bool center = false);
    void cylinder(float r, float h);
    void cone(float r, float h);
    void sphere(float r);

    // line functions
    void lineWidth(float width);
    void lineStipple(float enable, float length = 5.0);
    void line(float x, float y, float z);
    void line(QVector3D vector);

    // text functions
    void text(QString text, QFont font = QFont());

    // grouping functions
    void beginUnion(quint32 id);
    void endUnion();

    // item handling
    void addItem(QGLItem *item);
    void removeItem(QGLItem *item);

signals:
    void tChanged();

public slots:
    void paint();
    void cleanup();
    void sync();

private slots:
    void handleWindowChanged(QQuickWindow *win);

private:
    // the shader programs
    QOpenGLShaderProgram *m_modelProgram;
    QOpenGLShaderProgram *m_lineProgram;
    QOpenGLShaderProgram *m_textProgram;

    // vertex buffers
    QMap<ModelType, QOpenGLBuffer*> m_vertexBufferMap;
    QOpenGLBuffer *m_lineVertexBuffer;
    QOpenGLBuffer *m_textVertexBuffer;

    // transformation matrices
    QMatrix4x4 m_viewMatrix;
    QMatrix4x4 m_projectionMatrix;

    // shader program location ids
    int m_positionLocation;
    int m_normalLocation;
    int m_colorLocation;
    int m_lightPosLocation;
    int m_enableLightingLocation;
    int m_projectionMatrixLocation;
    int m_viewMatrixLocation;
    int m_modelMatrixLocation;
    int m_selectionModeLocation;
    int m_idColorLocation;

    int m_lineProjectionMatrixLocation;
    int m_lineViewMatrixLocation;
    int m_lineModelMatrixLocation;
    int m_lineColorLocation;
    int m_linePositionLocation;
    int m_lineStippleLocation;
    int m_lineStippleLengthLocation;

    int m_textProjectionMatrixLocation;
    int m_textViewMatrixLocation;
    int m_textModelMatrixLocation;
    int m_textColorLocation;
    int m_textPositionLocation;
    int m_textTexCoordinateLocation;
    int m_textTextureLocation;

    // thread secure properties
    qreal m_t;
    qreal m_thread_t;

    // model stack
    ModelParameters m_modelParameters;
    QStack<ModelParameters> m_modelParametersStack;
    QMap<ModelType, QList<ModelParameters>* > m_modelMap;

    // line stack
    LineParameters m_lineParameters;
    QStack<LineParameters> m_lineParametersStack;
    QList<LineParameters> m_lineParametersList;

    // text stack
    QList<QStaticText> m_textTextList;
    QList<QOpenGLTexture*> m_textTextureList;
    TextParameters m_textParameters;
    QStack<TextParameters> m_textParametersStack;
    QList<TextParameters> m_textParametersList;

    //GL items
    QList<QGLItem*> m_glItems;

    void clearModels(ModelType type);
    void drawModels(ModelType type);
    void drawModelVertices(ModelType type);

    void clearLines();
    void drawLines();

    void clearTexts();
    void drawTexts();
    void generateTextTexture(const QStaticText &staticText, QFont font);
    void clearTextTextures();

    void paintItems();

    // setup functions
    void initializeVertexBuffer(ModelType type, const QVector<ModelVertex> & vertices);
    void initializeVertexBuffer(ModelType type, const void *bufferData, int bufferLength);
    void setupVBOs();
    void setupLineVertexBuffer();
    void setupTextVertexBuffer();
    void setupShaders();
    void setupWindow();
    void setupCube();
    void setupCylinder(GLfloat originRadius, QVector3D originPoint, GLfloat endRadius, QVector3D endPoint, int detail, ModelType type);
    void setupSphere(int detail);
    void setupStack();
};

#endif // SQUIRCLE_H
