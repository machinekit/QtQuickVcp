#include "qglboundingboxitem.h"

QGLBoundingBoxItem::QGLBoundingBoxItem(QQuickItem *parent) :
    QGLItem(parent),
    m_size(QVector3D(10.0,10.0,10.0)),
    m_color(QColor(Qt::red))
{
    connect(this, SIGNAL(sizeChanged(QVector3D)),
            this, SIGNAL(propertyChanged()));
    connect(this, SIGNAL(colorChanged(QColor)),
            this, SIGNAL(propertyChanged()));
}

void QGLBoundingBoxItem::paint(QGLView *glView)
{
    glView->translate(position());
    glView->rotate(rotation());
    glView->scale(scale());

    glView->lineStipple(true, 1.0);
    glView->color(m_color);
    glView->beginUnion(modelId());
    glView->lineTo(m_size.x(), 0.0, 0.0);
    glView->lineTo(m_size.x(), m_size.y(), 0.0);
    glView->lineTo(0.0, m_size.y(), 0.0);
    glView->lineTo(0.0, 0.0, 0.0);
    glView->lineTo(0.0, 0.0, m_size.z());
    glView->lineTo(m_size.x(), 0.0, m_size.z());
    glView->lineTo(m_size.x(), m_size.y(), m_size.z());
    glView->lineTo(0.0, m_size.y(), m_size.z());
    glView->lineTo(0.0, 0.0, m_size.z());
    glView->resetTransformations();
    glView->lineFromTo(0.0, m_size.x(), 0.0, 0.0, m_size.x(), m_size.z());
    glView->lineFromTo(m_size.y(), m_size.x(), 0.0, m_size.y(), m_size.x(), m_size.z());
    glView->lineFromTo(m_size.y(), 0.0, 0.0, m_size.y(), 0.0, m_size.z());
    glView->endUnion();
}
