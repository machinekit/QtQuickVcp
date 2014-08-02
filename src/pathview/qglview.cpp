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

#include "qglview.h"

#include <QtQuick/qquickwindow.h>
#include <QtGui/QOpenGLShaderProgram>
#include <QtGui/QOpenGLContext>
#include <QtCore/qmath.h>
#include <QDateTime>

QGLView::QGLView(QQuickItem *parent)
    : QQuickItem(parent)
    , m_initialized(false)
    , m_modelProgram(0)
    , m_lineProgram(0)
    , m_textProgram(0)
    , m_projectionAspectRatio(1.0)
    , m_pathEnabled(false)
    , m_selectionModeActive(false)
    , m_currentGlItem(NULL)
    , m_propertySignalMapper(new QSignalMapper(this))
    , m_camera(new QGLCamera(this))
    , m_light(new QGLLight(this))
{
    setFlag(QQuickItem::ItemHasContents, true);

    connect(this, SIGNAL(windowChanged(QQuickWindow*)), this, SLOT(handleWindowChanged(QQuickWindow*)));
    connect(this, SIGNAL(childrenChanged()), this, SLOT(updateChildren()));
    connect(m_propertySignalMapper, SIGNAL(mapped(QObject*)), this, SLOT(updateItem(QObject*)));
    connect(this, SIGNAL(initialized()), this, SLOT(updateItems()), Qt::QueuedConnection);
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
    m_selectionPoint = QPoint(x, window()->height() - y);
    m_selectionModeActive = true;
    update();
}

void QGLView::handleWindowChanged(QQuickWindow *win)
{
    if (win) {
        // Connect the beforeRendering signal to our paint function.
        // Since this call is executed on the rendering thread it must be
        // a Qt::DirectConnection
        connect(win, SIGNAL(afterRendering()), this, SLOT(paint()), Qt::DirectConnection);
        connect(win, SIGNAL(beforeSynchronizing()), this, SLOT(sync()), Qt::DirectConnection);
        //connect(win, SIGNAL(frameSwapped()), win, SLOT(update()));  // repaint every frame
        connect(win, SIGNAL(widthChanged(int)), this, SLOT(updatePerspectiveAspectRatio()));
        connect(win, SIGNAL(heightChanged(int)), this, SLOT(updatePerspectiveAspectRatio()));

        // If we allow QML to do the clearing, they would clear what we paint
        // and nothing would show.
        win->setClearBeforeRendering(true);

        updatePerspectiveAspectRatio();
    }
}

void QGLView::updatePerspectiveAspectRatio()
{
    qreal ratio = window()->devicePixelRatio();
    int w = int(ratio * window()->width());
    int h = int(ratio * window()->height());
    m_projectionAspectRatio = (float)w/(float)h;
    updateProjectionMatrix();
}

void QGLView::updateViewMatrix()
{
    m_viewMatrix = m_camera->modelViewMatrix();
    update();
}

void QGLView::updateProjectionMatrix()
{
    m_projectionMatrix = m_camera->projectionMatrix(m_projectionAspectRatio);
    update();
}

void QGLView::updateItems()
{
    if (!m_initialized)
    {
        return;
    }

    paintGLItems();
    update();
}

void QGLView::updateItem(QObject *item)
{
    if (!m_initialized)
    {
        return;
    }

    paintGLItem(static_cast<QGLItem*>(item));
    update();
}

void QGLView::updateChildren()
{
    QList<QGLItem*> newItems;
    QList<QQuickItem*> objectChildren = childItems();
    QGLItem* glItem;

    // get all GL items
    for (int i = 0; i < objectChildren.size(); ++i)
    {
        glItem = qobject_cast<QGLItem*>(objectChildren.at(i));
        if (glItem != NULL)
        {
            newItems.append(glItem);
        }
    }

    // remove all remvoed GL items
    for (int i = (m_glItems.count()-1); i >= 0; i--)
    {
        if (!newItems.contains(m_glItems.at(i)))
        {
            removeGlItem(i);
        }
    }

    // add all new GL items
    for (int i = 0; i < newItems.count(); ++i)
    {
        if (!m_glItems.contains(newItems.at(i)))
        {
            addGlItem(newItems.at(i));
        }
    }
}

void QGLView::addDrawableList(QGLView::ModelType type)
{
    QList<Parameters*> *parametersList = new QList<Parameters*>();
    m_drawableMap.insert(type, parametersList);
}

QList<QGLView::Parameters *> *QGLView::getDrawableList(QGLView::ModelType type)
{
    return m_drawableMap.value(type);
}

QGLView::Parameters* QGLView::addDrawableData(const QGLView::LineParameters &parameters)
{
    // add parameter
    QList<Parameters*> *parametersList = m_drawableMap.value(Line);
    LineParameters *lineParameters = new LineParameters(parameters);
    lineParameters->creator = m_currentGlItem;
    parametersList->append(lineParameters);

    // add drawable to list
    Drawable drawable;
    drawable.type = Line;
    drawable.parameters = lineParameters;
    m_currentDrawableList->append(drawable);

    return lineParameters;
}

