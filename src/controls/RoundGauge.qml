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
** Derived from QtQuickCarGauges by lemirep: https://github.com/lemirep/QtQuickCarGauges
** Alexander Rössler @ The Cool Tool GmbH <mail DOT aroessler AT gmail DOT com>
**
****************************************************************************/
import QtQuick 2.2
import QtQuick.Controls 1.1

/*!
    \qmltype RoundGauge
    \inqmlmodule Machinekit.Controls
    \brief Provides a round gauge control to display data.
    \ingroup machinekitcontrols

    By default the gauge will be colored and have a range from 0.0 to 1.0.

    \qml
    RoundGauge {
        id: gauge
    }
    \endqml

    You can also use the gauge to visualize values growing in both directions
    from the \l zeroValue.

    \qml
    RoundGauge {
        id: gauge
        fancy: false
        minimumValue: -1.0
        maximumValue: 1.0
        zeroValue: 0.0
    }
    \endqml
*/

Item
{
    /*! Whether the values displayed on the gauge should be inverted or not.

        By default this is set to \c false.
    */
    property bool invert: false

    /*! This property indicates the amount of decimals.
      Note that if you enter more decimals than specified, they will
      be truncated to the specified amount of decimal places.

      The default value is \c{2}.
    */
    property int decimals: 2

    /*! The prefix for the value. I.e "$"
    */
    property string prefix: ""

    /*! The suffix for the value. I.e "cm"
    */
    property string suffix: ""

    /*! This property holds the gauge's current value.
        Attempting to change the current value to one outside the minimum-maximum
        range has no effect on the current value.

        The default value is \c{0}.
    */
    property double value : 0.0;

    /*! This property is the gauge's maximum value.
        The \l value is clamped to this value.
        If maximumValue is smaller than \l minimumValue, \l minimumValue will be enforced.
        The default value is \c{1}.
    */
    property double maximumValue: 1.0

    /*! This property is the gauge's minimum value.
        The \l value is clamped to this value.
        The default value is \c{0}.
    */
    property double minimumValue: 0.0

    /*! This property is the gauge's zero value.
        If the zero value is inside the range of \l minimumValue and \l maximumValue
        the bar will grow from that value and not from the left (or right) side
        of the gauge. Useful to represent value that may be both positive or negative

        By default this is bound to \l minimumValue.
    */
    property double zeroValue: minimumValue


    /*! This property holds the step size of the gauge's graduation

        The default value is \c{0.1}.
    */
    property double stepSize: 0.1

    /*! This property holds the color of value zone 0. Together with \l z1Color and
      \l z2Color this defines the colors for different zones. For description of
      zones see \l z0BorderValue.

      The default value is \c{green}
    */
    property color z0Color: fancy ? "green" : systemPalette.highlight

    /*! This property holds the color of value zone 1. Together with \l z0Color and
      \l z2Color this defines the colors for different zones. For description of
      zones see \l z0BorderValue.

      The default value is \c{yellow}
    */
    property color z1Color: "yellow"

    /*! This property holds the color value zone 2. Together with \l z0Color and
      \l z1Color this defines the colors for different zones. For description of
      zones see \l z0BorderValue.

      The default value is \c{red}
    */
    property color z2Color: "red"

    /*! This property holds the upper bound of value zone 0. Zone 0 will be filled
      with \l z0Color and smoothly transition to \l z1Color. If this property is
      greather than \l maximumValue only one color zone will be displayed. If
      \l zeroValue is greather than \l minimumValue the color zones will be mirrored
      to the left.

      The default value is \c{0.8}
    */
    property real z0BorderValue: 0.8

    /*! This property holds the upper bound of value zone 1. Zone 1 will be filled
      with \l z1Color and smoothly transition to \l z2Color. If this property is
      greather than \l maximumValue only two color zone will be displayed. If
      \l zeroValue is greather than \l minimumValue the color zones will be mirrored
      to the left.

      The default value is \c{0.9}
    */
    property real z1BorderValue: 0.9

    /*! This property holds the text color.
    */
    property color textColor: systemPalette.text

    /*! This property holds the color of the inner circling.
    */
    property color innerCirclingColor : systemPalette.dark;

    /*! This property holds the color of the outer circling.
    */
    property color outerCirclingColor : systemPalette.button;

    /*! This property holds the color of the graduation.
    */
    property color graduationColor : systemPalette.dark;

    /*! This property holds the background color.
    */
    property color backgroundColor : systemPalette.window;

    /*! This property holds the color of the needle.
    */
    property color needleColor: systemPalette.dark

    /*!
        \qmlproperty bool tickmarksEnabled

        This property indicates whether the gauge should display tickmarks
        at step intervals.

        The default value is \c true.
    */
    property bool tickMarksEnabled: true

    /*! This property holds whether the the gauge should be styled fancy
        or not. Fancy means that the value zones and colors are used for
        coloring the bar. If fancy is set to false the bar will be colored
        and styled in native system colors.

        By default this is set to \c true.
    */
    property bool fancy: true

    /*! This property holds whether the visualizer displaying the z color ranges
        should be visible or not.
    */
    property bool visualizerVisible: false

    /*! This property holds whether the indicator displaying the current value
        should be visible or not.
    */
    property bool indicatorVisible: true

    /*! This property holds whether the needle should be visible or not.
    */
    property bool needleVisible: true

    /*! \qmlproperty bool valueVisible

        This property holds whether the value should be visible or not.
    */
    property alias valueVisible : valueLabel.visible

    /*! This property holds whether the labels for the tickmarks should be visible or not.
    */
    property bool tickmarkLabelsVisible: false

    /*! This property holds the font of the tickmark labels.
    */
    property font tickmarkLabelsFont : dummyLabel.font

    /*! \qmlproperty Label valueLabel

        This property holds the \l Label used to display the value.
    */
    property alias valueLabel : valueLabel

    /*! This property holds wheter the full circle should be displayed or not.

        The default value is \c{false}.
    */
    property bool  fullCircle : false

    /*! This property holds whether the gauge should be animated or not.

        The default value is \c{false}.
    */
    property bool  animated: false

    /*! This property holds the width of the indicator displaying the gauge's value.
    */
    property int   indicatorWidth : canvas.width * 0.07

    /*! This property holds the width of the visualizer displaying the color zones.
    */
    property int   visualizerWidth: indicatorWidth * 0.8

    /*! This property holds the starting angle of the gauge. Must be in the range from 0 to 1.
    */
    property double startAngle: 0.125

    /*! This property holds the end angle of the gauge. Must be in the range from 0 to 1.
    */
    property double endAngle: 0.875

    id : main

    implicitWidth: 100
    implicitHeight: 100

    onMinimumValueChanged: updateIndicatorAndCanvas()
    onMaximumValueChanged: updateIndicatorAndCanvas()
    onZeroValueChanged: updateIndicatorAndCanvas()
    onZ0ColorChanged: updateIndicatorAndCanvas()
    onZ1ColorChanged: updateIndicatorAndCanvas()
    onZ2ColorChanged: updateIndicatorAndCanvas()
    onZ0BorderValueChanged: updateIndicatorAndCanvas()
    onZ1BorderValueChanged: updateIndicatorAndCanvas()
    onFancyChanged: updateIndicatorAndCanvas()
    onTickMarksEnabledChanged: updateIndicatorAndCanvas()
    onStartAngleChanged: updateIndicatorAndCanvas()
    onEndAngleChanged: updateIndicatorAndCanvas()
    onWidthChanged: updateAll()
    onHeightChanged: updateAll()
    onIndicatorVisibleChanged: updateCanvasAndNeedle()
    onVisualizerVisibleChanged: updateCanvas()
    onBackgroundColorChanged: updateCanvas()
    onGraduationColorChanged: updateCanvas()
    onInnerCirclingColorChanged: updateCanvas()
    onOuterCirclingColorChanged: updateCanvas()
    onFullCircleChanged: updateCanvas()
    onNeedleColorChanged: updateNeedle()

    /*! \internal */
    function updateCanvas() {
        canvas.requestPaint()
    }
    /*! \internal */
    function updateNeedle() {
        needleCanvas.requestPaint()
    }
    /*! \internal */
    function updateIndicatorAndCanvas() {
        indicator.requestPaint()
        canvas.requestPaint()
    }
    /*! \internal */
    function updateCanvasAndNeedle() {
        canvas.requestPaint()
        needleCanvas.requestPaint()
    }
    /*! \internal */
    function updateAll() {
        indicator.requestPaint()
        canvas.requestPaint()
        needleCanvas.requestPaint()
    }

    SystemPalette {
        id: systemPalette;
        colorGroup: main.enabled ? SystemPalette.Active : SystemPalette.Disabled
    }

    Label {
        id: dummyLabel
    }

    QtObject
    {
        id : d

        property int lineWidth: 1
        property int blurWidth: 2
        property int indicatorWidth: (indicatorVisible) ? main.indicatorWidth : 0
        property int smallSide: (main.width < main.height) ? main.width : main.height
        property double range : maximumValue - minimumValue;
        property double startAngle : Math.PI * (main.startAngle*2 + 0.5);
        property double endAngle : Math.PI * (main.endAngle*2 + 0.5);
        property double zeroAngle: startAngle + (endAngle - startAngle) * (main.zeroValue - main.minimumValue)/(main.maximumValue - main.minimumValue)
        property double radius : Math.min(canvas.width * 0.5, canvas.height * 0.5) - blurWidth
        property point  center : Qt.point(canvas.width * 0.5, canvas.height * 0.5)
        property double wholeAngle : endAngle - startAngle;
        property double rangedValue: Math.max(Math.min(value, maximumValue), minimumValue)
        property double currentValueRatio : (rangedValue - minimumValue) / d.range;
        property double needleAngleRad : d.startAngle + currentValueRatio * d.wholeAngle
        property double needleAngle : needleAngleRad  * 180 / Math.PI;

        Behavior on needleAngleRad {SpringAnimation {spring: 1.5; damping: 0.5; } enabled: main.animated}
        onNeedleAngleRadChanged: indicator.requestPaint();
        }

    Canvas {
        id: canvas

        height: d.smallSide
        width: height
        anchors.centerIn: parent
        contextType: "2d"
        smooth: true

        // DRAWN ONCE
        onPaint:
        {
            // INNER ARC WITH GRADIENT
            var fillGradiant = context.createRadialGradient(d.center.x, d.center.y, 0.1 * d.radius,
                                                            d.center.x, d.center.y, 0.75 * d.radius);
            fillGradiant.addColorStop(0, outerCirclingColor);
            fillGradiant.addColorStop(1, backgroundColor);

            context.beginPath();
            if (fullCircle)
                context.arc(d.center.x, d.center.y, d.radius - 0.001, 0 , 2 * Math.PI);
            else
                context.arc(d.center.x, d.center.y, d.radius - 0.001, Math.PI * 0.6666 , 2.3333 * Math.PI);
            context.fillStyle = fillGradiant;
            context.shadowBlur = d.blurWidth;
            context.shadowColor = outerCirclingColor;
            context.fill();

            context.beginPath();
            context.lineWidth = d.lineWidth;
            context.strokeStyle = innerCirclingColor;
            context.arc(d.center.x, d.center.y, d.radius - d.lineWidth*0.5, d.startAngle, d.endAngle);
            context.stroke();

            if (indicatorVisible) {
                // DRAW WHITE ARC CERCLING
                context.beginPath();
                context.lineWidth = d.lineWidth;
                context.strokeStyle = innerCirclingColor;
                context.arc(d.center.x, d.center.y, d.radius - (d.lineWidth*1.5 + d.indicatorWidth), d.startAngle, d.endAngle);
                context.stroke();
            }

            context.shadowBlur = 0;
            context.shadowColor = "transparent";

            if (fancy && visualizerVisible) {
                context.beginPath();
                context.lineWidth = d.lineWidth;
                context.strokeStyle = innerCirclingColor;
                context.arc(d.center.x, d.center.y, d.radius - (visualizerWidth + d.lineWidth*2.5 + d.indicatorWidth), d.startAngle, d.endAngle);
                context.stroke();

                // DRAW Z0 ARC
                context.beginPath();
                context.lineWidth = visualizerWidth;
                context.strokeStyle = z0Color;
                context.arc(d.center.x, d.center.y, d.radius - (visualizerWidth * 0.5 + d.lineWidth*2 + d.indicatorWidth), d.zeroAngle, d.startAngle + ((z0BorderValue - minimumValue) / d.range) * (d.wholeAngle));
                context.stroke();

                // DRAW Z1 ARC
                context.beginPath();
                context.lineWidth = visualizerWidth;
                context.strokeStyle = z1Color;
                context.arc(d.center.x, d.center.y, d.radius - (visualizerWidth * 0.5 + d.lineWidth*2 + d.indicatorWidth), d.startAngle + ((z0BorderValue - minimumValue) / d.range) * (d.wholeAngle), d.endAngle - (((d.range  - (z1BorderValue - minimumValue)) / d.range) * (d.wholeAngle)));
                context.stroke();

                // DRAW Z2 ARC
                context.beginPath();
                context.lineWidth = visualizerWidth;
                context.strokeStyle = z2Color;
                context.arc(d.center.x, d.center.y, d.radius - (visualizerWidth * 0.5 + d.lineWidth*2 + d.indicatorWidth), d.endAngle - (((d.range  - (z1BorderValue - minimumValue)) / d.range) * (d.wholeAngle)), d.endAngle);
                context.stroke();

                if (minimumValue != zeroValue) {
                    // DRAW Z0 ARC
                    context.beginPath();
                    context.lineWidth = visualizerWidth;
                    context.strokeStyle = z0Color;
                    context.arc(d.center.x, d.center.y,
                                d.radius - (visualizerWidth * 0.5 + d.lineWidth*2 + d.indicatorWidth),
                                Math.max(d.startAngle, d.zeroAngle - ((z0BorderValue - zeroValue) / d.range) * (d.wholeAngle)),
                                d.zeroAngle);
                    context.stroke();

                    // DRAW Z1 ARC
                    context.beginPath();
                    context.lineWidth = visualizerWidth;
                    context.strokeStyle = z1Color;
                    context.arc(d.center.x, d.center.y,
                                d.radius - (visualizerWidth * 0.5 + d.lineWidth*2 + d.indicatorWidth),
                                Math.max(d.startAngle, d.zeroAngle - (((z1BorderValue - zeroValue) / d.range) * (d.wholeAngle))),
                                Math.max(d.startAngle, d.zeroAngle - ((z0BorderValue - zeroValue) / d.range) * (d.wholeAngle)));
                    context.stroke();

                    // DRAW Z2 ARC
                    context.beginPath();
                    context.lineWidth = visualizerWidth;
                    context.strokeStyle = z2Color;
                    context.arc(d.center.x, d.center.y,
                                d.radius - (visualizerWidth * 0.5 + d.lineWidth*2 + d.indicatorWidth),
                                d.startAngle,
                                Math.max(d.startAngle, d.zeroAngle - (((z1BorderValue - zeroValue) / d.range) * (d.wholeAngle))));
                    context.stroke();
                }
            }

            // GRADUATIONS
            context.strokeStyle = graduationColor;
            context.lineWidth = visualizerWidth
            text_model.clear();

            var subDivs = (maximumValue - minimumValue) / stepSize
            var angleStep = (d.wholeAngle) / (subDivs)
            for (var i = 0; i <= subDivs; i++)
            {
                var rotAngle  = (d.startAngle) + i * (angleStep);

                if (tickMarksEnabled) {
                    var offset = 0.005//(subDivs % 2 !== 0) ? (i % 2 !== 0) ? 0.015 : 0.005 : 0.005
                    context.beginPath();
                    context.arc(d.center.x, d.center.y, d.radius - (visualizerWidth * 0.5 + d.lineWidth*2 + d.indicatorWidth), rotAngle - ((i === 0) ? 0 : offset), rotAngle + ((i === subDivs + 1) ? 0 : offset));
                    context.stroke();
                }

                var textVal = prefix + (((i * angleStep / d.wholeAngle) * d.range * 100) / 100 + minimumValue).toFixed(decimals) + suffix;
                text_model.append({"text" : textVal,
                                      "x" : d.center.x + ((d.radius - (((tickMarksEnabled ? visualizerWidth + d.lineWidth : 0) + d.indicatorWidth + d.lineWidth*2 + width * 0.03) + tickmarkLabelsFont.pixelSize * 0.5)) * Math.cos(rotAngle)),
                                      "y" : d.center.y + (d.radius - (((tickMarksEnabled ? visualizerWidth + d.lineWidth : 0) + d.indicatorWidth + d.lineWidth*2 + width * 0.03) + tickmarkLabelsFont.pixelSize * 0.5)) * Math.sin(rotAngle)});
            }
        }

        // REDRAWN WHEN GAUGE VALUE CHANGES
        Canvas
        {
            id : indicator

            anchors.fill: parent
            contextType: "2d"
            visible: indicatorVisible

            function getCadranColor()
            {
                if (!fancy)
                    return z0Color

                if (Math.abs(d.rangedValue-zeroValue) < (z0BorderValue-zeroValue))
                    return z0Color
                else if (Math.abs(d.rangedValue-zeroValue) < (z1BorderValue-zeroValue))
                    return z1Color
                else
                    return z2Color
            }

            property color targetColor : getCadranColor();
            Behavior on targetColor {ColorAnimation {duration: 750} enabled: main.animated}

            onPaint:
            {
                context.reset();
                context.beginPath();
                context.lineWidth = d.indicatorWidth;
                context.strokeStyle = targetColor;
                if (d.zeroAngle < d.needleAngleRad)
                    context.arc(d.center.x, d.center.y, d.radius - d.lineWidth - d.indicatorWidth * 0.5, d.zeroAngle, d.needleAngleRad);
                else
                    context.arc(d.center.x, d.center.y, d.radius - d.lineWidth - d.indicatorWidth * 0.5, d.needleAngleRad, d.zeroAngle);
                context.stroke();
            }
        }

        // NEEDLE HOLDER
        Rectangle
        {
            id : needleHolder

            visible: main.needleVisible
            width : d.radius * 0.4
            opacity : 0.8
            height: width
            radius: width/2
            x : d.center.x - width * 0.5
            y : d.center.y - height * 0.5

            color : systemPalette.window
        }

        // NEEDLE
        Item
        {
            id : needle

            visible: main.needleVisible
            width : d.radius - d.lineWidth - d.indicatorWidth
            height : needleCanvas.height
            x : d.center.x
            y : d.center.y

            Canvas
            {
                id : needleCanvas

                width : parent.width + needleHolder.width * 0.7
                height: needleHolder.width*0.2
                x : -needleHolder.width *0.7
                y: -height/2
                contextType: "2d"

                onPaint: {
                    context.reset();

                    context.strokeStyle = needleColor
                    context.fillStyle = needleColor
                    context.lineWidth = 10;

                    context.beginPath();
                    context.moveTo(width,height/2)
                    context.lineTo(0, 0);
                    context.lineTo(0, height);
                    context.fill()
                    context.closePath();
                }
            }

            transform : Rotation {
                angle : d.needleAngle
                axis { x : 0; y : 0; z : 1}
            }
        }

        // CENTER DOT
        Rectangle
        {
            width : d.radius * 0.1
            height : width
            radius : 180
            color : innerCirclingColor
            x : d.center.x - width * 0.5
            y : d.center.y - height * 0.5
            border
            {
                width : 1
                color : innerCirclingColor
            }
        }

        transform: Scale { origin.x: width/2; origin.y: height/2; xScale: ((!invert) ? 1 : -1)}
    }

    // VALUES
    Repeater
    {
        model :  ListModel {id : text_model}
        delegate : Component {
            Label
            {
                color : textColor
                font : tickmarkLabelsFont
                x : canvas.x + (invert ? parent.width -  (model.x + 0.5 * width) :  model.x - 0.5 * width)
                y : canvas.y + model.y - 0.5 * height
                text : model.text
                visible: main.tickmarkLabelsVisible
            }
        }
    }

    Label
    {
        id : valueLabel
        anchors
        {
            bottom : canvas.bottom
            bottomMargin : canvas.height * 0.15
            horizontalCenter : canvas.horizontalCenter
        }
        text : prefix + d.rangedValue.toFixed(decimals) + suffix;
        color : textColor
    }
}
