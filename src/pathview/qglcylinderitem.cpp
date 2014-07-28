#include "qglcylinderitem.h"

QGLCylinderItem::QGLCylinderItem(QQuickItem *parent) :
    QGLItem(parent),
    m_cylinderPointer(NULL),
    m_radius(1.0),
    m_height(1.0),
    m_color(QColor(Qt::yellow)),
    m_cone(false),
    m_selected(false)
{
    connect(this, SIGNAL(radiusChanged(float)),
            this, SIGNAL(needsUpdate()));
    connect(this, SIGNAL(heightChanged()),
            this, SIGNAL(needsUpdate()));
    connect(this, SIGNAL(colorChanged(QColor)),
            this, SIGNAL(needsUpdate()));
    connect(this, SIGNAL(coneChanged(bool)),
            this, SIGNAL(needsUpdate()));
}

void QGLCylinderItem::paint(QGLView *glView)
{
    glView->translate(position());
    glView->rotate(rotation());
    glView->scale(scale());

    glView->reset();
    glView->beginUnion();
    glView->color(m_color);
    if (!m_cone)
    {
        m_cylinderPointer = glView->cylinder(m_radius, m_height);
    }
    else
    {
        m_cylinderPointer = glView->cone(m_radius, m_height);
    }
    glView->endUnion();
}

void QGLCylinderItem::selectDrawable(void *pointer)
{
    bool selected;

    if (m_cylinderPointer == NULL)
    {
        return;
    }

    selected = (pointer == m_cylinderPointer);

    if (selected != m_selected)
    {
        m_selected = selected;
        emit selectedChanged(m_selected);
    }
}
