#ifndef QGLGRIDITEM_H
#define QGLGRIDITEM_H

#include "qglitem.h"

class QGLGridItem : public QGLItem
{
    Q_OBJECT
public:
    explicit QGLGridItem(QQuickItem *parent = 0);

    virtual void paint(QGLView *glView);

signals:

public slots:

};

#endif // QGLGRIDITEM_H
