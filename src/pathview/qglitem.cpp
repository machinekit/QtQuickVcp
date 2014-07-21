#include "qglitem.h"

QGLItem::QGLItem(QObject *parent) :
    QObject(parent),
    m_modelId(0),
    m_position(QVector3D(0,0,0)),
    m_scale(QVector3D(1,1,1)),
    m_rotation(QQuaternion())
{
}