QGLView::Parameters* QGLView::addDrawableData(const QGLView::TextParameters &parameters)
{
    QList<Parameters*> *parametersList = m_drawableMap.value(Text);
    TextParameters *textParameters = new TextParameters(parameters);
    textParameters->creator = m_currentGlItem;
    parametersList->append(textParameters);

    Drawable drawable;
    drawable.type = Text;
    drawable.parameters = textParameters;
    m_currentDrawableList->append(drawable);

    return textParameters;
}

QGLView::Parameters* QGLView::addDrawableData(QGLView::ModelType type, const QGLView::Parameters &parameters)
{
    QList<Parameters*> *parametersList = m_drawableMap.value(type);
    Parameters *modelParameters = new Parameters(parameters);
    modelParameters->creator = m_currentGlItem;
    parametersList->append(modelParameters);

    Drawable drawable;
    drawable.type = type;
    drawable.parameters = modelParameters;
    m_currentDrawableList->append(drawable);

    return modelParameters;
}

void QGLView::drawDrawables(QGLView::ModelType type)
{
    if (type == NoType)
    {
        QMapIterator<ModelType, QList<Parameters*>* > i(m_drawableMap);
        while (i.hasNext()) {
            i.next();
            drawDrawables(i.key());
        }
    }
    else
    {
        switch (type)
        {
        case Cube:
        case Cylinder:
        case Cone:
        case Sphere:
            drawModelVertices(type);
            break;
        case Text:
            drawTexts();
            break;
        case Line:
            drawLines();
            break;
        default:
            return;
        }
    }
}

void QGLView::clearDrawables()
{
    QMapIterator<ModelType, QList<Parameters*>* > i(m_drawableMap);
    while (i.hasNext()) {
        i.next();
        QList<Parameters*> * parametersList = i.value();
        for (int j = 0; j < i.value()->size(); ++j)
        {
            Parameters *parameters = parametersList->takeAt(j);
            delete parameters;
        }
    }
}

void QGLView::cleanupDrawables(ModelType type)
{
    QList<Parameters*> * parametersList = m_drawableMap.value(type);

    for (int i = (parametersList->size()-1); i >= 0; i--)
    {
        Parameters *parameters = parametersList->at(i);
        if (parameters->deleteFlag)
        {
            delete parametersList->takeAt(i);
        }
    }
}

void QGLView::removeDrawables(QList<QGLView::Drawable> *drawableList)
{
    QList<ModelType> types;

    for (int i = (drawableList->size() - 1); i >= 0; i--)
    {
        Drawable drawable = drawableList->at(i);
        drawable.parameters->deleteFlag = true; // mark for deletion
        if (!types.contains(drawable.type)) {
            types.append(drawable.type);
        }
    }

    for (int i = 0; i < types.size(); ++i) {
        cleanupDrawables(types.at(i));
    }

    drawableList->clear();
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
    setupLineVertexBuffer();
    setupTextVertexBuffer();
}

void QGLView::setupLineVertexBuffer()
{
    const int MaxLinesPerPath = 50;

    m_lineVertexBuffer = new QOpenGLBuffer(QOpenGLBuffer::VertexBuffer);
    m_lineVertexBuffer->create();
    m_lineVertexBuffer->setUsagePattern(QOpenGLBuffer::StaticDraw);
    m_lineVertexBuffer->bind();
    m_lineVertexBuffer->allocate(MaxLinesPerPath*sizeof(GLvector3D));
    m_lineVertexBuffer->release();

    addDrawableList(Line);
}

void QGLView::setupTextVertexBuffer()
{
    static const TextVertex vertices[] = {
        {{0.0f, 1.0f, 0.0f}, {0.0f, 0.0f}},
        {{0.0f, 0.0f, 0.0f}, {0.0f, 1.0f}},
        {{1.0f, 1.0f, 0.0f}, {1.0f, 0.0f}},
        {{0.0f, 0.0f, 0.0f}, {0.0f, 1.0f}},
        {{1.0f, 0.0f, 0.0f}, {1.0f, 1.0f}},
        {{1.0f, 1.0f, 0.0f}, {1.0f, 0.0f}},
        {{1.0f, 1.0f, 0.0f}, {0.0f, 0.0f}},
        {{1.0f, 0.0f, 0.0f}, {0.0f, 1.0f}},
        {{0.0f, 1.0f, 0.0f}, {1.0f, 0.0f}},
        {{1.0f, 0.0f, 0.0f}, {0.0f, 1.0f}},
        {{0.0f, 0.0f, 0.0f}, {1.0f, 1.0f}},
        {{0.0f, 1.0f, 0.0f}, {1.0f, 0.0f}},
    };

    m_textVertexBuffer = new QOpenGLBuffer(QOpenGLBuffer::VertexBuffer);
    m_textVertexBuffer->create();
    m_textVertexBuffer->setUsagePattern(QOpenGLBuffer::StaticDraw);
    m_textVertexBuffer->bind();
    m_textVertexBuffer->allocate(vertices, sizeof(vertices));
    m_textVertexBuffer->release();

    addDrawableList(Text);
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

    addDrawableList(Cube);
}

