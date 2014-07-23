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

QGLView::QGLView()
    : m_modelProgram(0)
    , m_t(0)
    , m_thread_t(0)
    , m_camera(new QGLCamera(this))
    , m_projectionAspectRatio(1.0)
{
    connect(this, SIGNAL(windowChanged(QQuickWindow*)), this, SLOT(handleWindowChanged(QQuickWindow*)));
    connect(this, SIGNAL(childrenChanged()), this, SLOT(updateChildren()));
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
        connect(win, SIGNAL(widthChanged(int)), this, SLOT(updatePerspectiveAspectRatio()));
        connect(win, SIGNAL(heightChanged(int)), this, SLOT(updatePerspectiveAspectRatio()));

        // If we allow QML to do the clearing, they would clear what we paint
        // and nothing would show.
        win->setClearBeforeRendering(false);

        updatePerspectiveAspectRatio();
    }
}

void QGLView::updatePerspectiveAspectRatio()
{
    qreal ratio = window()->devicePixelRatio();
    int w = int(ratio * window()->width());
    int h = int(ratio * window()->height());
    m_projectionAspectRatio = (float)w/(float)h;
}

void QGLView::updateChildren()
{
    m_glItems.clear();

    QList<QQuickItem*> objectChildren = childItems();
    QGLItem* glItem;
    for (int i = 0; i < objectChildren.size(); ++i)
    {

        glItem = qobject_cast<QGLItem*>(objectChildren.at(i));
        if (glItem != NULL)
        {
            addGlItem(glItem);
        }
    }

    emit glItemsChanged(glItems());
}

void QGLView::clearModels(QGLView::ModelType type = NoType)
{
    if (type == NoType)
    {
        QMapIterator<ModelType, QList<ModelParameters>* > i(m_modelMap);
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
        QMapIterator<ModelType, QList<ModelParameters>* > i(m_modelMap);
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
    setupLineVertexBuffer();
    setupTextVertexBuffer();
}

void QGLView::setupLineVertexBuffer()
{
    m_lineVertexBuffer = new QOpenGLBuffer(QOpenGLBuffer::VertexBuffer);
    m_lineVertexBuffer->create();
    m_lineVertexBuffer->setUsagePattern(QOpenGLBuffer::StaticDraw);
    m_lineVertexBuffer->bind();
    m_lineVertexBuffer->allocate(2*sizeof(Vector3D));
    m_lineVertexBuffer->release();
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

    QList<ModelParameters> *modelParametersList = new QList<ModelParameters>();
    m_modelMap.insert(Cube, modelParametersList);
}

void QGLView::drawModelVertices(ModelType type)
{
    QOpenGLBuffer *vertexBuffer = m_vertexBufferMap[type];
    QList<ModelParameters> *modelParametersList = m_modelMap[type];

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
        ModelParameters modelParameters = modelParametersList->at(i);
        m_modelProgram->setUniformValue(m_colorLocation, modelParameters.color);
        m_modelProgram->setUniformValue(m_modelMatrixLocation, modelParameters.modelMatrix);
        m_modelProgram->setUniformValue(m_idColorLocation, QColor(0xFF000000u + modelParameters.id));    // color for selection mode
        glDrawArrays(GL_TRIANGLES, 0, vertexBuffer->size()/sizeof(ModelVertex));
    }

    m_modelProgram->disableAttributeArray(m_positionLocation);
    m_modelProgram->disableAttributeArray(m_normalLocation);
    vertexBuffer->release();
}

void QGLView::clearLines()
{
    m_lineParametersList.clear();

}

void QGLView::drawLines()
{
    Vector3D vertices[2] = {{0.0f, 0.0f, 0.0f},
                            {0.0f, 0.0f, 0.0f}};

    if (m_lineParametersList.isEmpty())
    {
        return;
    }

    m_lineVertexBuffer->bind();
    m_lineProgram->enableAttributeArray(m_linePositionLocation);
    m_lineProgram->setAttributeBuffer(m_linePositionLocation, GL_FLOAT, 0, 3);

    for (int i = 0; i < m_lineParametersList.size(); ++i)
    {
        LineParameters lineParameters = m_lineParametersList.at(i);
        vertices[1] = lineParameters.vector;
        m_lineVertexBuffer->write(0, vertices, 2*sizeof(Vector3D));
        m_lineProgram->setUniformValue(m_lineColorLocation, lineParameters.color);
        m_lineProgram->setUniformValue(m_lineModelMatrixLocation, lineParameters.modelMatrix);
        m_lineProgram->setUniformValue(m_lineStippleLocation, lineParameters.stipple);
        m_lineProgram->setUniformValue(m_lineStippleLengthLocation, lineParameters.stippleLength);
        glLineWidth(lineParameters.width);
        glDrawArrays(GL_LINE_STRIP, 0, 2);
    }

    m_lineProgram->disableAttributeArray(m_linePositionLocation);
    m_lineVertexBuffer->release();
}

