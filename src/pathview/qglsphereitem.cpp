#include "qglsphereitem.h"

QGLSphereItem::QGLSphereItem(QQuickItem *parent) :
    QGLItem(parent),
    m_radius(1.0),
    m_color(QColor(Qt::yellow))
{
    connect(this, SIGNAL(radiusChanged(float)),
            this, SIGNAL(needsUpdate()));
    connect(this, SIGNAL(colorChanged(QColor)),
            this, SIGNAL(needsUpdate()));
}

void QGLSphereItem::paint(QGLView *glView)
{
    glView->translate(position());
    glView->rotate(rotation());
    glView->scale(scale());

    glView->reset();
    glView->beginUnion();
    glView->color(m_color);
    glView->sphere(m_radius);
    glView->endUnion();
}