void QGLView::setupShaders()
{
    // model shader
    m_modelProgram = new QOpenGLShaderProgram();
    m_modelProgram->addShaderFromSourceFile(QOpenGLShader::Vertex, ":/shaders/ModelVertexShader.glsl");
    m_modelProgram->addShaderFromSourceFile(QOpenGLShader::Fragment, ":/shaders/ModelFragmentShader.glsl");
    m_modelProgram->link();

    m_positionLocation = m_modelProgram->attributeLocation("position");
    m_normalLocation = m_modelProgram->attributeLocation("normal");
    m_colorLocation = m_modelProgram->uniformLocation("color");
    m_lightPositionLocation = m_modelProgram->uniformLocation("light.position");
    m_lightIntensitiesLocation = m_modelProgram->uniformLocation("light.intensities");
    m_lightAttenuationLocation = m_modelProgram->uniformLocation("light.attenuation");
    m_lightAmbientCoefficientLocation = m_modelProgram->uniformLocation("light.ambientCoefficient");
    m_lightEnabledLocation = m_modelProgram->uniformLocation("light.enabled");
    m_modelMatrixLocation = m_modelProgram->uniformLocation("modelMatrix");
    m_viewMatrixLocation = m_modelProgram->uniformLocation("viewMatrix");
    m_projectionMatrixLocation = m_modelProgram->uniformLocation("projectionMatrix");
    m_idColorLocation = m_modelProgram->uniformLocation("idColor");
    m_selectionModeLocation = m_modelProgram->uniformLocation("selectionMode");

    // line shader
    m_lineProgram = new QOpenGLShaderProgram();
    m_lineProgram->addShaderFromSourceFile(QOpenGLShader::Vertex, ":/shaders/LineVertexShader.glsl");
    m_lineProgram->addShaderFromSourceFile(QOpenGLShader::Fragment, ":/shaders/LineFragmentShader.glsl");
    m_lineProgram->link();

    m_linePositionLocation = m_lineProgram->attributeLocation("position");
    m_lineProjectionMatrixLocation = m_lineProgram->uniformLocation("projectionMatrix");
    m_lineViewMatrixLocation = m_lineProgram->uniformLocation("viewMatrix");
    m_lineModelMatrixLocation = m_lineProgram->uniformLocation("modelMatrix");
    m_lineColorLocation = m_lineProgram->uniformLocation("color");
    m_lineStippleLocation = m_lineProgram->uniformLocation("stipple");
    m_lineStippleLengthLocation = m_lineProgram->uniformLocation("stippleLength");
    m_lineIdColorLocation = m_lineProgram->uniformLocation("idColor");
    m_lineSelectionModeLocation = m_lineProgram->uniformLocation("selectionMode");

    // text shader
    m_textProgram = new QOpenGLShaderProgram();
    m_textProgram->addShaderFromSourceFile(QOpenGLShader::Vertex, ":/shaders/TextVertexShader.glsl");
    m_textProgram->addShaderFromSourceFile(QOpenGLShader::Fragment, ":/shaders/TextFragmentShader.glsl");
    m_textProgram->link();

    m_textPositionLocation = m_textProgram->attributeLocation("position");
    m_textTexCoordinateLocation = m_textProgram->attributeLocation("texCoordinate");
    m_textProjectionMatrixLocation = m_textProgram->uniformLocation("projectionMatrix");
    m_textViewMatrixLocation = m_textProgram->uniformLocation("viewMatrix");
    m_textModelMatrixLocation = m_textProgram->uniformLocation("modelMatrix");
    m_textColorLocation = m_textProgram->uniformLocation("color");
    m_textTextureLocation = m_textProgram->uniformLocation("texture");
    m_textAspectRatioLocation = m_textProgram->uniformLocation("aspectRatio");
    m_textAlignmentLocation = m_textProgram->uniformLocation("alignment");
    m_textIdColorLocation = m_textProgram->uniformLocation("idColor");
    m_textSelectionModeLocation = m_textProgram->uniformLocation("selectionMode");
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
    for (int i = 0; i < detail; ++i)
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

    addDrawableList(type);
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

    addDrawableList(Sphere);
}

void QGLView::setupStack()
{
    m_modelParametersStack.push(new Parameters());
    m_modelParameters = new Parameters();

    m_lineParametersStack.push(new LineParameters());
    m_lineParameters = new LineParameters();

    m_textParametersStack.push(new TextParameters());
    m_textParameters = new TextParameters();
}

