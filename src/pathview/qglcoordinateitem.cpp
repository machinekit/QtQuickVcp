#include "qglcoordinateitem.h"

QGLCoordinateItem::QGLCoordinateItem(QQuickItem *parent):
    QGLItem(parent),
    m_axesLength(5.0),
    m_textSize(1.0)
{
    connect(this, SIGNAL(axesLengthChanged(float)),
            this, SIGNAL(propertyChanged()));
    connect(this, SIGNAL(textSizeChanged(float)),
            this, SIGNAL(propertyChanged()));
}

void QGLCoordinateItem::paint(QGLView *glView)
{
    glView->translate(position());
    glView->rotate(rotation());
    glView->scale(scale());

    glView->beginUnion(modelId());

    glView->color(QColor(Qt::red));
    glView->line(m_axesLength,0,0);
    glView->color(QColor(Qt::red));
    glView->translate(m_axesLength, -m_textSize/2.0, 0);
    glView->scale(m_textSize, m_textSize, m_textSize);
    glView->text("X");

    glView->color(QColor(Qt::green));
    glView->line(0,m_axesLength,0);
    glView->color(QColor(Qt::green));
    glView->translate(m_textSize/2.0, m_axesLength, 0);
    glView->rotate(90,0,0,1);
    glView->scale(m_textSize, m_textSize, m_textSize);
    glView->text("Y");

    glView->color(QColor(Qt::blue));
    glView->line(0,0,m_axesLength);
    glView->color(QColor(Qt::blue));
    glView->translate(0,0,m_axesLength);
    glView->rotate(90,1,0,0);
    glView->scale(m_textSize, m_textSize, m_textSize);
    glView->text("Z", QGLView::AlignCenter);

    glView->endUnion();
}
