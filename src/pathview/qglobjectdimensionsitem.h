#ifndef QGLOBJECTDIMENSIONSITEM_H
#define QGLOBJECTDIMENSIONSITEM_H

#include "qglitem.h"

class QGLObjectDimensionsItem : public QGLItem
{
    Q_OBJECT
    Q_PROPERTY(QVector3D objectSize READ objectSize WRITE setObjectSize NOTIFY objectSizeChanged)
    Q_PROPERTY(QVector3D objectPosition READ objectPosition WRITE setObjectPosition NOTIFY objectPositionChanged)
    Q_PROPERTY(QColor color READ color WRITE setColor NOTIFY colorChanged)
    Q_PROPERTY(float textSize READ textSize WRITE setTextSize NOTIFY textSizeChanged)
    Q_PROPERTY(QString prefix READ prefix WRITE setPrefix NOTIFY prefixChanged)
    Q_PROPERTY(QString suffix READ suffix WRITE setSuffix NOTIFY suffixChanged)
    Q_PROPERTY(int decimals READ decimals WRITE setDecimals NOTIFY decimalsChanged)

public:
    explicit QGLObjectDimensionsItem(QQuickItem *parent = 0);

    virtual void paint(QGLView *glView);

    QVector3D objectSize() const
    {
        return m_objectSize;
    }

    QVector3D objectPosition() const
    {
        return m_objectPosition;
    }

    QColor color() const
    {
        return m_color;
    }

    float textSize() const
    {
        return m_textSize;
    }

    QString prefix() const
    {
        return m_prefix;
    }

    QString suffix() const
    {
        return m_suffix;
    }

    int decimals() const
    {
        return m_decimals;
    }

signals:
    void objectSizeChanged(QVector3D arg);
    void objectPositionChanged(QVector3D arg);
    void colorChanged(QColor arg);
    void textSizeChanged(float arg);
    void prefixChanged(QString arg);
    void suffixChanged(QString arg);
    void decimalsChanged(int arg);

public slots:
    void setObjectSize(QVector3D arg)
    {
        if (m_objectSize != arg) {
            m_objectSize = arg;
            emit objectSizeChanged(arg);
        }
    }

    void setObjectPosition(QVector3D arg)
    {
        if (m_objectPosition != arg) {
            m_objectPosition = arg;
            emit objectPositionChanged(arg);
        }
    }

    void setColor(QColor arg)
    {
        if (m_color != arg) {
            m_color = arg;
            emit colorChanged(arg);
        }
    }

    void setTextSize(float arg)
    {
        if (m_textSize != arg) {
            m_textSize = arg;
            emit textSizeChanged(arg);
        }
    }

    void setPrefix(QString arg)
    {
        if (m_prefix != arg) {
            m_prefix = arg;
            emit prefixChanged(arg);
        }
    }

    void setSuffix(QString arg)
    {
        if (m_suffix != arg) {
            m_suffix = arg;
            emit suffixChanged(arg);
        }
    }

    void setDecimals(int arg)
    {
        if (m_decimals != arg) {
            m_decimals = arg;
            emit decimalsChanged(arg);
        }
    }

private:
    QVector3D m_objectSize;
    QVector3D m_objectPosition;
    QColor m_color;
    float m_textSize;
    QString m_prefix;
    QString m_suffix;
    int m_decimals;
};

#endif // QGLOBJECTDIMENSIONSITEM_H