void QGLView::drawModelVertices(ModelType type)
{
    QOpenGLBuffer *vertexBuffer = m_vertexBufferMap[type];
    QList<Parameters*> *modelParametersList = getDrawableList(type);

    if (modelParametersList->isEmpty())
    {
        return;
    }

    vertexBuffer->bind();
    m_modelProgram->enableAttributeArray(m_positionLocation);
    m_modelProgram->enableAttributeArray(m_normalLocation);
    m_modelProgram->setAttributeBuffer(m_positionLocation, GL_FLOAT, 0, 3, sizeof(ModelVertex));
    m_modelProgram->setAttributeBuffer(m_normalLocation, GL_FLOAT, 3*sizeof(GLfloat), 3, sizeof(ModelVertex));

    for (int i = 0; i < modelParametersList->size(); ++i)
    {
        Parameters *modelParameters = static_cast<Parameters*>(modelParametersList->at(i));
        m_modelProgram->setUniformValue(m_colorLocation, modelParameters->color);
        m_modelProgram->setUniformValue(m_modelMatrixLocation, modelParameters->modelMatrix);

        if (m_selectionModeActive)  // selection mode active
        {
            m_modelProgram->setUniformValue(m_idColorLocation, QColor(0xFF000000u + m_currentDrawableId));    // color for selection mode
            m_drawableIdMap.insert(m_currentDrawableId, modelParameters);
            m_currentDrawableId++;
        }

        glDrawArrays(GL_TRIANGLES, 0, vertexBuffer->size()/sizeof(ModelVertex));
    }

    m_modelProgram->disableAttributeArray(m_positionLocation);
    m_modelProgram->disableAttributeArray(m_normalLocation);
    vertexBuffer->release();
}

void QGLView::drawLines()
{
    QList<Parameters*>* parametersList = getDrawableList(Line);

    if (parametersList->isEmpty())
    {
        return;
    }

    m_lineVertexBuffer->bind();
    m_lineProgram->enableAttributeArray(m_linePositionLocation);
    m_lineProgram->setAttributeBuffer(m_linePositionLocation, GL_FLOAT, 0, 3);

    for (int i = 0; i < parametersList->size(); ++i)
    {
        LineParameters *lineParameters = static_cast<LineParameters*>(parametersList->at(i));
        m_lineVertexBuffer->write(0, lineParameters->vertices.data(), lineParameters->vertices.size() * sizeof(GLvector3D));
        m_lineProgram->setUniformValue(m_lineColorLocation, lineParameters->color);
        m_lineProgram->setUniformValue(m_lineModelMatrixLocation, lineParameters->modelMatrix);
        m_lineProgram->setUniformValue(m_lineStippleLocation, lineParameters->stipple);
        m_lineProgram->setUniformValue(m_lineStippleLengthLocation, lineParameters->stippleLength);

        if (m_selectionModeActive)  // selection mode active
        {
            m_lineProgram->setUniformValue(m_lineIdColorLocation, QColor(0xFF000000u + m_currentDrawableId));    // color for selection mode
            m_drawableIdMap.insert(m_currentDrawableId, lineParameters);
            m_currentDrawableId++;
        }

        glLineWidth(lineParameters->width);
        glDrawArrays(GL_LINE_STRIP, 0, lineParameters->vertices.size());
    }

    m_lineProgram->disableAttributeArray(m_linePositionLocation);
    m_lineVertexBuffer->release();
}

void QGLView::drawTexts()
{
    QList<Parameters*>* parametersList = getDrawableList(Text);

    if (parametersList->isEmpty())
    {
        return;
    }

    m_textVertexBuffer->bind();
    m_textProgram->enableAttributeArray(m_textPositionLocation);
    m_textProgram->enableAttributeArray(m_textTexCoordinateLocation);
    m_textProgram->setAttributeBuffer(m_textPositionLocation, GL_FLOAT, 0, 3, sizeof(TextVertex));
    m_textProgram->setAttributeBuffer(m_textTexCoordinateLocation, GL_FLOAT, 3*sizeof(GLfloat), 2, sizeof(TextVertex));

    for (int i = 0; i < parametersList->size(); ++i)
    {
        TextParameters *textParameters = static_cast<TextParameters*>(parametersList->at(i));
        QStaticText staticText = textParameters->staticText;
        int textureIndex = m_textTextList.indexOf(staticText);
        QOpenGLTexture *texture = m_textTextureList.at(textureIndex);
        float aspectRatio = m_textAspectRatioList.at(textureIndex);

        m_textProgram->setUniformValue(m_textAspectRatioLocation, aspectRatio);
        m_textProgram->setUniformValue(m_textAlignmentLocation, (GLint)textParameters->alignment);
        m_textProgram->setUniformValue(m_textColorLocation, textParameters->color);
        m_textProgram->setUniformValue(m_textModelMatrixLocation, textParameters->modelMatrix);
        m_textProgram->setUniformValue(m_textTextureLocation, texture->textureId());

        if (m_selectionModeActive)  // selection mode active
        {
            m_textProgram->setUniformValue(m_textIdColorLocation, QColor(0xFF000000u + m_currentDrawableId));    // color for selection mode
            m_drawableIdMap.insert(m_currentDrawableId, textParameters);
            m_currentDrawableId++;
        }

        texture->bind(texture->textureId());
        glDrawArrays(GL_TRIANGLES, 0, m_textVertexBuffer->size()/sizeof(TextVertex));
        texture->release(texture->textureId());
    }

    m_textProgram->disableAttributeArray(m_textPositionLocation);
    m_textProgram->disableAttributeArray(m_textTexCoordinateLocation);
    m_textVertexBuffer->release();
}

