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

#ifndef PIESTYLE_H
#define PIESTYLE_H

#include <QQuickPaintedItem>
#include <QPainter>
#include <qmath.h>
#include <QtMath>

class PieStyle : public QQuickPaintedItem
{
    Q_OBJECT
    Q_ENUMS(CapStyle)
    Q_PROPERTY(double value MEMBER m_value WRITE setValue NOTIFY valueChanged)
    Q_PROPERTY(double minValue MEMBER m_minValue WRITE setMinValue NOTIFY minValueChanged)
    Q_PROPERTY(double maxValue MEMBER m_maxValue WRITE setMaxValue NOTIFY maxValueChanged)
    Q_PROPERTY(double stepSize MEMBER m_stepSize WRITE setStepSize NOTIFY stepSizeChanged)
    Q_PROPERTY(bool multicolor MEMBER m_multiColor WRITE setMultiColor NOTIFY multiColorChanged)

    Q_PROPERTY(QColor color MEMBER m_color WRITE setColor NOTIFY colorChanged)
    Q_PROPERTY(CapStyle style MEMBER m_style NOTIFY styleChanged)
    Q_PROPERTY(bool readOnly MEMBER m_readOnly NOTIFY readOnlyChanged)

public:
    explicit PieStyle(QQuickItem *parent = 0);

    enum CapStyle {
        Flat = 8,
        Curve
    };

    void paint(QPainter *painter);
    Q_INVOKABLE double endValueFromPoint(qreal x = 0, qreal y = 0);
    void classBegin();
    void componentComplete();
    bool animationRunning();

signals:
    void valueChanged(double arg);
    void multiColorChanged(bool arg);
    void minValueChanged(double arg);
    void maxValueChanged(double arg);
    void stepSizeChanged(double arg);
    void readOnlyChanged(bool arg);
    void colorChanged(QColor arg);
    void styleChanged(CapStyle arg);

protected slots:
    void setMinValue(double arg);
    void setMaxValue(double arg);
    void setStepSize(double arg);
    void setMultiColor(bool arg);
    void setValue(double arg);
    void setColor(QColor arg);

private:
    double m_value;
    double m_minValue;
    double m_maxValue;
    double m_stepSize;
    bool m_readOnly;
    QColor m_color;
    CapStyle m_style;
    bool m_multiColor;
    qreal m_factor;
    qreal m_scale;
    int m_startAngle;
    int m_spanAngle;
    QList<QColor> m_chunkColors;
    QObject *m_anim;
};

#endif // PIESTYLE_H
