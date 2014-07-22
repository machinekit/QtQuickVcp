#ifndef QGLCYLINDERITEM_H
#define QGLCYLINDERITEM_H

#include "qglitem.h"

class QGLCylinderItem : public QGLItem
{
    Q_OBJECT
    Q_PROPERTY(float radius READ radius WRITE setRadius NOTIFY radiusChanged)
    Q_PROPERTY(float height READ height WRITE setHeight NOTIFY heightChanged)
    Q_PROPERTY(QColor color READ color WRITE setColor NOTIFY colorChanged)
    Q_PROPERTY(bool cone READ isCone WRITE setCone NOTIFY coneChanged)

public:
    explicit QGLCylinderItem(QObject *parent = 0);
    explicit QGLCylinderItem(float radius, float height, QObject *parent = 0);
    explicit QGLCylinderItem(float radius, float height, QColor color, QObject *parent = 0);

    virtual void paint(QGLView *glView);

    float radius() const
    {
        return m_radius;
    }

    float height() const
    {
        return m_height;
    }

    QColor color() const
    {
        return m_color;
    }

    bool isCone() const
    {
        return m_cone;
    }

signals:
    void radiusChanged(float arg);
    void heightChanged(float arg);
    void colorChanged(QColor arg);
    void coneChanged(bool arg);

public slots:
    void setRadius(float arg)
    {
        if (m_radius != arg) {
            m_radius = arg;
            emit radiusChanged(arg);
        }
    }

    void setHeight(float arg)
    {
        if (m_height != arg) {
            m_height = arg;
            emit heightChanged(arg);
        }
    }

    void setColor(QColor arg)
    {
        if (m_color != arg) {
            m_color = arg;
            emit colorChanged(arg);
        }
    }

    void setCone(bool arg)
    {
        if (m_cone != arg) {
            m_cone = arg;
            emit coneChanged(arg);
        }
    }

private:
    float m_radius;
    float m_height;
    QColor m_color;
    bool m_cone;
};

#endif // QGLCYLINDERITEM_H