void QGLView::generateTextTexture(const QStaticText &staticText, QFont font)
{
    if (m_textTextureList.size() > 100) // maximum of 100 rendered texts TODO: make parameter
    {
        clearTextTextures();
    }

    QPixmap pixmap(QSize(qCeil(staticText.size().width()), qCeil(staticText.size().height())));
    pixmap.fill(QColor(Qt::transparent));

    QPainter painter(&pixmap);
    painter.setPen(Qt::white);
    painter.setFont(font);
    painter.drawStaticText(0, 0, staticText);

    QOpenGLTexture *texture = new QOpenGLTexture(QOpenGLTexture::Target2D);
    texture->create();
    texture->setMinificationFilter(QOpenGLTexture::Nearest);
    texture->setMagnificationFilter(QOpenGLTexture::Nearest);
    texture->setData(pixmap.toImage());

    float aspectRatio = staticText.size().width()/staticText.size().height();

    m_textTextList.append(staticText);
    m_textTextureList.append(texture);
    m_textAspectRatioList.append(aspectRatio);
}

void QGLView::clearTextTextures()
{
    QList<int> usedIndexes;
    QList<Parameters*>* parametersList = getDrawableList(Text);


    // search for indexes still in use
    for (int i = 0; i < parametersList->size(); ++i)
    {
        TextParameters *textParameters = static_cast<TextParameters*>(parametersList->at(i));

        if (m_textTextList.contains(textParameters->staticText))
        {
            usedIndexes.append(i);
        }
    }

    // remove all textures that are not in use anymore
    for (int i = (m_textTextureList.size()-1); i >= 0; i--)
    {
        if (!usedIndexes.contains(i))
        {
            m_textTextList.removeAt(i);
            m_textAspectRatioList.removeAt(i);
            m_textTextureList.at(i)->destroy();
            delete m_textTextureList.at(i);
            m_textTextureList.removeAt(i);
        }
    }
}

void QGLView::paintGLItems()
{
    for (int i = 0; i < m_glItems.size(); ++i)
    {
        paintGLItem(m_glItems.at(i));
    }
}

void QGLView::clearGLItem(QGLItem *item)
{
    removeDrawables(m_drawableListMap.value(item));
}

void QGLView::paintGLItem(QGLItem *item)
{
    if (item->isVisible())
    {
        m_currentDrawableList = m_drawableListMap.value(item);
        m_currentGlItem = item;
        resetTransformations(true); // reset all tranformations for a clean start
        item->paint(this);
    }
    else
    {
        removeDrawables(m_drawableListMap.value(item));     // if the item is not visible we remove all drawables
    }
}

quint32 QGLView::getSelection()
{
    QList<quint32> ids;
    QList<quint32> sortedIds;
    int selectionSize = 5;  // only odd selection sizes make sense
    int sizeOffset;
    QPoint correctedSelectionPoint;

    sizeOffset = qFloor((float)selectionSize/2.0);  // calculate the offset

    // correct the position of the selection to center the mouse click
    correctedSelectionPoint.setX(qMin(qMax(m_selectionPoint.x() - sizeOffset, 0), window()->width() - selectionSize));
    correctedSelectionPoint.setY(qMin(qMax(m_selectionPoint.y() - sizeOffset, 0), window()->height() - selectionSize));

    // read the data
    for (int i = 0; i < (selectionSize * selectionSize); ++i)
    {
        GLcolorRGB colorRGB;
        quint32 id;

        glReadPixels(correctedSelectionPoint.x() + (i % selectionSize), correctedSelectionPoint.y() + (i / selectionSize), 1, 1, GL_RGB, GL_UNSIGNED_BYTE, &colorRGB);
        id = ((quint32)colorRGB.r << 16) + ((quint32)colorRGB.g << 8) + (quint32)colorRGB.b;

        if ((id != 0) && (!ids.contains(id)))   // collect all ids in selection
        {
            ids.append(id);
        }
    }

    int maxCount = 0;

    // find the id that is most common in the selection
    foreach(int id, ids)
    {
        int count;
        qCount(ids.begin(), ids.end(), id, count);
        if (count > maxCount) {
            sortedIds.prepend(id);
            maxCount = count;
        }
        else {
            sortedIds.append(id);
        }
    }

    if (sortedIds.size() > 0)
    {
        return sortedIds.first();
    }
    else
    {
        return 0;   // nothing selected
    }
}

void QGLView::addGlItem(QGLItem *item)
{
    m_glItems.append(item);

    QList<Drawable> *drawableList = new QList<Drawable>();
    m_drawableListMap.insert(item, drawableList);

    if (m_initialized) {
        paintGLItem(item);
        update();
    }

    m_propertySignalMapper->setMapping(item, item);
    connect(item, SIGNAL(needsUpdate()), m_propertySignalMapper, SLOT(map()));
    connect(this, SIGNAL(drawableSelected(void*)), item, SLOT(selectDrawable(void*)), Qt::QueuedConnection);
    emit glItemsChanged(glItems());
}

