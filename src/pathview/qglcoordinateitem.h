#ifndef QGLCOORDINATEITEM_H
#define QGLCOORDINATEITEM_H

#include "qglitem.h"

class QGLCoordinateItem : public QGLItem
{
    Q_OBJECT
    Q_PROPERTY(float axesLength READ axesLength WRITE setAxesLength NOTIFY axesLengthChanged)
    Q_PROPERTY(float textSize READ textSize WRITE setTextSize NOTIFY textSizeChanged)

public:
    QGLCoordinateItem(QQuickItem *parent = 0);

    virtual void paint(QGLView *glView);

    float axesLength() const
    {
        return m_axesLength;
    }

    float textSize() const
    {
        return m_textSize;
    }

public slots:
    void setAxesLength(float arg)
    {
        if (m_axesLength != arg) {
            m_axesLength = arg;
            emit axesLengthChanged(arg);
        }
    }

    void setTextSize(float arg)
    {
        if (m_textSize != arg) {
            m_textSize = arg;
            emit textSizeChanged(arg);
        }
    }

signals:
    void axesLengthChanged(float arg);
    void textSizeChanged(float arg);

private:
    float m_axesLength;
    float m_textSize;
};

#endif // QGLCOORDINATEITEM_H
