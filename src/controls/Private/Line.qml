/****************************************************************************
**
** Copyright (C) 2014 Alexander Rössler
** License: LGPL version 2.1
**
** This file is part of QtQuickVcp.
**
** All rights reserved. This program and the accompanying materials
** are made available under the terms of the GNU Lesser General Public License
** (LGPL) version 2.1 which accompanies this distribution, and is available at
** http://www.gnu.org/licenses/lgpl-2.1.html
**
** This library is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
** Lesser General Public License for more details.
**
** Contributors:
** Alexander Rössler @ The Cool Tool GmbH <mail DOT aroessler AT gmail DOT com>
**
****************************************************************************/
import QtQuick 2.0
import Machinekit.Controls 1.0

/*!
    \qmltype Line
    \inqmlmodule Machinekit.Controls
    \brief Provides a component for drawing a line in QML.
*/

Rectangle {
    id: line

    /*! \qmlproperty real x1

        This property holds x1 coordinate of the line.
    */
    property alias x1: line.x

    /*! \qmlproperty real y1

        This property holds y1 coordinate of the line.
    */
    property alias y1: line.y

    /*! \qmlproperty real x2

        This property holds x2 coordinate of the line.
    */
    property real x2: line.x

    /*! \qmlproperty real y2

        This property holds y2 coordinate of the line.
    */
    property real y2: line.y

    /*! \qmlproperty color lineColor

        This property holds the color of the line.
    */
    property alias lineColor: wrapper.color

    /*! \qmlproperty real lineWidth

        This property holds the width of the line.
    */
    property alias lineWidth: wrapper.height

    /*! \internal */
    function getWidth(sx1,sy1,sx2,sy2)
    {
        var w=Math.sqrt(Math.pow((sx2-sx1),2)+Math.pow((sy2-sy1),2));
        return w;
    }

    /*! \internal */
    function getSlope(sx1,sy1,sx2,sy2)
    {
        var a,m,d;
        var b=sx2-sx1;

        a=sy2-sy1;
        m=a/b;

        if ((b===0) && (a > 0))
            d = 90
        else if ((b===0) && (a <= 0))
            d = -90
        else
            d=Math.atan(m)*180/Math.PI;

        if (a<0 && b<0)
            return d+180;
        else if (a>=0 && b>=0)
            return d;
        else if (a<0 && b>=0)
            return d;
        else if (a>=0 && b<0)
            return d+180;
        else
            return 0;
    }

    transformOrigin: Item.TopLeft;

    width: getWidth(x1,y1,x2,y2);
    rotation: getSlope(x1,y1,x2,y2);
    color: "#00000000"

    Rectangle {
        id: wrapper

        color: "black"
        y: -height/2; x: - radius
        height: 10; width: parent.width + radius
        radius: height/2
        smooth: parent.smooth;
    }
}
