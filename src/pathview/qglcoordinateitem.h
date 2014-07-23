#ifndef QGLCOORDINATEITEM_H
#define QGLCOORDINATEITEM_H

#include "qglitem.h"

class QGLCoordinateItem : public QGLItem
{
    Q_OBJECT

public:
    QGLCoordinateItem(QQuickItem *parent = 0);

    virtual void paint(QGLView *glView);
};

#endif // QGLCOORDINATEITEM_H
