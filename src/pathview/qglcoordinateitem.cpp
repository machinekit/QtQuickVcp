#include "qglcoordinateitem.h"

QGLCoordinateItem::QGLCoordinateItem(QQuickItem *parent):
    QGLItem(parent)
{

}

void QGLCoordinateItem::paint(QGLView *glView)
{
    glView->translate(position());
    glView->rotate(rotation());
    glView->scale(scale());

    glView->beginUnion(modelId());

    glView->color(QColor(Qt::red));
    glView->line(10,0,0);
    glView->color(QColor(Qt::red));
    glView->translate(10,-0.5,0);
    glView->text("X");

    glView->color(QColor(Qt::green));
    glView->line(0,10,0);
    glView->color(QColor(Qt::green));
    glView->translate(0.5,10,0);
    glView->rotate(90,0,0,1);
    glView->text("Y");

    glView->color(QColor(Qt::blue));
    glView->line(0,0,10);
    glView->color(QColor(Qt::blue));
    glView->translate(0,0,10);
    glView->rotate(90,1,0,0);
    glView->text("Z");

    glView->endUnion();
}
