#include "qglgriditem.h"

QGLGridItem::QGLGridItem(QObject *parent) :
    QGLItem(parent)
{
}

void QGLGridItem::paint(QGLView *glView)
{
    float width = 5.0;
    float length = 5.0;
    float lineWidth = 0.02;
    float lineHeight = 0.01;
    float intervalX = 0.5;
    float intervalY = 0.5;

    glView->translate(position());
    glView->rotate(rotation());
    glView->scale(scale());

    glView->color(Qt::white);

    glView->beginUnion(modelId());

    float posX = 0;
    while (posX <= width)
    {
        glView->translate(posX - lineWidth/2.0, 0, 0);
        glView->cube(lineWidth, length, lineHeight);
        posX += intervalX;
    }

    float posY = 0;
    while (posY <= width)
    {
        glView->translate(0, posY + lineWidth/2.0, 0);
        glView->rotate(-90, 0, 0, 1);
        glView->cube(lineWidth, length, lineHeight);
        posY += intervalY;
    }

    glView->endUnion();
}
