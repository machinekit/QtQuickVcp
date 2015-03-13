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

#include "arcstyle.h"
#define M_180_D_PI 180/M_PI

ArcStyle::ArcStyle(QQuickItem *parent) :
    QQuickPaintedItem(parent),
    m_value(0),
    m_maxValue(100),
    m_percent(0),
    m_readOnly(false),
    m_color(QColor(135,206,235)),
    m_mode(ArcStyle::Normal),
    m_factor(57.6),
    m_scale(16 / m_factor),
    m_startAngle(-1680),
    m_spanAngle(480),
    m_offset((1440) + ( m_spanAngle / 2 ))
{
    setAntialiasing(true);
#if defined Q_OS_LINUX || defined Q_OS_MAC
    setRenderTarget(QQuickPaintedItem::FramebufferObject);
#endif
}

void ArcStyle::paint(QPainter *painter)
{
    static const qreal m = 1.5;
    painter->setPen(Qt::NoPen);
    painter->setBrush(m_color);
    painter->drawPie(boundingRect().marginsRemoved(QMarginsF(m,m,m,m)), -m_startAngle, -m_spanAngle);
    qreal w = (width()/3.0)-1.0;
    painter->setBrush(Qt::white);
    painter->drawEllipse(boundingRect().center(),w,w);
}

int ArcStyle::endValueFromPoint(qreal x, qreal y)
{
    qreal theta = qAtan2(x,-y);
    qreal angle = fmod((theta * M_180_D_PI) + 360,360);
    int v = qFloor(angle) * m_scale;
    return m_mode==ArcStyle::Percent ? v*100/m_maxValue : v;
}

void ArcStyle::classBegin()
{}

void ArcStyle::componentComplete()
{
    m_anim =  children().first()->children().first();
}

bool ArcStyle::animationRunning()
{
    return m_anim->property("running").toBool();
}

void ArcStyle::setValue(int arg)
{
    if (m_value == arg)
        return;

    m_value = arg;

    if(m_value>m_maxValue)
        m_value = m_maxValue;

    m_startAngle = (m_value * m_factor) - m_offset;
    m_percent = m_value*100/m_maxValue;
    if(m_mode == ArcStyle::Normal) {
        emit percentChanged(m_percent);
        emit valueChanged(m_value);
    } else {
        emit valueChanged(m_value);
        emit percentChanged(m_percent);
    }
    update();
}

void ArcStyle::setMaxValue(int arg)
{
    if (m_maxValue == arg)
        return;

    m_maxValue = arg;
    m_factor = (360.0/m_maxValue) * 16;
    m_scale = 16 / m_factor;
    emit maxValueChanged(arg);
}

void ArcStyle::setPercent(int arg)
{
    if (m_percent == arg)
        return;

    m_percent = arg;
    setValue(m_percent*m_maxValue/100);
}
