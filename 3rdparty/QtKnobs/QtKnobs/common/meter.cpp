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

#include "meter.h"
#include <QRadialGradient>
#include <QRect>

Meter::Meter(QQuickItem *parent) :
    QQuickPaintedItem(parent)
{
}

void Meter::paint(QPainter *painter)
{
    static qreal w = width()/30.0;
    static qreal p = (width()/2.0)-(width()/12.0);
    painter->translate(width() / 2.0, height() / 2.0);
    painter->scale(1.0,1.0);
    for (int i = 0; i < 20; ++i) {
        QRectF r(p-w/2.0,-w/2.0,w,w);

        QRadialGradient g(r.center(),w);
        g.setColorAt(0.0,QColor(0,191,255));
        g.setColorAt(1.0,Qt::black);

        painter->setPen(Qt::NoPen);
        painter->setBrush(g);
        painter->drawEllipse(r);
        painter->rotate(18);
    }
}