void QGLView::clearTexts()
{
    m_textParametersList.clear();
}

void QGLView::drawTexts()
{
    if (m_textParametersList.isEmpty())
    {
        return;
    }

    m_textVertexBuffer->bind();
    m_textProgram->enableAttributeArray(m_textPositionLocation);
    m_textProgram->enableAttributeArray(m_textTexCoordinateLocation);
    m_textProgram->setAttributeBuffer(m_textPositionLocation, GL_FLOAT, 0, 3, sizeof(TextVertex));
    m_textProgram->setAttributeBuffer(m_textTexCoordinateLocation, GL_FLOAT, 3*sizeof(GLfloat), 2, sizeof(TextVertex));

    for (int i = 0; i < m_textParametersList.size(); ++i)
    {
        TextParameters textParameters = m_textParametersList.at(i);
        QStaticText staticText = textParameters.staticText;
        int textureIndex = m_textTextList.indexOf(staticText);
        QOpenGLTexture *texture = m_textTextureList.at(textureIndex);
        float aspectRatio = m_textAspectRatioList.at(textureIndex);

        m_textProgram->setUniformValue(m_textAspectRatioLocation, aspectRatio);
        m_textProgram->setUniformValue(m_textColorLocation, textParameters.color);
        m_textProgram->setUniformValue(m_textModelMatrixLocation, textParameters.modelMatrix);
        m_textProgram->setUniformValue(m_textTextureLocation, texture->textureId());
        texture->bind(texture->textureId());
        glDrawArrays(GL_TRIANGLES, 0, m_textVertexBuffer->size()/sizeof(TextVertex));
        texture->release();
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

    // search for indexes still in use
    for (int i = 0; i < m_textParametersList.size(); ++i)
    {
        TextParameters textParameters = m_textParametersList.at(i);

        if (m_textTextList.contains(textParameters.staticText))
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

void QGLView::paintItems()
{
    for (int i = 0; i < m_glItems.size(); ++i)
    {
        m_glItems.at(i)->paint(this);
    }
}

void QGLView::addGlItem(QGLItem *item)
{
    m_glItems.append(item);
    emit glItemsChanged(glItems());
}

void QGLView::removeGlItem(int index)
{
    m_glItems.removeAt(index);
    emit glItemsChanged(glItems());
}

void QGLView::removeGlItem(QGLItem *item)
{
    for (int i = 0; i < m_glItems.size(); ++i)
    {
        if (m_glItems.at(i) == item)
        {
            m_glItems.removeAt(i);
            emit glItemsChanged(glItems());
            return;
        }
    }
}

void QGLView::clearGlItems()
{
    m_glItems.clear();
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

    QList<ModelParameters> *modelParametersList = new QList<ModelParameters>();
    m_modelMap.insert(type, modelParametersList);
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

    QList<ModelParameters> *modelParametersList = new QList<ModelParameters>();
    m_modelMap.insert(Sphere, modelParametersList);
}

void QGLView::setupStack()
{
    ModelParameters defaultModelParameters;

    defaultModelParameters.modelMatrix = QMatrix4x4();
    defaultModelParameters.color = QColor(Qt::yellow);
    defaultModelParameters.id = 0;

    m_modelParametersStack.clear();
    m_modelParametersStack.push(defaultModelParameters);
    m_modelParameters = defaultModelParameters;

    LineParameters defaultLineParameters;

    defaultLineParameters.modelMatrix = QMatrix4x4();
    defaultLineParameters.color = QColor(Qt::red);
    defaultLineParameters.stipple = false;
    defaultLineParameters.stippleLength = 5;
    defaultLineParameters.width = 1;

    m_lineParametersStack.clear();
    m_lineParametersStack.push(defaultLineParameters);
    m_lineParameters = defaultLineParameters;

    TextParameters defaultTextParameters;

    defaultTextParameters.modelMatrix = QMatrix4x4();
    defaultTextParameters.color = QColor(Qt::white);
    defaultTextParameters.staticText = QStaticText();

    m_textParametersStack.clear();
    m_textParametersStack.push(defaultTextParameters);
    m_textParameters = defaultTextParameters;
}

void QGLView::color(float r, float g, float b, float a)
{
    color(QColor((int)(255.0*r), (int)(255.0*g), (int)(255.0*b), (int)(255.0*a)));
}

void QGLView::color(QColor color)
{
    m_modelParameters.color = color;
    m_lineParameters.color = color;
    m_textParameters.color = color;
}

void QGLView::translate(float x, float y, float z)
{
    translate(QVector3D(x, y, z));
}

void QGLView::translate(QVector3D vector)
{
    m_modelParameters.modelMatrix.translate(vector);
    m_lineParameters.modelMatrix.translate(vector);
    m_textParameters.modelMatrix.translate(vector);
}

void QGLView::rotate(float angle, float x, float y, float z)
{
    rotate(angle, QVector3D(x, y, z));
}

void QGLView::rotate(float angle, QVector3D axis)
{
    m_modelParameters.modelMatrix.rotate(angle, axis);
    m_lineParameters.modelMatrix.rotate(angle, axis);
    m_textParameters.modelMatrix.rotate(angle, axis);
}

void QGLView::rotate(QQuaternion quaternion)
{
    m_modelParameters.modelMatrix.rotate(quaternion);
    m_lineParameters.modelMatrix.rotate(quaternion);
    m_textParameters.modelMatrix.rotate(quaternion);
}

void QGLView::scale(float x, float y, float z)
{
    scale(QVector3D(x, y, z));
}

void QGLView::scale(QVector3D vector)
{
    m_modelParameters.modelMatrix.scale(vector);
    m_lineParameters.modelMatrix.scale(vector);
    m_textParameters.modelMatrix.scale(vector);
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

    m_modelParameters.modelMatrix.scale((x == 1) ? -1.0 : 1.0,
                                   (y == 1) ? -1.0 : 1.0,
                                   (z == 1) ? -1.0 : 1.0);
    m_lineParameters.modelMatrix.scale((x == 1) ? -1.0 : 1.0,
                                   (y == 1) ? -1.0 : 1.0,
                                   (z == 1) ? -1.0 : 1.0);
    m_textParameters.modelMatrix.scale((x == 1) ? -1.0 : 1.0,
                                   (y == 1) ? -1.0 : 1.0,
                                   (z == 1) ? -1.0 : 1.0);
}

void QGLView::resetTransformations(bool hard)
{
    if (hard)
    {
        m_modelParameters = m_modelParametersStack.first();
        m_modelParametersStack.clear();
        m_modelParametersStack.push(m_modelParameters);

        m_lineParameters = m_lineParametersStack.first();
        m_lineParametersStack.clear();
        m_lineParametersStack.push(m_lineParameters);

        m_textParameters = m_textParametersStack.first();
        m_textParametersStack.clear();
        m_textParametersStack.push(m_textParameters);
    }
    else
    {
        m_modelParameters = m_modelParametersStack.top();
        m_lineParameters = m_lineParametersStack.top();
        m_textParameters = m_textParametersStack.top();
    }
}

void QGLView::cube(float w, float l, float h, bool center)
{
    cube(QVector3D(w, l, h), center);
}

void QGLView::cube(QVector3D size, bool center)
{
    if (center)
    {
        m_modelParameters.modelMatrix.translate(-size/2.0);
    }
    m_modelParameters.modelMatrix.scale(size);

    m_modelMap[Cube]->append(m_modelParameters);

    resetTransformations();
}

void QGLView::cylinder(float r, float h)
{
    m_modelParameters.modelMatrix.scale(r, r, h);
    m_modelMap[Cylinder]->append(m_modelParameters);
    resetTransformations();
}

void QGLView::cone(float r, float h)
{
    m_modelParameters.modelMatrix.scale(r, r, h);
    m_modelMap[Cone]->append(m_modelParameters);
    resetTransformations();
}

void QGLView::sphere(float r)
{
    m_modelParameters.modelMatrix.scale(r,r,r);
    m_modelMap[Sphere]->append(m_modelParameters);
    resetTransformations();
}

void QGLView::lineWidth(float width)
{
    m_lineParameters.width = width;
}

void QGLView::lineStipple(float enable, float length)
{
    m_lineParameters.stipple = enable;
    m_lineParameters.stippleLength = length;
}

void QGLView::line(float x, float y, float z)
{
    m_lineParameters.vector.x = x;
    m_lineParameters.vector.y = y;
    m_lineParameters.vector.z = z;

    m_lineParametersList.append(m_lineParameters);
    resetTransformations();
}

void QGLView::line(QVector3D vector)
{
    line(vector.x(), vector.y(), vector.z());
}

void QGLView::text(QString text, QFont font)
{
    QStaticText staticText(text);
    font.setPixelSize(100);
    staticText.prepare(QTransform(), font);

    if (!m_textTextList.contains(staticText))
    {
        generateTextTexture(staticText, font);
    }

    m_textParameters.staticText = staticText;

    m_textParametersList.append(m_textParameters);
    resetTransformations();
}

void QGLView::beginUnion(quint32 id = 0)
{
    m_modelParameters.id = id;
    m_modelParametersStack.push(m_modelParameters);

    m_lineParametersStack.push(m_lineParameters);

    m_textParametersStack.push(m_textParameters);
}

void QGLView::endUnion()
{
    m_modelParametersStack.pop();
    m_modelParameters = m_modelParametersStack.top();

    m_lineParametersStack.pop();
    m_lineParameters = m_lineParametersStack.top();

    m_textParametersStack.pop();
    m_textParameters = m_textParametersStack.top();
}

void QGLView::paint()
{
    if (!m_modelProgram) {
        setupShaders();
        setupWindow();
        setupVBOs();
        setupStack();
    }
    //glViewport(0, 0, w, h);

    m_projectionMatrix = m_camera->projectionMatrix(m_projectionAspectRatio);//QMatrix4x4();
    //m_projectionMatrix.frustum(-3, 3, -matrixH/2, matrixH/2, 4, 10);

    m_viewMatrix = m_camera->modelViewMatrix();//QMatrix4x4();
   // m_viewMatrix.translate(0,0,0);

    glClearColor(0.0, 0.0, 0.0, 1.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Enable depth test
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
    glDepthMask(true);

    // Enable back face culling
    glEnable(GL_CULL_FACE);

    // Enable Alpha blend
    //glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    //glEnable(GL_BLEND);

    clearLines();
    clearModels();
    clearTexts();
    resetTransformations(true); // reset all tranformations for a clean start

    paintItems();

    m_lineProgram->bind();
    m_lineProgram->setUniformValue(m_lineProjectionMatrixLocation, m_projectionMatrix);
    m_lineProgram->setUniformValue(m_lineViewMatrixLocation, m_viewMatrix);
    drawLines();
    m_lineProgram->release();

    m_textProgram->bind();
    m_textProgram->setUniformValue(m_textProjectionMatrixLocation, m_projectionMatrix);
    m_textProgram->setUniformValue(m_textViewMatrixLocation, m_viewMatrix);
    drawTexts();
    m_textProgram->release();

    m_light.enabled = true;
    m_light.ambientCoefficient = 0.1;
    m_light.attenuation = 0.01;
    m_light.intensities = QVector3D(3.0, 3.0, 10.0 );
    m_light.position = QVector3D(1.0, 1.0, 5);

    m_modelProgram->bind();
    m_modelProgram->setUniformValue(m_projectionMatrixLocation, m_projectionMatrix);
    m_modelProgram->setUniformValue(m_viewMatrixLocation, m_viewMatrix);
    m_modelProgram->setUniformValue(m_lightPositionLocation, m_light.position);
    m_modelProgram->setUniformValue(m_lightIntensitiesLocation, m_light.intensities);
    m_modelProgram->setUniformValue(m_lightAttenuationLocation, m_light.attenuation);
    m_modelProgram->setUniformValue(m_lightAmbientCoefficientLocation, m_light.ambientCoefficient);
    m_modelProgram->setUniformValue(m_lightEnabledLocation, m_light.enabled);
    m_modelProgram->setUniformValue(m_selectionModeLocation, false);
    drawModels();
    m_modelProgram->release();
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

