#include "qglgriditem.h"

QGLGridItem::QGLGridItem(QQuickItem *parent) :
    QGLItem(parent)
{
}

void QGLGridItem::paint(QGLView *glView)
{
    float width = 5.0;
    float length = 5.0;
    float intervalX = 0.5;
    float intervalY = 0.5;

    glView->translate(position());
    glView->rotate(rotation());
    glView->scale(scale());

    glView->color(Qt::gray);

    glView->beginUnion(modelId());

    float posX = 0;
    while (posX <= width)
    {

        glView->translate(posX, 0, 0);
        glView->line(0, length, 0);
        posX += intervalX;
    }

    float posY = 0;
    while (posY <= width)
    {
        glView->translate(0, posY, 0);
        glView->line(length, 0, 0);
        posY += intervalY;
    }

    glView->endUnion();
}
