#ifndef QGLCANVAS_H
#define QGLCANVAS_H

#include "qglitem.h"

class QGLCanvas : public QGLItem
{
    Q_OBJECT
    Q_PROPERTY(QGLView *context READ contex NOTIFY contextChanged)

public:
    explicit QGLCanvas(QQuickItem *parent = 0);

    virtual void paint(QGLView *glView);

    QGLView * contex() const
    {
        return m_context;
    }

signals:
    void contextChanged(QGLView * arg);
    void paint();

public slots:

private:
    QGLView * m_context;
};

#endif // QGLCANVAS_H
