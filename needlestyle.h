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

#ifndef NEEDLESTYLE_H
#define NEEDLESTYLE_H

#include <QQuickPaintedItem>
#include <qmath.h>
#include <QDebug>
#include <QPainter>

class NeedleStyle : public QQuickPaintedItem
{
    Q_OBJECT
    Q_ENUMS(Style)
    Q_PROPERTY(Style style READ style WRITE setstyle NOTIFY styleChanged)
    Q_PROPERTY(QColor color READ color WRITE setColor)

public:
    explicit NeedleStyle(QQuickItem *parent = 0);
    void paint(QPainter *painter);
    Q_INVOKABLE int endValueFromPoint(qreal x = 0, qreal y = 0);

    enum Style {
        Point,
        Round,
        Groove
    };

    Style style() const;
    QColor color() const;

signals:
    void styleChanged(Style arg);

protected slots:
    void setstyle(Style arg);
    void setColor(QColor arg);

private:
    Style m_style;
    qreal m_scale;
    QColor m_color;
};

#endif // NEEDLESTYLE_H
