#include "qglcubeitem.h"

QGLCubeItem::QGLCubeItem(QQuickItem *parent) :
    QGLItem(parent),
    m_size(QVector3D(1,1,1)),
    m_color(QColor(Qt::yellow)),
    m_centered(false)
{
}

QGLCubeItem::QGLCubeItem(QVector3D size, QQuickItem *parent) :
    QGLItem(parent),
    m_color(QColor(Qt::yellow)),
    m_centered(false)
{
    m_size = size;
}

QGLCubeItem::QGLCubeItem(QVector3D size, QColor color, QQuickItem *parent) :
    QGLItem(parent),
    m_centered(false)
{
    m_size = size;
    m_color = color;
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