void QGLView::removeGlItem(int index)
{
    QGLItem *item = m_glItems.takeAt(index);

    if (m_initialized) {
        clearGLItem(item);
        update();
    }

    delete m_drawableListMap.take(item);

    m_propertySignalMapper->removeMappings(item);
    disconnect(item, SIGNAL(propertyChanged()), m_propertySignalMapper, SLOT(map()));
    disconnect(this, SIGNAL(drawableSelected(void*)), item, SLOT(selectDrawable(void*)));
    emit glItemsChanged(glItems());
}

void QGLView::removeGlItem(QGLItem *item)
{
    for (int i = 0; i < m_glItems.size(); ++i)
    {
        if (m_glItems.at(i) == item)
        {
            removeGlItem(i);
            return;
        }
    }
}

void QGLView::clearGlItems()
{
    for (int i = (m_glItems.size()-1); i >= 0; i--)
    {
        removeGlItem(i);
    }
}

QQmlListProperty<QGLItem> QGLView::glItems()
{
    return QQmlListProperty<QGLItem>(this, m_glItems);
}

int QGLView::glItemCount() const
{
    return m_glItems.count();
}

QGLItem *QGLView::glItem(int index) const
{
    return m_glItems.at(index);
}

void QGLView::color(float r, float g, float b, float a)
{
    color(QColor((int)(255.0*r), (int)(255.0*g), (int)(255.0*b), (int)(255.0*a)));
}

void QGLView::color(const QColor &color)
{
    m_modelParameters->color = color;
    m_lineParameters->color = color;
    m_textParameters->color = color;
}

void QGLView::translate(float x, float y, float z)
{
    translate(QVector3D(x, y, z));
}

void QGLView::translate(const QVector3D &vector)
{
    m_modelParameters->modelMatrix.translate(vector);
    m_lineParameters->modelMatrix.translate(vector);
    m_textParameters->modelMatrix.translate(vector);
}

void QGLView::rotate(float angle, float x, float y, float z)
{
    rotate(angle, QVector3D(x, y, z));
}

void QGLView::rotate(float angle, const QVector3D &axis)
{
    m_modelParameters->modelMatrix.rotate(angle, axis);
    m_lineParameters->modelMatrix.rotate(angle, axis);
    m_textParameters->modelMatrix.rotate(angle, axis);
}

void QGLView::rotate(const QQuaternion &quaternion)
{
    m_modelParameters->modelMatrix.rotate(quaternion);
    m_lineParameters->modelMatrix.rotate(quaternion);
    m_textParameters->modelMatrix.rotate(quaternion);
}

void QGLView::scale(float x, float y, float z)
{
    scale(QVector3D(x, y, z));
}

void QGLView::scale(const QVector3D &vector)
{
    m_modelParameters->modelMatrix.scale(vector);
    m_lineParameters->modelMatrix.scale(vector);
    m_textParameters->modelMatrix.scale(vector);
}

void QGLView::mirror(float x, float y, float z)
{
    mirror(QVector3D(x, y, z));
}

void QGLView::mirror(const QVector3D &vector)
{
    int x = (int)vector.x();
    int y = (int)vector.y();
    int z = (int)vector.z();

    m_modelParameters->modelMatrix.scale((x == 1) ? -1.0 : 1.0,
                                   (y == 1) ? -1.0 : 1.0,
                                   (z == 1) ? -1.0 : 1.0);
    m_lineParameters->modelMatrix.scale((x == 1) ? -1.0 : 1.0,
                                   (y == 1) ? -1.0 : 1.0,
                                   (z == 1) ? -1.0 : 1.0);
    m_textParameters->modelMatrix.scale((x == 1) ? -1.0 : 1.0,
                                   (y == 1) ? -1.0 : 1.0,
                                   (z == 1) ? -1.0 : 1.0);
}

void QGLView::resetTransformations(bool hard)
{
    if (hard)
    {
        delete m_modelParameters;
        m_modelParameters = m_modelParametersStack.takeFirst();
        foreach (Parameters *parameters, m_modelParametersStack)
        {
            delete parameters;
        }
        m_modelParametersStack.clear();
        m_modelParametersStack.push(new Parameters(m_modelParameters));

        delete m_lineParameters;
        m_lineParameters = m_lineParametersStack.takeFirst();
        foreach (LineParameters *parameters, m_lineParametersStack)
        {
            delete parameters;
        }
        m_lineParametersStack.clear();
        m_lineParametersStack.push(new LineParameters(m_lineParameters));

        delete m_textParameters;
        m_textParameters = m_textParametersStack.takeFirst();
        foreach (TextParameters *parameters, m_textParametersStack)
        {
            delete parameters;
        }
        m_textParametersStack.clear();
        m_textParametersStack.push(new TextParameters(m_textParameters));
    }
    else
    {
        delete m_modelParameters;
        m_modelParameters = new Parameters(m_modelParametersStack.top());
        delete m_lineParameters;
        m_lineParameters = new LineParameters(m_lineParametersStack.top());
        delete m_textParameters;
        m_textParameters = new TextParameters(m_textParametersStack.top());
    }
}

