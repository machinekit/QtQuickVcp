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

    paint();
}

void QGLCanvas::selectDrawable(void *pointer)
{
    emit drawableSelected(pointer);
}
