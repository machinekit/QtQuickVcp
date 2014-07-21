#include "qglcubeitem.h"

QGLCubeItem::QGLCubeItem(QObject *parent) :
    QGLItem(parent),
    m_size(QVector3D(1,1,1)),
    m_color(QColor(Qt::yellow))
{
}

QGLCubeItem::QGLCubeItem(QVector3D size, QObject *parent) :
    QGLItem(parent),
    m_color(QColor(Qt::yellow))
{
    m_size = size;
}

QGLCubeItem::QGLCubeItem(QVector3D size, QColor color, QObject *parent) :
    QGLItem(parent)
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
    glView->cube(m_size);
    glView->endUnion();
}