void *QGLView::cube(float w, float l, float h, bool center)
{
    return cube(QVector3D(w, l, h), center);
}

void *QGLView::cube(const QVector3D &size, bool center)
{
    if (center)
    {
        m_modelParameters->modelMatrix.translate(-size/2.0);
    }
    m_modelParameters->modelMatrix.scale(size);

    Parameters *parameters = addDrawableData(Cube, m_modelParameters);
    resetTransformations();
    return parameters;
}

void *QGLView::cylinder(float r, float h)
{
    m_modelParameters->modelMatrix.scale(r, r, h);
    Parameters *parameters = addDrawableData(Cylinder, m_modelParameters);
    resetTransformations();
    return parameters;
}

void *QGLView::cone(float r, float h)
{
    m_modelParameters->modelMatrix.scale(r, r, h);
    Parameters *parameters = addDrawableData(Cone, m_modelParameters);
    resetTransformations();
    return parameters;
}

void *QGLView::sphere(float r)
{
    m_modelParameters->modelMatrix.scale(r,r,r);
    Parameters *parameters = addDrawableData(Sphere, m_modelParameters);
    resetTransformations();
    return parameters;
}

void QGLView::lineWidth(float width)
{
    m_lineParameters->width = width;
}

void QGLView::lineStipple(float enable, float length)
{
    m_lineParameters->stipple = enable;
    m_lineParameters->stippleLength = length;
}

void *QGLView::line(float x, float y, float z)
{
    GLvector3D vector;
    vector.x = x;
    vector.y = y;
    vector.z = z;

    m_lineParameters->vertices.append(vector);

    Parameters *parameters = addDrawableData(m_lineParameters);
    resetTransformations();
    return parameters;
}

void *QGLView::line(const QVector3D &vector)
{
    return line(vector.x(), vector.y(), vector.z());
}

void *QGLView::lineTo(float x, float y, float z)
{
    if (!m_pathEnabled)
    {
        GLvector3D lastVector;
        GLvector3D vector;

        if (m_lineParameters->vertices.size() > 1)
        {
            lastVector = m_lineParameters->vertices.takeLast();
        }
        else
        {
            lastVector = m_lineParameters->vertices.last();
        }

        vector.x = x - lastVector.x;
        vector.y = y - lastVector.y;
        vector.z = z - lastVector.z;
        m_lineParameters->vertices.append(vector);

        Parameters *parameters = addDrawableData(m_lineParameters);
        m_lineParameters->modelMatrix.translate(vector.x,
                                                vector.y,
                                                vector.z);
        vector.x = x;
        vector.y = y;
        vector.z = z;
        m_lineParameters->vertices.removeLast();
        m_lineParameters->vertices.append(vector);

        return parameters;
    }
    else
    {
        GLvector3D vector;
        vector.x = x;
        vector.y = y;
        vector.z = z;
        m_lineParameters->vertices.append(vector);
    }

    return NULL;
}

void *QGLView::lineTo(const QVector3D &vector)
{
    return lineTo(vector.x(), vector.y(), vector.z());
}

void *QGLView::lineFromTo(float x1, float y1, float z1, float x2, float y2, float z2)
{
    return lineFromTo(QVector3D(x1, y1, z1), QVector3D(x2, y2, z2));
}

void *QGLView::lineFromTo(const QVector3D &startPosition, const QVector3D &endPosition)
{
    QVector3D diffVector = endPosition - startPosition;
    GLvector3D vector;
    vector.x = diffVector.x();
    vector.y = diffVector.y();
    vector.z = diffVector.z();
    m_lineParameters->vertices.append(vector);

    m_lineParameters->modelMatrix.translate(startPosition);
    Parameters *parameters = addDrawableData(m_lineParameters);
    resetTransformations();
    return parameters;
}

void QGLView::beginPath()
{
    m_pathEnabled = true;
}

void *QGLView::endPath()
{
    m_pathEnabled = false;
    Parameters *parameters = addDrawableData(m_lineParameters);
    resetTransformations();
    return parameters;
}

void QGLView::text(QString text, TextAlignment alignment , QFont font)
{
    QStaticText staticText(text);
    font.setPixelSize(100);
    staticText.prepare(QTransform(), font);

    if (!m_textTextList.contains(staticText))
    {
        generateTextTexture(staticText, font);
    }

    m_textParameters->staticText = staticText;
    m_textParameters->alignment = alignment;

    addDrawableData(m_textParameters);
    resetTransformations();
}

void QGLView::beginUnion()
{
    m_modelParametersStack.push(new Parameters(m_modelParameters));
    m_lineParametersStack.push(new LineParameters(m_lineParameters));
    m_textParametersStack.push(new TextParameters(m_textParameters));
}

