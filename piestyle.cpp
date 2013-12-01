/****************************************************************************
** Knobs is a free Qt and QML based widget.
** Copyright (C) 2013 Ashish Dabhade
** Email: ashishd157@gmail.com
**
** This program is free software: you can redistribute it and/or modify
** it under the terms of the GNU General Public License as published by
** the Free Software Foundation, either version 3 of the License, or
** (at your option) any later version.
**
** This program is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
** GNU General Public License for more details.
**
** You should have received a copy of the GNU General Public License
** along with this program (in doc/ directory).
** If not, see <http://www.gnu.org/licenses/gpl-3.0.html>.
****************************************************************************/

#include "piestyle.h"
#define M_180_D_PI 180/M_PI

PieStyle::PieStyle(QQuickItem *parent) :
    QQuickPaintedItem(parent)
{
    m_endValue = 0;
    m_multicolor = false;
    m_scale = 16 / 57.6;
    m_startAngle = (m_endValue * 57.6) + (1440);
    setAntialiasing(true);
}

void PieStyle::paint(QPainter *painter)
{
    painter->setPen(Qt::NoPen);
    painter->setBrush(m_color);

    if(!m_multicolor)
        painter->drawPie(boundingRect().adjusted(1, 1, -1, -1), m_startAngle, -m_spanAngle);
    else
    {
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
                painter->setBrush(m_chunkColors[i]);
                break;
            }

            painter->drawPie(boundingRect().adjusted(1, 1, -1, -1), m_startAngle, -m_spanAngle);
        }
    }
}

QColor PieStyle::color() const
{
    return m_color;
}

void PieStyle::setColor(QColor arg)
{
    m_color = arg;
}

int PieStyle::endValue() const
{
    return m_endValue;
}

int PieStyle::endValueFromPoint(qreal x, qreal y)
{
    qreal theta = qAtan2(x,-y);
    qreal angle = fmod((theta * M_180_D_PI) + 360,360);
    int v = qFloor(angle) * m_scale;
    return v;
}

void PieStyle::setEndValue(int arg)
{
    if (m_endValue != arg) {
        m_endValue = arg;
        m_spanAngle = m_endValue * 57.6;
        emit endValueChanged(arg);
        update();
    }
}

bool PieStyle::multicolor() const
{
    return m_multicolor;
}

void PieStyle::setmulticolor(bool arg)
{
    if (m_multicolor != arg) {
        m_multicolor = arg;

        if(m_multicolor)
            m_chunkColors << QColor(197,81,134)
                          << QColor(232,156,132)
                          << QColor(254,197,107)
                          << QColor(81,197,212);

        emit multicolorChanged(arg);

    }
}
