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
    Q_PROPERTY(bool selected READ isSelected NOTIFY selectedChanged)

public:
    explicit QGLCylinderItem(QQuickItem *parent = 0);

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

    bool isSelected() const
    {
        return m_selected;
    }

signals:
    void radiusChanged(float arg);
    void heightChanged(float arg);
    void colorChanged(QColor arg);
    void coneChanged(bool arg);
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
    void *m_cylinderPointer;
    float m_radius;
    float m_height;
    QColor m_color;
    bool m_cone;
    bool m_selected;
};

#endif // QGLCYLINDERITEM_H
