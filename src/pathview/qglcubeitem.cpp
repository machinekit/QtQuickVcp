#include "qglcubeitem.h"

QGLCubeItem::QGLCubeItem(QQuickItem *parent) :
    QGLItem(parent),
    m_size(QVector3D(1,1,1)),
    m_color(QColor(Qt::yellow)),
    m_centered(false)
{
    connect(this, SIGNAL(sizeChanged(QVector3D)),
            this, SIGNAL(propertyChanged()));
    connect(this, SIGNAL(colorChanged(QColor)),
            this, SIGNAL(propertyChanged()));
    connect(this, SIGNAL(centeredChanged(bool)),
            this, SIGNAL(propertyChanged()));
}

void QGLCubeItem::paint(QGLView *glView)
{
    glView->translate(position());
    glView->rotate(rotation());
    glView->scale(scale());

    glView->beginUnion(modelId());
    glView->color(m_color);
    glView->cube(m_size, m_centered);
    glView->endUnion();
}
