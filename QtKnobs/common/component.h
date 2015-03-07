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

#ifndef COMPONENT_H
#define COMPONENT_H

#include <QString>

class Component
{
public:
    Component();
    ~Component();

    static QByteArray getComponent(QString type, QString mode="value") {
        if(type=="Rectangle") {
            return "import QtQuick 2.0\nRectangle{anchors.centerIn:parent}";
        } else if(type=="Gradient") {
            return "import QtQuick 2.0"
                   "\nimport QtGraphicalEffects 1.0"
                   "\nLinearGradient{\ngradient:Gradient{"
                   "\nGradientStop{position:0.0;color:\"transparent\"}"
                   "\nGradientStop{position:1.0;color:\"darkgray\"}"
                   "\n}}";
        } else if(type=="Text") {
            return "import QtQuick 2.0\nText{anchors.centerIn:parent}";
        } else if(type=="ArcStyle"||type=="PieStyle"||type=="NeedleStyle") {
            return "import QtQuick 2.0"
                    "\nimport Knobs 1.1"
                    "\n"+type.toLatin1()+"{"
                    "\nid:chunk"
                    "\nanchors.fill:parent"
                    "\nEvents{id:events}"
                    "\nBehavior on "+mode.toLatin1()+"{"
                    "\nenabled:!events.pressed"
                    "\nSmoothedAnimation{"
                    "\nid:animate"
                    "\nduration:800"
                    "}}}";
        }

        return "";
    }
};

#endif // COMPONENT_H
