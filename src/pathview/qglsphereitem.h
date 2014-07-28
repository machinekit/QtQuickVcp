#ifndef QGLSPHEREITEM_H
#define QGLSPHEREITEM_H

#include "qglitem.h"

class QGLSphereItem : public QGLItem
{
    Q_OBJECT
    Q_PROPERTY(float radius READ radius WRITE setRadius NOTIFY radiusChanged)
    Q_PROPERTY(QColor color READ color WRITE setColor NOTIFY colorChanged)
    Q_PROPERTY(bool selected READ isSelected NOTIFY selectedChanged)

public:
    explicit QGLSphereItem(QQuickItem *parent = 0);

    virtual void paint(QGLView *glView);

    float radius() const
    {
        return m_radius;
    }

    QColor color() const
    {
        return m_color;
    }

    bool isSelected() const
    {
        return m_selected;
    }

signals:
    void radiusChanged(float arg);
    void colorChanged(QColor arg);
    void selectedChanged(bool arg);

public slots:
    virtual void selectDrawable(void *pointer);

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
    void *m_spherePointer;
    float m_radius;
    QColor m_color;
    bool m_selected;
};

#endif // QGLSPHEREITEM_H
