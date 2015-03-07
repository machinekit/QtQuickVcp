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

#ifndef KNOB_H
#define KNOB_H

#include <QQuickItem>
#include <QColor>
#include <QQmlEngine>
#include "component.h"
#include "../styles/arcstyle.h"
#include "../styles/piestyle.h"
#include "../styles/needlestyle.h"
#include "meter.h"
#include "dial.h"

#define GET(type, name) \
    type name() const { return m_ ## name; }

#define SET(type,Name,name) \
void set##Name(type arg) { if (m_##name == arg) return; m_##name = arg; emit name##Changed(arg); };

class Knob : public QQuickItem
{
    Q_OBJECT
    Q_ENUMS(Style)
    Q_ENUMS(NeedleType)
    Q_ENUMS(PieType)
    Q_ENUMS(Mode)
    Q_PROPERTY(qreal size READ size WRITE setSize NOTIFY sizeChanged)
    Q_PROPERTY(QColor color READ color WRITE setColor NOTIFY colorChanged)
    Q_PROPERTY(QColor backgroundColor READ backgroundColor WRITE setBackgroundColor NOTIFY backgroundColorChanged)
    Q_PROPERTY(QColor foregroundColor READ foregroundColor WRITE setForegroundColor NOTIFY foregroundColorChanged)
    Q_PROPERTY(QColor textColor READ textColor WRITE setTextColor NOTIFY textColorChanged)
    Q_PROPERTY(QColor borderColor MEMBER m_borderColor NOTIFY borderColorChanged)
    Q_PROPERTY(int maximumValue READ maximumValue WRITE setMaximumValue NOTIFY maximumValueChanged)
    Q_PROPERTY(Style style READ style WRITE setStyle NOTIFY styleChanged)
    Q_PROPERTY(NeedleType needleType READ needleType WRITE setNeedleType NOTIFY needleTypeChanged)
    Q_PROPERTY(PieType pieType READ pieType WRITE setPieType NOTIFY pieTypeChanged)
    Q_PROPERTY(bool pieMultiColor READ pieMultiColor WRITE setPieMultiColor NOTIFY pieMultiColorChanged)
    Q_PROPERTY(bool meter READ meter WRITE setMeter NOTIFY meterChanged)
    Q_PROPERTY(int value READ value WRITE setValue NOTIFY valueChanged)
    Q_PROPERTY(int percent READ percent WRITE setPercent NOTIFY percentChanged)
    Q_PROPERTY(bool readOnly READ readOnly WRITE setReadOnly NOTIFY readOnlyChanged)
    Q_PROPERTY(Mode mode READ mode WRITE setMode NOTIFY modeChanged)


public:
    explicit Knob(QQuickItem *parent = 0);
    ~Knob();

    enum Style {
        Pie = 0,
        Arc,
        Needle
    };

    enum NeedleType {
        Point = 5,
        Round,
        Groove
    };

    enum PieType {
        Flat = 8,
        Curve
    };

    enum Mode {
        Normal = 10,
        Percent
    };

    GET(qreal, size)
    GET(QColor, color)
    GET(QColor, backgroundColor)
    GET(QColor, foregroundColor)
    GET(QColor, textColor)
    GET(bool, pieMultiColor)
    GET(bool, meter)
    GET(int, percent)
    GET(int, value)
    GET(int, maximumValue)
    GET(bool, readOnly)
    GET(Style, style)
    GET(PieType, pieType)
    GET(NeedleType, needleType)
    GET(Mode, mode)

    void classBegin();
    void componentComplete();
    void base();
    void top();
    void mid();
    void showMeter();

protected:
    void makeConnections(QQuickItem &item);

protected slots:
    void setSize(qreal arg);
    SET(int,MaximumValue,maximumValue)
    SET(Style,Style,style)
    SET(PieType,PieType,pieType)
    SET(bool,PieMultiColor,pieMultiColor)
    SET(bool,Meter,meter)
    SET(int,Value,value)
    SET(QColor,Color,color)
    SET(QColor,BackgroundColor,backgroundColor)
    SET(QColor,ForegroundColor,foregroundColor)
    SET(QColor,TextColor,textColor)
    SET(NeedleType,NeedleType,needleType)
    SET(int,Percent,percent)
    SET(bool,ReadOnly,readOnly)
    SET(Mode,Mode,mode)

signals:
    void sizeChanged(int arg);
    void colorChanged(QColor arg);
    void backgroundColorChanged(QColor arg);
    void foregroundColorChanged(QColor arg);
    void textColorChanged(QColor arg);
    void maximumValueChanged(int arg);
    void styleChanged(Style arg);
    void pieTypeChanged(int arg);
    void pieMultiColorChanged(bool arg);
    void meterChanged(bool arg);
    void valueChanged(int arg);
    void needleTypeChanged(NeedleType arg);
    void percentChanged(int arg);
    void readOnlyChanged(bool arg);
    void modeChanged(Mode arg);
    void borderColorChanged(QColor arg);

private:
    qreal m_size;
    QColor m_color;
    QColor m_backgroundColor;
    QColor m_foregroundColor;
    QColor m_textColor;
    QColor m_borderColor;
    int m_value;
    int m_maximumValue;
    int m_percent;
    bool m_readOnly;
    bool m_pieMultiColor;
    bool m_meter;

    Style m_style;
    NeedleType m_needleType;
    PieType m_pieType;
    Mode m_mode;

    QQmlEngine *m_engine;
    Dial *m_base;
    QQuickItem *m_text;
};

#endif // KNOB_H
