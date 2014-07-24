#ifndef QGLPATHITEM_H
#define QGLPATHITEM_H

#include "qglitem.h"

class QGLPathItem : public QGLItem
{
    Q_OBJECT
public:
    explicit QGLPathItem(QQuickItem *parent = 0);

    virtual void paint(QGLView *glView);

signals:

public slots:

};

#endif // QGLPATHITEM_H
