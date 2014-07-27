#include "qglitem.h"

QGLItem::QGLItem(QQuickItem *parent) :
    QQuickItem(parent),
    m_position(QVector3D(0,0,0)),
    m_scale(QVector3D(1,1,1)),
    m_rotation(QQuaternion()),
    m_rotationAngle(0),
    m_rotationAxis(QVector3D())
{
    connect(this, SIGNAL(positionChanged(QVector3D)),
            this, SIGNAL(needsUpdate()));
    connect(this, SIGNAL(positionChanged(QVector3D)),
            this, SIGNAL(needsUpdate()));
    connect(this, SIGNAL(scaleChanged()),
            this, SIGNAL(needsUpdate()));
    connect(this, SIGNAL(rotationChanged(QQuaternion)),
            this, SIGNAL(needsUpdate()));
    connect(this, SIGNAL(rotationAngleChanged(float)),
            this, SIGNAL(needsUpdate()));
    connect(this, SIGNAL(rotationAxisChanged(QVector3D)),
            this, SIGNAL(needsUpdate()));
    connect(this, SIGNAL(visibleChanged()),
            this, SIGNAL(needsUpdate()));
}

void QGLItem::requestPaint()
{
    emit needsUpdate();
}
