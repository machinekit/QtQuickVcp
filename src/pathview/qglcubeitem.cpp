#include "qglcubeitem.h"

QGLCubeItem::QGLCubeItem(QQuickItem *parent) :
    QGLItem(parent),
    m_size(QVector3D(1,1,1)),
    m_color(QColor(Qt::yellow)),
    m_centered(false)
{
    connect(this, SIGNAL(sizeChanged(QVector3D)),
            this, SIGNAL(needsUpdate()));
    connect(this, SIGNAL(colorChanged(QColor)),
            this, SIGNAL(needsUpdate()));
    connect(this, SIGNAL(centeredChanged(bool)),
            this, SIGNAL(needsUpdate()));
}

void QGLCubeItem::paint(QGLView *glView)
{
    glView->translate(position());
    glView->rotate(rotation());
    glView->scale(scale());

    glView->reset();
    glView->beginUnion();
    glView->color(m_color);
    glView->cube(m_size, m_centered);
    glView->endUnion();
}
