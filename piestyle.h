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

#ifndef PIESTYLE_H
#define PIESTYLE_H

#include <QQuickPaintedItem>
#include <QPainter>
#include <qmath.h>

class PieStyle : public QQuickPaintedItem
{
    Q_OBJECT
    Q_PROPERTY(int endValue READ endValue WRITE setEndValue NOTIFY endValueChanged)
    Q_PROPERTY(QColor color READ color WRITE setColor)
    Q_PROPERTY(bool multicolor READ multicolor WRITE setmulticolor NOTIFY multicolorChanged)

public:
    explicit PieStyle(QQuickItem *parent = 0);

    void paint(QPainter *painter);

    QColor color() const;
    int startValue() const;
    int endValue() const;

    Q_INVOKABLE int endValueFromPoint(qreal x = 0, qreal y = 0);

    bool multicolor() const;

protected slots:
    void setmulticolor(bool arg);

signals:
    void endValueChanged(int arg);
    void multicolorChanged(bool arg);

private:
    int m_spanAngle;
    int m_startAngle;
    int m_endValue;
    int m_offset;

    QColor m_color;
    qreal m_scale;

    bool m_multicolor;

    QList<QColor> m_chunkColors;

protected slots:
    void setColor(QColor arg);
    void setEndValue(int arg);
};

#endif // PIESTYLE_H
