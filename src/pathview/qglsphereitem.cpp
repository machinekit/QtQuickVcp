#include "qglsphereitem.h"

QGLSphereItem::QGLSphereItem(QQuickItem *parent) :
    QGLItem(parent),
    m_radius(1.0),
    m_color(QColor(Qt::yellow))
{
    connect(this, SIGNAL(radiusChanged(float)),
            this, SIGNAL(propertyChanged()));
    connect(this, SIGNAL(colorChanged(QColor)),
            this, SIGNAL(propertyChanged()));
}

void QGLSphereItem::paint(QGLView *glView)
{
    glView->translate(position());
    glView->rotate(rotation());
    glView->scale(scale());

    glView->beginUnion(modelId());
    glView->color(m_color);
    glView->sphere(m_radius);
    glView->endUnion();
}
