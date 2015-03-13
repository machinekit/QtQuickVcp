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

#include "piestyle.h"
#define M_180_D_PI 180/M_PI
#define M_PI_D_180 M_PI/180

PieStyle::PieStyle(QQuickItem *parent) :
    QQuickPaintedItem(parent),
    m_value(0),
    m_maxValue(100),
    m_percent(0),
    m_readOnly(false),
    m_color(QColor(135,206,235)),
    m_mode(PieStyle::Normal),
    m_style(PieStyle::Flat),
    m_multiColor(false),
    m_factor(57.6),
    m_scale(16 / m_factor),
    m_startAngle((m_value * m_factor) + (1440)),
    m_spanAngle(0)
{
    setAntialiasing(true);
#if defined Q_OS_LINUX || defined Q_OS_MAC
    setRenderTarget(QQuickPaintedItem::FramebufferObject);
#endif
}

void PieStyle::paint(QPainter *painter)
{
    qreal w = (width()/3.0)-1.0;
    static const qreal m = 1.5;
    painter->setPen(Qt::NoPen);
    if(m_style==PieStyle::Flat) {
        if(!m_multiColor) {
            painter->setBrush(m_color);
            painter->drawPie(boundingRect().marginsRemoved(QMarginsF(m,m,m,m)), m_startAngle, -m_spanAngle);
        } else {
            qreal parts = m_spanAngle/1440 + 1;
            for(int i=0;i<parts;i++)
            {
                switch(i)
                {
                    case 0:
                        m_startAngle = 1440;
                        painter->setBrush(m_chunkColors[i]);
                        break;
                    case 1:
                    case 2:
                    case 3:
                        m_startAngle = m_startAngle - 1440;
                        m_spanAngle = m_spanAngle - 1440;
                        painter->setBrush(m_chunkColors.at(i));
                        break;
                }
                painter->drawPie(boundingRect().marginsRemoved(QMarginsF(m,m,m,m)), m_startAngle, -m_spanAngle);
            }
        }
    } else {
        if(m_value==0)
            return;
        qreal w = width();
        qreal h = height();
        qreal a = ((m_spanAngle/16.0)-90.0)*M_PI_D_180;
        qreal d = qCeil((w-w/1.2)/2.0)+1.0;
        qreal x = w/2.0+(w/2.0-d)*qCos(a);
        qreal y = h/2.0+(h/2.0-d)*qSin(a);
        painter->setBrush(m_color);
        painter->drawPie(boundingRect().marginsRemoved(QMarginsF(m,m,m,m)), m_startAngle, -m_spanAngle);
        painter->drawEllipse(QPointF(w/2.0,d),d-2.0,d-2.0);
        painter->drawEllipse(QPointF(x,y),d-2.0,d-2.0);
    }
    painter->setBrush(Qt::white);
    painter->drawEllipse(boundingRect().center(),w,w);
}

int PieStyle::endValueFromPoint(qreal x, qreal y)
{
    qreal theta = qAtan2(x,-y);
    qreal angle = fmod((theta * M_180_D_PI) + 360,360);
    int v = qFloor(angle) * m_scale;
    return m_mode==PieStyle::Percent ? v*100/m_maxValue : v;
}

void PieStyle::classBegin()
{}

void PieStyle::componentComplete()
{
    m_anim =  children().first()->children().first();
}

bool PieStyle::animationRunning()
{
    return m_anim->property("running").toBool();
}

void PieStyle::setValue(int arg)
{
    if (m_value == arg)
        return;

    m_value = arg;

    if(m_value>m_maxValue)
        m_value = m_maxValue;

    m_spanAngle = m_value * m_factor;
    m_percent = m_value*100/m_maxValue;
    if(m_mode == PieStyle::Normal) {
        emit percentChanged(m_percent);
        emit valueChanged(m_value);
    } else {
        emit valueChanged(m_value);
        emit percentChanged(m_percent);
    }
    update();
}

void PieStyle::setMaxValue(int arg)
{
    if (m_maxValue == arg)
        return;

    m_maxValue = arg;
    m_factor = (360.0/m_maxValue) * 16;
    m_scale = 16 / m_factor;
    emit maxValueChanged(arg);
}

void PieStyle::setPercent(int arg)
{
    if (m_percent == arg)
        return;

    m_percent = arg;
    setValue(m_percent*m_maxValue/100);
}

void PieStyle::setMultiColor(bool arg)
{
    if (m_multiColor != arg) {
        m_multiColor = arg;

        if(m_multiColor)
            m_chunkColors << QColor(197,81,134)
                          << QColor(232,156,132)
                          << QColor(254,197,107)
                          << QColor(81,197,212);

        emit multiColorChanged(arg);
    }
}

