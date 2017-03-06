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

#ifndef GLVIEW_H
#define GLVIEW_H

#include <QtQuick/QQuickPaintedItem>
#include <QtGui/QOpenGLShaderProgram>
#include <QTimer>
#include <QOpenGLBuffer>
#include <QOpenGLTexture>
#include <QOpenGLFunctions>
#include <QStack>
#include <QStaticText>
#include <QPainter>
#include <QQmlListProperty>
#include <QSignalMapper>
#include "glitem.h"
#include "qglcamera.h"
#include "gllight.h"

namespace qtquickvcp {

class GLItem;

class GLView : public QQuickPaintedItem, protected QOpenGLFunctions
{
    Q_OBJECT

    Q_INTERFACES(QQmlParserStatus)
    Q_PROPERTY(QColor backgroundColor READ backgroundColor WRITE setBackgroundColor NOTIFY backgroundColorChanged)
    Q_PROPERTY(QGLCamera *camera READ camera WRITE setCamera NOTIFY cameraChanged)
    Q_PROPERTY(GLLight *light READ light WRITE setLight NOTIFY lightChanged)
    Q_PROPERTY(QQmlListProperty<qtquickvcp::GLItem> glItems READ glItems NOTIFY glItemsChanged)
    Q_ENUMS(TextAlignment)

public:
    GLView(QQuickItem *parent = 0);
    ~GLView();

    enum TextAlignment {
        AlignLeft = 0,
        AlignCenter = 1,
        AlignRight = 2
    };

    QColor backgroundColor() const { return m_backgroundColor; }
    void setBackgroundColor(const QColor &backgroundColor);

    Q_INVOKABLE void readPixel(int x, int y);

    QGLCamera* camera()
    {
        return m_camera;
    }

    GLLight * light() const
    {
        return m_light;
    }

    QQmlListProperty<GLItem> glItems();
    int glItemCount() const;
    GLItem *glItem(int index) const;

    void paint(QPainter * painter);

signals:
    void backgroundColorChanged();
    void cameraChanged(QGLCamera *arg);
    void glItemsChanged(QQmlListProperty<GLItem> arg);
    void lightChanged(GLLight * arg);
    void initialized();
    void drawableSelected(void *pointer);

public slots:
    void paint();
    void cleanup();
    void sync();

    // item handling
    void addGlItem(GLItem *glItem);
    void removeGlItem(int index);
    void removeGlItem(GLItem *glItem);
    void clearGlItems();

    // loading the correct current drawables list
    void prepare(GLItem *glItem);

    // removing drawables
    void reset();

    // model and line transformation functions
    void color(float r, float g, float b, float a);
    void color(const QColor &color);
    void translate(float x, float y, float z);
    void translate(const QVector3D &vector);
    void rotate(float angle, float x, float y, float z);
    void rotate(float angle, const QVector3D &axis);
    void rotate(const QQuaternion &quaternion);
    void scale(float x, float y, float z);
    void scale(const QVector3D &vector);
    void mirror(float x, float y, float z);
    void mirror(const QVector3D &vector);
    void resetTransformations(bool hard = false);

    // model functions
    void *cube(float w, float l, float h, bool center = false);
    void *cube(const QVector3D &size, bool center = false);
    void *cylinder(float r, float h);
    void *cone(float r, float h);
    void *sphere(float r);

    // line functions
    void lineWidth(float width);
    void lineStipple(bool enable, float length = 5.0);
    void *line(float x, float y, float z);
    void *line(const QVector3D &vector);
    void* lineTo(float x, float y, float z);
    void *lineTo(const QVector3D &vector);
    void *lineFromTo(float x1, float y1, float z1, float x2, float y2, float z2);
    void *lineFromTo(const QVector3D &startPosition, const QVector3D &endPosition);
    void beginPath();
    void *endPath();
    void *arc(float x, float y, float radius, float startAngle, float endAngle, bool anticlockwise, float helixOffset = 0.0);

    // text functions
    void text(QString text, TextAlignment alignment = AlignLeft, QFont font = QFont());

    // grouping functions
    void beginUnion();
    void endUnion();

    // update functions
    void updateColor(void *drawablePointer, const QColor &color);

    void setCamera(QGLCamera *arg)
    {
        if (m_camera != arg) {
            m_camera = arg;
            emit cameraChanged(arg);
            connect(m_camera, SIGNAL(projectionChanged()),
                    this, SLOT(updateProjectionMatrix()));
            connect(m_camera, SIGNAL(viewChanged()),
                    this, SLOT(updateViewMatrix()));
        }
    }

    void setLight(GLLight * arg)
    {
        if (m_light != arg) {
            m_light = arg;
            emit lightChanged(arg);
            connect(m_light, SIGNAL(propertyChanged()),
                    this, SLOT(update()));
        }
    }

private slots:
    void handleWindowChanged(QQuickWindow *win);
    void updatePerspectiveAspectRatio();
    void updateViewMatrix();
    void updateProjectionMatrix();
    void updateItems();
    void updateItem(QObject *item);
    void updateChildren();

private:
    enum ModelType {
        NoType = 0,
        Cube = 1,
        Cylinder = 2,
        Sphere = 3,
        Cone = 4,
        Text = 5,
        Line = 6
    };

    typedef struct {
        GLfloat x;
        GLfloat y;
    } GLvector2D;

    typedef struct {
        GLfloat x;
        GLfloat y;
        GLfloat z;
    } GLvector3D;

