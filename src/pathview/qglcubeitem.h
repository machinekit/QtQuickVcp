#ifndef QGLCUBEITEM_H
#define QGLCUBEITEM_H

#include "qglitem.h"

class QGLCubeItem : public QGLItem
{
    Q_OBJECT
    Q_PROPERTY(QVector3D size READ size WRITE setSize NOTIFY sizeChanged)
    Q_PROPERTY(QColor color READ color WRITE setColor NOTIFY colorChanged)
    Q_PROPERTY(bool centered READ isCentered WRITE setCentered NOTIFY centeredChanged)

public:
    explicit QGLCubeItem(QQuickItem *parent = 0);
    explicit QGLCubeItem(QVector3D size, QQuickItem *parent = 0);
    explicit QGLCubeItem(QVector3D size, QColor color, QQuickItem *parent = 0);

    virtual void paint(QGLView *glView);

    QVector3D size() const
    {
        return m_size;
    }

    QColor color() const
    {
        return m_color;
    }

    bool isCentered() const
    {
        return m_centered;
    }

signals:

    void sizeChanged(QVector3D arg);
    void colorChanged(QColor arg);

    void centeredChanged(bool arg);

public slots:

    void setSize(float w, float l, float h)
    {
        setSize(QVector3D(w,l,h));
    }

    void setSize(QVector3D arg)
    {
        if (m_size != arg) {
            m_size = arg;
            emit sizeChanged(arg);
        }
    }

    void setColor(QColor arg)
    {
        if (m_color != arg) {
            m_color = arg;
            emit colorChanged(arg);
        }
    }

    void setCentered(bool arg)
    {
        if (m_centered != arg) {
            m_centered = arg;
            emit centeredChanged(arg);
        }
    }

private:
    QVector3D m_size;
    QColor m_color;
    bool m_centered;
};

#endif // QGLCUBEITEM_H
