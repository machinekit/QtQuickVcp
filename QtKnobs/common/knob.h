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
    Q_PROPERTY(qreal size READ size WRITE setSize NOTIFY sizeChanged)
    Q_PROPERTY(QColor color READ color WRITE setColor NOTIFY colorChanged)
    Q_PROPERTY(QColor backgroundColor READ backgroundColor WRITE setBackgroundColor NOTIFY backgroundColorChanged)
    Q_PROPERTY(QColor foregroundColor READ foregroundColor WRITE setForegroundColor NOTIFY foregroundColorChanged)
    Q_PROPERTY(QColor textColor READ textColor WRITE setTextColor NOTIFY textColorChanged)
    Q_PROPERTY(QColor borderColor MEMBER m_borderColor NOTIFY borderColorChanged)
    Q_PROPERTY(double minimumValue READ minimumValue WRITE setMinimumValue NOTIFY minimumValueChanged)
    Q_PROPERTY(double maximumValue READ maximumValue WRITE setMaximumValue NOTIFY maximumValueChanged)
    Q_PROPERTY(int decimals READ decimals WRITE setDecimals NOTIFY decimalsChanged)
    Q_PROPERTY(Style style READ style WRITE setStyle NOTIFY styleChanged)
    Q_PROPERTY(NeedleType needleType READ needleType WRITE setNeedleType NOTIFY needleTypeChanged)
    Q_PROPERTY(PieType pieType READ pieType WRITE setPieType NOTIFY pieTypeChanged)
    Q_PROPERTY(bool pieMultiColor READ pieMultiColor WRITE setPieMultiColor NOTIFY pieMultiColorChanged)
    Q_PROPERTY(bool meter READ meter WRITE setMeter NOTIFY meterChanged)
    Q_PROPERTY(double value READ value WRITE setValue NOTIFY valueChanged)
    Q_PROPERTY(bool readOnly READ readOnly WRITE setReadOnly NOTIFY readOnlyChanged)
    Q_PROPERTY(QString suffix READ suffix WRITE setSuffix NOTIFY suffixChanged)
    Q_PROPERTY(QString prefix READ prefix WRITE setPrefix NOTIFY prefixChanged)
    Q_PROPERTY(QFont font READ font WRITE setFont NOTIFY fontChanged)


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

    GET(qreal, size)
    GET(QColor, color)
    GET(QColor, backgroundColor)
    GET(QColor, foregroundColor)
    GET(QColor, textColor)
    GET(bool, pieMultiColor)
    GET(bool, meter)
    GET(double, value)
    GET(double, minimumValue)
    GET(double, maximumValue)
    GET(int, decimals)
    GET(bool, readOnly)
    GET(Style, style)
    GET(PieType, pieType)
    GET(NeedleType, needleType)
    GET(QString, suffix)
    GET(QString, prefix)
    GET(QFont, font)

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
    SET(double,MinimumValue,minimumValue)
    SET(double,MaximumValue,maximumValue)
    SET(int,Decimals,decimals)
    SET(Style,Style,style)
    SET(PieType,PieType,pieType)
    SET(bool,PieMultiColor,pieMultiColor)
    SET(bool,Meter,meter)
    SET(double,Value,value)
    SET(QColor,Color,color)
    SET(QColor,BackgroundColor,backgroundColor)
    SET(QColor,ForegroundColor,foregroundColor)
    SET(QColor,TextColor,textColor)
    SET(NeedleType,NeedleType,needleType)
    SET(bool,ReadOnly,readOnly)
    SET(QString,Prefix,prefix)
    SET(QString,Suffix,suffix)
    SET(QFont,Font,font)

signals:
    void sizeChanged(int arg);
    void colorChanged(QColor arg);
    void backgroundColorChanged(QColor arg);
    void foregroundColorChanged(QColor arg);
    void textColorChanged(QColor arg);
    void minimumValueChanged(double arg);
    void maximumValueChanged(double arg);
    void decimalsChanged(int arg);
    void styleChanged(Style arg);
    void pieTypeChanged(int arg);
    void pieMultiColorChanged(bool arg);
    void meterChanged(bool arg);
    void valueChanged(double arg);
    void needleTypeChanged(NeedleType arg);
    void readOnlyChanged(bool arg);
    void borderColorChanged(QColor arg);
    void suffixChanged(QString arg);
    void prefixChanged(QString arg);
    void fontChanged(QFont arg);

private:
    qreal m_size;
    QColor m_color;
    QColor m_backgroundColor;
    QColor m_foregroundColor;
    QColor m_textColor;
    QColor m_borderColor;
    double m_value;
    double m_minimumValue;
    double m_maximumValue;
    int m_decimals;
    bool m_readOnly;
    bool m_pieMultiColor;
    bool m_meter;
    QString m_suffix;
    QString m_prefix;
    QFont m_font;

    Style m_style;
    NeedleType m_needleType;
    PieType m_pieType;

    QQmlEngine *m_engine;
    Dial *m_base;
    QQuickItem *m_text;
};

#endif // KNOB_H
