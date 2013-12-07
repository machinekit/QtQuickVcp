/****************************************************************************
** Knobs is a free Qt and QML based widget.
** Copyright (C) 2013 Ashish Dabhade
** Email: ashishd157@gmail.com
**
** This program is free software: you can redistribute it and/or modify
** it under the terms of the GNU Lesser General Public License as published by
** the Free Software Foundation, either version 2 of the License, or
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

#include "needlestyle.h"
#define M_180_D_PI 180/M_PI

NeedleStyle::NeedleStyle(QQuickItem *parent) :
    QQuickPaintedItem(parent)
{
    m_scale = 16 / 57.6;
    setAntialiasing(true);
}

void NeedleStyle::paint(QPainter *painter)
{
    painter->setPen(Qt::NoPen);
    painter->setBrush(m_color);
    painter->translate(width() / 2, height() / 2);
    painter->scale(1.0,1.0);

    switch(m_style)
    {
    case NeedleStyle::Point:

        static const QPoint points[3] = {
            QPoint(0, 14),
            QPoint(0, -14),
            QPoint(width()/2 - 5, 0)
        };
        painter->drawConvexPolygon(points, 3);
        break;

    case NeedleStyle::Round:
        painter->drawRoundedRect(0,-10,width()/2-5,20,10,10);
        break;

    case NeedleStyle::Groove:
        static int w = width()/12;
        static int p = (width()/2) - w/2 - 1;

        QRect r(p-w/2,-w/2,w,w);

        QRadialGradient g(r.center(),w);
        g.setColorAt(0,QColor(0,191,255));
        g.setColorAt(1,Qt::black);

        painter->setPen(Qt::NoPen);
        painter->setBrush(g);

        painter->drawEllipse(r);

        break;
    }
}

NeedleStyle::Style NeedleStyle::style() const
{
    return m_style;
}

void NeedleStyle::setstyle(Style arg)
{
    if (m_style != arg) {
        m_style = arg;
        emit styleChanged(arg);
    }
}

int NeedleStyle::endValueFromPoint(qreal x, qreal y)
{
    qreal theta = qAtan2(x,-y);
    qreal angle = fmod((theta * M_180_D_PI) + 360,360);
    int v = qFloor(angle) * m_scale;
    return v;
}

QColor NeedleStyle::color() const
{
    return m_color;
}

void NeedleStyle::setColor(QColor arg)
{
    m_color = arg;
}
