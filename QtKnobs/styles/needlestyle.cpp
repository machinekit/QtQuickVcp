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

#include "needlestyle.h"
#define M_180_D_PI 180/M_PI

NeedleStyle::NeedleStyle(QQuickItem *parent) :
    QQuickPaintedItem(parent),
    m_value(0),
    m_minValue(0),
    m_maxValue(100),
    m_readOnly(false),
    m_color(QColor(135,206,235)),
    m_style(Style::Point),
    m_factor(3.6),
    m_scale(m_factor),
    m_angle(-90),
    m_offset(90.0)
{
    setAntialiasing(true);
#if defined Q_OS_LINUX || defined Q_OS_MAC
    setRenderTarget(QQuickPaintedItem::FramebufferObject);
#endif
}

void NeedleStyle::paint(QPainter *painter)
{
    qreal w = width()-1.0;
    qreal h = height();
    qreal d = 0.0;
    painter->setPen(Qt::NoPen);
    painter->setBrush(m_color);
    painter->save();
    painter->translate(w/2,h/2);
    painter->scale(1.0,1.0);
    painter->rotate(m_angle);

    switch(m_style)
    {
        case NeedleStyle::Point: {
            d = 3.0;
            static const QPoint points[3] = {
                QPoint(0, 14),
                QPoint(0, -14),
                QPoint(w/2 - 5, 0)
            };
            painter->drawConvexPolygon(points, 3);
            break;
        }

        case NeedleStyle::Round: {
            d = 3.0;
            painter->drawRoundedRect(0,-10,w/2-5,20,10,10);
            break;
        }

        case NeedleStyle::Groove: {
            d = 2.4;
            static qreal wd = w/12.0;
            static qreal p = (w/2.0) - wd/2.0;
            QRectF r(p-wd/2.0,-wd/2.0,wd,wd);
            QRadialGradient g(r.center(),wd);
            g.setColorAt(0,QColor(0,191,255));
            g.setColorAt(1,Qt::black);
            painter->setPen(Qt::NoPen);
            painter->setBrush(g);
            painter->drawEllipse(r);
            break;
        }
    }

    painter->restore();
    painter->setBrush(Qt::white);
    painter->drawEllipse(boundingRect().center(),w/d,w/d);
}

double NeedleStyle::endValueFromPoint(qreal x, qreal y)
{
    qreal theta = qAtan2(x,-y);
    qreal angle = fmod((theta * M_180_D_PI) + 360,360);
    double v = qFloor(angle) / m_scale;
    return v;
}

void NeedleStyle::classBegin()
{}

void NeedleStyle::componentComplete()
{
    m_anim =  children().first()->children().first();
}

bool NeedleStyle::animationRunning()
{
    return m_anim->property("running").toBool();
}

void NeedleStyle::setValue(double arg)
{
    if (m_value == arg)
        return;

    m_value = arg;

    if(m_value>m_maxValue)
        m_value = m_maxValue;

    m_angle = (m_value * m_factor) - m_offset;
    emit valueChanged(m_value);
    update();
}

void NeedleStyle::setMinValue(double arg)
{
    if (m_minValue == arg)
        return;

    m_minValue = arg;
    emit minValueChanged(arg);
}

void NeedleStyle::setMaxValue(double arg)
{
    if (m_maxValue == arg)
        return;

    m_maxValue = arg;
    m_factor = 360.0/m_maxValue;
    m_scale = m_factor;
    emit maxValueChanged(arg);
}
