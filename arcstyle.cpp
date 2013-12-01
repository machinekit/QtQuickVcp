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

#include "arcstyle.h"
#define M_180_D_PI 180/M_PI

ArcStyle::ArcStyle(QQuickItem *parent) :
    QQuickPaintedItem(parent)
{
    m_endValue = 0;
    m_scale = 16 / 57.6;

    m_spanAngle = 480;
    m_offset = (1440) + ( m_spanAngle / 2 );
//    setRenderTarget(QQuickPaintedItem::FramebufferObject);
    setAntialiasing(true);
}

void ArcStyle::paint(QPainter *painter)
{
    painter->setPen(Qt::NoPen);
    painter->setBrush(m_color);
    painter->drawPie(boundingRect().adjusted(1, 1, -1, -1), -m_startAngle, -m_spanAngle);
}

QColor ArcStyle::color() const
{
    return m_color;
}

void ArcStyle::setColor(QColor arg)
{
    m_color = arg;
}

int ArcStyle::endValue() const
{
    return m_endValue;
}

int ArcStyle::endValueFromPoint(qreal x, qreal y)
{
    qreal theta = qAtan2(x,-y);
    qreal angle = fmod((theta * M_180_D_PI) + 360,360);
    int v = qFloor(angle) * m_scale;
    return v;
}

void ArcStyle::setEndValue(int arg)
{
    if (m_endValue != arg) {
        m_endValue = arg;
        m_startAngle = (m_endValue * 57.6) - m_offset;
        emit endValueChanged(arg);
        update();
    }
}


