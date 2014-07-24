#ifndef QGLBOUNDINGBOXITEM_H
#define QGLBOUNDINGBOXITEM_H

#include "qglitem.h"

class QGLBoundingBoxItem : public QGLItem
{
    Q_OBJECT
    Q_PROPERTY(QVector3D size READ size WRITE setSize NOTIFY sizeChanged)
    Q_PROPERTY(QColor color READ color WRITE setColor NOTIFY colorChanged)

public:
    explicit QGLBoundingBoxItem(QQuickItem *parent = 0);

    virtual void paint(QGLView *glView);

    QVector3D size() const
    {
        return m_size;
    }

    QColor color() const
    {
        return m_color;
    }

signals:
    void sizeChanged(QVector3D arg);
    void colorChanged(QColor arg);

public slots:
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

private:
    QVector3D m_size;
    QColor m_color;
};

#endif // QGLBOUNDINGBOXITEM_H
