#include "qglsphereitem.h"

QGLSphereItem::QGLSphereItem(QQuickItem *parent) :
    QGLItem(parent),
    m_spherePointer(NULL),
    m_radius(1.0),
    m_color(QColor(Qt::yellow)),
    m_selected(false)
{
    connect(this, SIGNAL(radiusChanged(float)),
            this, SIGNAL(needsUpdate()));
    connect(this, SIGNAL(colorChanged(QColor)),
            this, SIGNAL(needsUpdate()));
}

void QGLSphereItem::paint(QGLView *glView)
{
    glView->prepare(this);

    glView->reset();
    glView->beginUnion();
    glView->color(m_color);
    m_spherePointer = glView->sphere(m_radius);
    glView->endUnion();
}

void QGLSphereItem::selectDrawable(void *pointer)
{
    bool selected;

    if (m_spherePointer == NULL)
    {
        return;
    }

    selected = (pointer == m_spherePointer);

    if (selected != m_selected)
    {
        m_selected = selected;
        emit selectedChanged(m_selected);
    }
}
