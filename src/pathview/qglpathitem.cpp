#include "qglpathitem.h"
#include <QtCore/qmath.h>

QGLPathItem::QGLPathItem(QQuickItem *parent) :
    QGLItem(parent)
{
}

void QGLPathItem::paint(QGLView *glView)
{
    int n = 5000;
    float f = 16;
    float a = 1000;
    float x;
    float y;
    float z;

    glView->translate(position());
    glView->rotate(rotation());
    glView->scale(scale());

    glView->color(QColor(Qt::white));
    glView->beginUnion();
    for (int i = 0; i < n; ++i)
    {
        x = qCos(2 * M_PI * (float)i / f);
        y = qSin(2 * M_PI * (float)i / f);
        z = (float)i/a;
        glView->lineTo(x, y, z);
        //glView->cylinder(1.0,1.0);
    }
    glView->endUnion();
}

void QGLPathItem::selectDrawable(void *pointer)
{

}
