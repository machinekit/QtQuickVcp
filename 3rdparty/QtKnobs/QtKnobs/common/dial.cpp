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

#include "dial.h"

Dial::Dial(QQuickItem *parent)
    :QQuickPaintedItem(parent),
    m_bottom(true),
    m_color(QColor(241,241,241)),
    m_borderColor(Qt::darkGray)
{
    connect(this, SIGNAL(borderColorChanged(QColor)), this, SLOT(update()));
}

Dial::~Dial()
{}

void Dial::paint(QPainter *painter)
{
    if(m_bottom){
        painter->setPen(m_borderColor.lighter(110));
    }
    else{
        QLinearGradient g(QPointF(width()/2.0,0),QPointF(width()/2.0,height()));
        g.setColorAt(0.0,Qt::transparent);
        g.setColorAt(1.0,m_borderColor);
        QPen p;
        p.setBrush(g);
        painter->setPen(p);
    }
    painter->setBrush(m_color);
    painter->drawEllipse(boundingRect().adjusted(1,1,-1,-1));
}

