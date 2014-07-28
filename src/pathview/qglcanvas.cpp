#include "qglcanvas.h"

QGLCanvas::QGLCanvas(QQuickItem *parent) :
    QGLItem(parent),
    m_context(NULL)
{
}

void QGLCanvas::paint(QGLView *glView)
{
    if (m_context != glView) {
        m_context = glView;
        emit contextChanged(glView);
    }

    glView->translate(position());
    glView->rotate(rotation());
    glView->scale(scale());

    glView->beginUnion();
    paint();
    glView->endUnion();
}

void QGLCanvas::selectDrawable(void *pointer)
{
    emit drawableSelected(pointer);
}