void QGLView::endUnion()
{
    delete m_modelParametersStack.pop();
    m_modelParameters = new Parameters(m_modelParametersStack.top());

    delete m_lineParametersStack.pop();
    m_lineParameters = new LineParameters(m_lineParametersStack.top());

    delete m_textParametersStack.pop();
    m_textParameters = new TextParameters(m_textParametersStack.top());
}

void QGLView::updateColor(void *drawablePointer, const QColor &color)
{
    Parameters *parameters;

    parameters = static_cast<Parameters*>(drawablePointer);
    parameters->color = color;
}

void QGLView::paint()
{
    GLboolean scissorEnabled;
    GLboolean depthTestEnabled;
    GLint depthFunc;
    GLboolean depthMask;
    GLboolean cullFaceEnabled;


    if (!m_initialized) {
        setupShaders();
        setupWindow();
        setupVBOs();
        setupStack();
        m_initialized = true;
        emit initialized();
    }

    glScissor(this->x(), window()->height() - this->y() - this->height(), this->width(), this->height());

    glGetBooleanv(GL_SCISSOR_TEST, &scissorEnabled);
    glEnable(GL_SCISSOR_TEST);

    //glViewport(this->x(), window()->height() - this->y() - this->height(), this->width(), this->height());

    glClearColor(0.0, 0.0, 0.0, 1.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Enable depth test
    glGetBooleanv(GL_DEPTH_TEST, &depthTestEnabled);
    glEnable(GL_DEPTH_TEST);
    glGetIntegerv(GL_DEPTH_FUNC, &depthFunc);
    glDepthFunc(GL_LEQUAL);
    glGetBooleanv(GL_DEPTH_WRITEMASK, &depthMask);
    glDepthMask(GL_TRUE);

    // Enable back face culling
    glGetBooleanv(GL_CULL_FACE, &cullFaceEnabled);
    glEnable(GL_CULL_FACE);

    // Enable Alpha blend
    //glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    //glEnable(GL_BLEND);

    if (m_selectionModeActive)
    {
        m_currentDrawableId = 1;    // we start by one since 0 is the background color
    }

    m_lineProgram->bind();
    m_lineProgram->setUniformValue(m_lineProjectionMatrixLocation, m_projectionMatrix);
    m_lineProgram->setUniformValue(m_lineViewMatrixLocation, m_viewMatrix);
    m_lineProgram->setUniformValue(m_lineSelectionModeLocation, m_selectionModeActive);
    drawLines();
    m_lineProgram->release();

    m_textProgram->bind();
    m_textProgram->setUniformValue(m_textProjectionMatrixLocation, m_projectionMatrix);
    m_textProgram->setUniformValue(m_textViewMatrixLocation, m_viewMatrix);
    m_textProgram->setUniformValue(m_textSelectionModeLocation, m_selectionModeActive);
    //drawTexts();
    m_textProgram->release();

    m_modelProgram->bind();
    m_modelProgram->setUniformValue(m_projectionMatrixLocation, m_projectionMatrix);
    m_modelProgram->setUniformValue(m_viewMatrixLocation, m_viewMatrix);
    m_modelProgram->setUniformValue(m_lightPositionLocation, m_light->position());
    m_modelProgram->setUniformValue(m_lightIntensitiesLocation, m_light->intensities());
    m_modelProgram->setUniformValue(m_lightAttenuationLocation, m_light->attenuation());
    m_modelProgram->setUniformValue(m_lightAmbientCoefficientLocation, m_light->ambientCoefficient());
    m_modelProgram->setUniformValue(m_lightEnabledLocation, m_light->enabled());
    m_modelProgram->setUniformValue(m_selectionModeLocation, m_selectionModeActive);
    drawDrawables(Cube);
    drawDrawables(Cylinder);
    drawDrawables(Cone);
    drawDrawables(Sphere);
    m_modelProgram->release();

    if (m_selectionModeActive)
    {
        quint32 id = getSelection();
        emit drawableSelected(m_drawableIdMap.value(id, NULL));

        m_drawableIdMap.clear();
        m_selectionModeActive = false;
        paint();
    }

    if (!scissorEnabled)
    {
        glDisable(GL_SCISSOR_TEST);
    }
    glClear(GL_SCISSOR_BIT);

    if (!depthTestEnabled)
    {
        glDisable(GL_DEPTH_TEST);
    }
    glDepthFunc(depthFunc);
    glDepthMask(depthMask);

    if (!cullFaceEnabled)
    {
        glDisable(GL_CULL_FACE);
    }
}

void QGLView::cleanup()
{
    if (m_modelProgram) {
        delete m_modelProgram;
        m_modelProgram = 0;
    }

    if (m_lineProgram) {
        delete m_lineProgram;
        m_lineProgram = 0;
    }

    if (m_textProgram) {
        delete m_textProgram;
        m_textProgram = 0;
    }
}

void QGLView::sync()
{
    m_thread_t = m_t;
}

void QGLView::reset()
{
    removeDrawables(m_currentDrawableList);
}
