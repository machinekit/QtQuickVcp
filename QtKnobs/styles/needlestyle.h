/*************************************************************************************
** The MIT License (MIT)
**
** Knobs is a free Qt and QML based widget.
** Copyright (c) 2015 Ashish Dabhade
** Email: ashishd157@gmail.com
**
** Permission is hereby granted, free of charge, to any person obtaining a copy
** of this software and associated documentation files (the "Software"), to deal
** in the Software without restriction, including without limitation the rights
** to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
** copies of the Software, and to permit persons to whom the Software is
** furnished to do so, subject to the following conditions:
**
** The above copyright notice and this permission notice shall be included in all
** copies or substantial portions of the Software.
**
** THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
** IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
** FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
** AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
** LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
** OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
** SOFTWARE.
************************************************************************************/

#ifndef NEEDLESTYLE_H
#define NEEDLESTYLE_H

#include <QQuickPaintedItem>
#include <qmath.h>
#include <QDebug>
#include <QPainter>

class NeedleStyle : public QQuickPaintedItem
{
    Q_OBJECT
    Q_ENUMS(Style)
    Q_PROPERTY(double value MEMBER m_value WRITE setValue NOTIFY valueChanged)
    Q_PROPERTY(double minValue MEMBER m_minValue WRITE setMinValue NOTIFY minValueChanged)
    Q_PROPERTY(double maxValue MEMBER m_maxValue WRITE setMaxValue NOTIFY maxValueChanged)
    Q_PROPERTY(double stepSize MEMBER m_stepSize WRITE setStepSize NOTIFY stepSizeChanged)
    Q_PROPERTY(Style style MEMBER m_style NOTIFY styleChanged)
    Q_PROPERTY(QColor color MEMBER m_color WRITE setColor NOTIFY colorChanged)
    Q_PROPERTY(bool readOnly MEMBER m_readOnly NOTIFY readOnlyChanged)

public:
    explicit NeedleStyle(QQuickItem *parent = 0);

    enum Style {
        Point = 5,
        Round,
        Groove
    };

    void paint(QPainter *painter);
    Q_INVOKABLE double endValueFromPoint(qreal x = 0, qreal y = 0);
    void classBegin();
    void componentComplete();
    bool animationRunning();

signals:
    void styleChanged(Style arg);
    void valueChanged(double arg);
    void minValueChanged(double arg);
    void maxValueChanged(double arg);
    void stepSizeChanged(double arg);
    void readOnlyChanged(bool arg);
    void colorChanged(QColor arg);

protected slots:
    void setValue(double arg);
    void setMinValue(double arg);
    void setMaxValue(double arg);
    void setStepSize(double arg);
    void setColor(QColor arg);

private:
    double m_value;
    double m_minValue;
    double m_maxValue;
    double m_stepSize;
    bool m_readOnly;
    QColor m_color;
    Style m_style;
    qreal m_factor;
    qreal m_scale;
    qreal m_angle;
    int m_offset;
    QObject *m_anim;
};

#endif // NEEDLESTYLE_H
