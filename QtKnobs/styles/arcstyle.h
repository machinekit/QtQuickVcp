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

#ifndef ARCSTYLE_H
#define ARCSTYLE_H

#include <QQuickPaintedItem>
#include <QPainter>
#include <QQmlParserStatus>
#include <qmath.h>

class ArcStyle : public QQuickPaintedItem
{
    Q_OBJECT
    Q_ENUMS(Mode)
    Q_PROPERTY(int value MEMBER m_value WRITE setValue NOTIFY valueChanged)
    Q_PROPERTY(int maxValue MEMBER m_maxValue WRITE setMaxValue NOTIFY maxValueChanged)
    Q_PROPERTY(int percent MEMBER m_percent WRITE setPercent NOTIFY percentChanged)
    Q_PROPERTY(QColor color MEMBER m_color NOTIFY colorChanged)
    Q_PROPERTY(bool readOnly MEMBER m_readOnly NOTIFY readOnlyChanged)
    Q_PROPERTY(Mode mode MEMBER m_mode NOTIFY modeChanged)

public:
    explicit ArcStyle(QQuickItem *parent = 0);

    enum Mode {
        Normal = 10,
        Percent
    };

    void paint(QPainter *painter);
    Q_INVOKABLE int endValueFromPoint(qreal x = 0, qreal y = 0);
    void classBegin();
    void componentComplete();
    bool animationRunning();

signals:
    void valueChanged(int arg);
    void maxValueChanged(int arg);
    void percentChanged(int arg);
    void readOnlyChanged(bool arg);
    void modeChanged(Mode arg);
    void colorChanged(QColor arg);

protected slots:
    void setValue(int arg);
    void setMaxValue(int arg);
    void setPercent(int arg);

private:
    int m_value;
    int m_maxValue;
    int m_percent;
    bool m_readOnly;
    QColor m_color;
    Mode m_mode;
    qreal m_factor;
    qreal m_scale;
    int m_startAngle;
    int m_spanAngle;
    int m_offset;
    QObject *m_anim;
};

#endif // ARCSTYLE_H
