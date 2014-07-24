#include "qglcylinderitem.h"

QGLCylinderItem::QGLCylinderItem(QQuickItem *parent) :
    QGLItem(parent),
    m_radius(1.0),
    m_height(1.0),
    m_color(QColor(Qt::yellow)),
    m_cone(false)
{
    connect(this, SIGNAL(radiusChanged(float)),
            this, SIGNAL(propertyChanged()));
    connect(this, SIGNAL(heightChanged()),
            this, SIGNAL(propertyChanged()));
    connect(this, SIGNAL(colorChanged(QColor)),
            this, SIGNAL(propertyChanged()));
    connect(this, SIGNAL(coneChanged(bool)),
            this, SIGNAL(propertyChanged()));
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
