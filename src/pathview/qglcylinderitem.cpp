#include "qglcylinderitem.h"

QGLCylinderItem::QGLCylinderItem(QQuickItem *parent) :
    QGLItem(parent),
    m_radius(1.0),
    m_height(1.0),
    m_color(QColor(Qt::yellow)),
    m_cone(false)
{
}

QGLCylinderItem::QGLCylinderItem(float radius, float height, QQuickItem *parent) :
    QGLItem(parent),
    m_color(QColor(Qt::yellow)),
    m_cone(false)
{
    m_radius = radius;
    m_height = height;
}

QGLCylinderItem::QGLCylinderItem(float radius, float height, QColor color, QQuickItem *parent) :
    QGLItem(parent),
    m_cone(false)
{
    m_radius = radius;
    m_height = height;
    m_color = color;
}

void QGLCylinderItem::paint(QGLView *glView)
{
    glView->translate(position());
    glView->rotate(rotation());
    glView->scale(scale());

    glView->beginUnion(modelId());
    glView->color(m_color);
    if (!m_cone)
    {
        glView->cylinder(m_radius, m_height);
    }
    else
    {
        glView->cone(m_radius, m_height);
    }
    glView->endUnion();
}
