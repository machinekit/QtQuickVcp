#ifndef QGLSPHEREITEM_H
#define QGLSPHEREITEM_H

#include "qglitem.h"

class QGLSphereItem : public QGLItem
{
    Q_OBJECT
    Q_PROPERTY(float radius READ radius WRITE setRadius NOTIFY radiusChanged)
    Q_PROPERTY(QColor color READ color WRITE setColor NOTIFY colorChanged)

public:
    explicit QGLSphereItem(QQuickItem *parent = 0);
    explicit QGLSphereItem(float radius, QQuickItem *parent = 0);
    explicit QGLSphereItem(float radius, QColor color, QQuickItem *parent = 0);

    virtual void paint(QGLView *glView);

    float radius() const
    {
        return m_radius;
    }

    QColor color() const
    {
        return m_color;
    }

signals:
    void radiusChanged(float arg);

    void colorChanged(QColor arg);

public slots:
    void setRadius(float arg)
    {
        if (m_radius != arg) {
            m_radius = arg;
            emit radiusChanged(arg);
        }
    }

    void setColor(QColor arg)
    {
        if (m_color != arg) {
            m_color = arg;
            emit colorChanged(arg);
        }
    }

private:
    float m_radius;

    QColor m_color;
};

#endif // QGLSPHEREITEM_H
