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

#include "meter.h"
#include <QDebug>
#include <QRadialGradient>
#include <QRect>

Meter::Meter(QQuickItem *parent) :
    QQuickPaintedItem(parent)
{
    setAntialiasing(true);
}

void Meter::paint(QPainter *painter)
{
    static int w = width()/30;
    static int p = (width()/2)-(width()/12);

    painter->translate(width() / 2, height() / 2);
    painter->scale(1.0,1.0);
    for (int i = 0; i < 20; ++i) {
        if(i%5==0)
        {
            painter->setPen(QPen(QColor(197,81,134), w));
            painter->drawLine(p, 0, width()/2, 0);
        }
        else
        {
            QRect r(p-w/2,-w/2,w,w);

            QRadialGradient g(r.center(),w);
            g.setColorAt(0,QColor(0,191,255));
            g.setColorAt(1,Qt::black);

            painter->setPen(Qt::NoPen);
            painter->setBrush(g);
            painter->drawEllipse(r);
        }

        painter->rotate(18);
    }
}