    typedef struct {
        GLubyte r;
        GLubyte g;
        GLubyte b;
    } GLcolorRGB ;

    typedef struct {
        GLvector3D position;
        GLvector3D normal;
    } ModelVertex;

    typedef struct {
        GLvector3D position;
        GLvector2D texCoordinate;
    } TextVertex;

    class Parameters {
    public:
        Parameters():
            creator(nullptr),
            modelMatrix(QMatrix4x4()),
            color(QColor(Qt::yellow)),
            deleteFlag(false)
        { }

        Parameters(Parameters *parameters)
        {
            creator = parameters->creator;
            modelMatrix = parameters->modelMatrix;
            color = parameters->color;
            deleteFlag = parameters->deleteFlag;
        }

        GLItem *creator;
        QMatrix4x4 modelMatrix;
        QColor color;
        bool deleteFlag;    // marks the parameter to delete
    };

    class LineParameters: public Parameters {
    public:
        LineParameters():
            Parameters(),
            width(1.0),
            stipple(false),
            stippleLength(1.0)
        {
            GLvector3D vector;
            vector.x = 0.0;
            vector.y = 0.0;
            vector.z = 0.0;
            vertices.append(vector);
            color = QColor(Qt::red);
        }

        LineParameters(LineParameters *parameters):
            Parameters(parameters)
        {
            vertices = parameters->vertices;
            width = parameters->width;
            stipple = parameters->stipple;
            stippleLength = parameters->stippleLength;
        }

        QVector<GLvector3D> vertices;
        GLfloat width;
        bool stipple;
        GLfloat stippleLength;
    };

    class TextParameters: public Parameters {
    public:
        TextParameters():
            Parameters(),
            staticText(QStaticText()),
            alignment(AlignLeft)
        {
            color = QColor(Qt::white);
        }

        TextParameters(TextParameters *parameters):
            Parameters(parameters)
        {
            staticText = parameters->staticText;
            alignment = parameters->alignment;
        }

        QStaticText staticText;
        TextAlignment alignment;
    };

    typedef struct {
        ModelType type;
        Parameters *parameters;
    } Drawable;

    bool m_initialized;

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
    float m_projectionAspectRatio;

    // shader program location ids
    int m_positionLocation;
    int m_normalLocation;
    int m_colorLocation;
    int m_lightPositionLocation;
    int m_lightIntensitiesLocation;
    int m_lightAttenuationLocation;
    int m_lightAmbientCoefficientLocation;
    int m_lightEnabledLocation;
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
    int m_lineSelectionModeLocation;
    int m_lineIdColorLocation;

    int m_textProjectionMatrixLocation;
    int m_textViewMatrixLocation;
    int m_textModelMatrixLocation;
    int m_textColorLocation;
    int m_textPositionLocation;
    int m_textTexCoordinateLocation;
    int m_textTextureLocation;
    int m_textAspectRatioLocation;
    int m_textAlignmentLocation;
    int m_textSelectionModeLocation;
    int m_textIdColorLocation;

    // thread secure properties
    QColor m_backgroundColor;
    QColor m_thread_backgroundColor;

    QSize m_viewportSize;

    // generic map of drawable items
    QMap<ModelType, QList<Parameters*>* > m_drawableMap;

    // model stack
    Parameters *m_modelParameters;
    QStack<Parameters*> m_modelParametersStack;

    // line stack
    bool m_pathEnabled;
    LineParameters *m_lineParameters;
    QStack<LineParameters*> m_lineParametersStack;

    // text stack
    TextParameters *m_textParameters;
    QStack<TextParameters*> m_textParametersStack;
    QList<QStaticText> m_textTextList;
    QList<QImage> m_textImageList;
    QList<QOpenGLTexture*> m_textTextureList;
    QList<float> m_textAspectRatioList;

    // item selection
    quint32 m_currentDrawableId;
    QMap<quint32, Parameters* > m_drawableIdMap;
    QPoint m_selectionPoint;
    bool m_selectionModeActive;

    //GL items
    GLItem *m_currentGlItem;
    QList<GLItem*> m_glItems;
    QMap<GLItem*, QList<Drawable>* > m_drawableListMap;
    QList<Drawable> *m_currentDrawableList;
    QSignalMapper *m_propertySignalMapper;
    QList<GLItem*> m_modifiedGlItems;  // list of gl items that have been modified

    // camera
    QGLCamera *m_camera;

    // light
    GLLight *m_light;

    void addDrawableList(ModelType type);
    QList<Parameters*>* getDrawableList(ModelType type);
    Parameters *addDrawableData(const LineParameters & parameters);
    Parameters *addDrawableData(const TextParameters & parameters);
    Parameters *addDrawableData(ModelType type, const Parameters & parameters);

    void drawDrawables(ModelType type = NoType);
    void clearDrawables();
    void cleanupDrawables(ModelType type);
    void removeDrawables(QList<Drawable> *drawableList);

    void drawModelVertices(ModelType type);

    void drawLines();

    void drawTexts();
    void prepareTextTexture(const QStaticText &staticText, QFont font);
    void createTextTexture(TextParameters *textParameters);
    void clearTextTextures();

    void updateGLItems();
    void clearGLItem(GLItem *item);
    void updateGLItem(GLItem *item);
    void paintGLItems();
    void paintGLItem(GLItem *item);

    quint32 getSelection();

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
}; // class GLView
} // namespace qtquickvcp

#endif // GLVIEW_H
