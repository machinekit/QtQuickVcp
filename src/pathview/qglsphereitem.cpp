#include "qglsphereitem.h"

QGLSphereItem::QGLSphereItem(QQuickItem *parent) :
    QGLItem(parent),
    m_radius(1.0),
    m_color(QColor(Qt::yellow))
{
}

QGLSphereItem::QGLSphereItem(float radius, QQuickItem *parent) :
    QGLItem(parent),
    m_color(QColor(Qt::yellow))
{
    m_radius = radius;
}

QGLSphereItem::QGLSphereItem(float radius, QColor color, QQuickItem *parent) :
    QGLItem(parent)
{
    m_radius = radius;
    m_color = color;
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
