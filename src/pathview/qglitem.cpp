#include "qglitem.h"

QGLItem::QGLItem(QQuickItem *parent) :
    QQuickItem(parent),
    m_modelId(0),
    m_position(QVector3D(0,0,0)),
    m_scale(QVector3D(1,1,1)),
    m_rotation(QQuaternion()),
    m_rotationAngle(0),
    m_rotationAxis(QVector3D())
{
    connect(this, SIGNAL(positionChanged(QVector3D)),
            this, SIGNAL(propertyChanged()));
    connect(this, SIGNAL(positionChanged(QVector3D)),
            this, SIGNAL(propertyChanged()));
    connect(this, SIGNAL(scaleChanged()),
            this, SIGNAL(propertyChanged()));
    connect(this, SIGNAL(rotationChanged(QQuaternion)),
            this, SIGNAL(propertyChanged()));
    connect(this, SIGNAL(rotationAngleChanged(float)),
            this, SIGNAL(propertyChanged()));
    connect(this, SIGNAL(rotationAxisChanged(QVector3D)),
            this, SIGNAL(propertyChanged()));
    connect(this, SIGNAL(visibleChanged()),
            this, SIGNAL(propertyChanged()));
}
