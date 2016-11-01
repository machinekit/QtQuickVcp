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
//  A toy QML colorpicker control, by Ruslan Shestopalyuk
import QtQuick 2.0
import QtQuick.Controls 1.0
import QtQuick.Layouts 1.0
import QtQuick.Window 2.0

/*!
    \qmltype ColorPicker
    \inqmlmodule Machinekit.Controls
    \brief Provides a beautiful color picker.
    \ingroup machinekitcontrols

    The color picker provides an easy to use control to select colors. You can
    use the selected color by using the \l colorValue property.

    \qml
    ColorPicker {
        id: colorPicker
    }
    \endqml
*/

Item {
    /*!
        \qmlproperty bool ColorPicker::showAlpha

        Whether the alpha channel selection should be visible or not. By default this is
        set to false.
    */
    property bool alphaVisible: false

    /*!
        \qmlproperty int ColorPicker::colorValue

        Returns the color selected in the color picker.
    */
    readonly property color colorValue: hsba(hueSlider.item.value, sbPicker.item.saturation,
                                               sbPicker.item.brightness, alphaSlider.item.value)

    /*!
        \qmlmethod ColorPicker::hsba(double h, double s, double b, double a)

        Creates color value from hue, saturation, brightness, alpha.
    */
    function hsba(h, s, b, a) {
        var lightness = (2 - s)*b;
        var satHSL = s*b/((lightness <= 1) ? lightness : 2 - lightness);
        lightness /= 2;
        return Qt.hsla(h, satHSL, lightness, a);
    }

    /*!
        \qmlmethod ColorPicker::fullColorString(color clr, double a)

        Creates a full color string from color value and alpha[0..1], e.g. "#FF00FF00".
    */
    function fullColorString(clr, a) {
        return "#" + ((Math.ceil(a*255) + 256).toString(16).substr(1, 2) +
                clr.toString().substr(1, 6)).toUpperCase();
    }

    /*!
        \qmlmethod ColorPicker::getChannelStr(color clr, int channelIdx)

        Extracts integer color channel value [0..255] from color value.
    */
    function getChannelStr(clr, channelIdx) {
        return parseInt(clr.toString().substr(channelIdx*2 + 1, 2), 16);
    }

    id: colorPicker
    width: 340; height: 250
    implicitWidth: 340; implicitHeight: 250

    Component {
        id: checkerBoard
        Grid {
            id: root
            property int cellSide: 5
            anchors.fill: parent
            rows: Math.round(height/cellSide); columns: Math.round(width/cellSide)
            clip: true
            Repeater {
                model: root.columns*root.rows
                Rectangle {
                    width: root.cellSide; height: root.cellSide
                    color: (index % 2 === 0) ? "gray" : "white"
                }
            }
        }
    }

    Component {
        id: colorSlider
        Item {
            property real value: visible?Math.max(1 - pickerCursor.y/height, 0.0):1.0

            id: main

            width: 15; height: 300
            Item {
                id: pickerCursor
                width: parent.width
                Rectangle {
                     y: -height*0.5
                    width: parent.width; height: main.height * 0.03
                    border.color: "black"; border.width: 1
                    color: "transparent"
                    Rectangle {
                        anchors.fill: parent; anchors.margins: 2
                        border.color: "white"; border.width: 1
                        color: "transparent"
                    }
                }
            }
            MouseArea {
                anchors.fill: parent
                function handleMouse(mouse) {
                    if (mouse.buttons & Qt.LeftButton) {
                        pickerCursor.y = Math.max(0, Math.min(height, mouse.y));
                    }
                }
                onPositionChanged: handleMouse(mouse)
                onPressed: handleMouse(mouse)
            }
        }
    }

    Component {
        id: numberBox
        RowLayout {
            property alias  caption: captionBox.text
            property alias  value: inputBox.text
            property alias  min: numValidator.bottom
            property alias  max: numValidator.top
            property alias  decimals: numValidator.decimals

            width: 80;
            height: 15
            spacing: 5
            Label {
                id: captionBox
                Layout.fillHeight: true
                //color: "#AAAAAA"
                font.pixelSize: 11; font.bold: true
                horizontalAlignment: Text.AlignRight; verticalAlignment: Text.AlignVCenter
            }
            TextField {
                id:  inputBox
                Layout.fillHeight: true
                Layout.fillWidth: true
                validator: DoubleValidator {
                    id: numValidator
                    bottom: 0; top: 1; decimals: 2
                    notation: DoubleValidator.StandardNotation
                }
            }
        }
    }

    Component {
        id: sbPickerComponent
        Item {
            id: root
            property real hueColor : 1.0
            property real saturation : pickerCursor.x/width
            property real brightness : 1 - pickerCursor.y/height
            width: 200; height: 300
            clip: true
            Rectangle {
                height: parent.width
                width: parent.height
                x: (width - parent.width) / 2
                y: (parent.height - height) / 2
                transform: Rotation {
                    angle: -90
                    origin.x: width/2
                    origin.y: height/2
                }

                gradient: Gradient {
                    GradientStop { position: 0.0; color: "#FFFFFF" }
                    GradientStop { position: 1.0; color: Qt.hsla(hueColor,1.0,0.5,1.0) }
                }
            }
            Rectangle {
                anchors.fill: parent
                gradient: Gradient {
                    GradientStop { position: 1.0; color: "#FF000000" }
                    GradientStop { position: 0.0; color: "#00000000" }
                }
            }
            Item {
                id: pickerCursor
                property int r : 8
                Rectangle {
                    x: -parent.r; y: -parent.r
                    width: parent.r*2; height: parent.r*2
                    radius: parent.r
                    border.color: "black"; border.width: 2
                    color: "transparent"
                    Rectangle {
                        anchors.fill: parent; anchors.margins: 2;
                        border.color: "white"; border.width: 2
                        radius: width/2
                        color: "transparent"
                    }
                }
            }
            MouseArea {
                anchors.fill: parent
                function handleMouse(mouse) {
                    if (mouse.buttons & Qt.LeftButton) {
                        pickerCursor.x = Math.max(0, Math.min(width,  mouse.x));
                        pickerCursor.y = Math.max(0, Math.min(height, mouse.y));
                    }
                }
                onPositionChanged: handleMouse(mouse)
                onPressed: handleMouse(mouse)
            }
        }
    }

    RowLayout {
        anchors.fill: parent
        spacing: 0

        // hue picking slider
        Item {
            id: huePicker

            Layout.preferredWidth: parent.width * 0.05
            Layout.fillHeight: true
            Rectangle {
                anchors.fill: parent
                gradient: Gradient {
                    GradientStop { position: 1.0;  color: "#FF0000" }
                    GradientStop { position: 0.85; color: "#FFFF00" }
                    GradientStop { position: 0.76; color: "#00FF00" }
                    GradientStop { position: 0.5;  color: "#00FFFF" }
                    GradientStop { position: 0.33; color: "#0000FF" }
                    GradientStop { position: 0.16; color: "#FF00FF" }
                    GradientStop { position: 0.0;  color: "#FF0000" }
                }
            }
            Loader { sourceComponent: colorSlider; id: hueSlider; anchors.fill: parent }
        }

        // alpha (transparency) picking slider
        Item {
            id: alphaPicker

            visible: alphaVisible
            Layout.preferredWidth: parent.width * 0.05
            Layout.fillHeight: true
            Loader {
                id: alphaBoard
                sourceComponent: checkerBoard
                anchors.fill: parent
                Binding { target: alphaBoard.item; property: "cellSide"; value: alphaPicker.width * 0.08}
            }
            //  alpha intensity gradient background
            Rectangle {
                anchors.fill: parent
                gradient: Gradient {
                    GradientStop { position: 0.0; color: "#FF000000" }
                    GradientStop { position: 1.0; color: "#00000000" }
                }
            }
            Loader { sourceComponent: colorSlider; active: true; id: alphaSlider; anchors.fill: parent }
        }

        // saturation/brightness picker box
        Loader {
            sourceComponent: sbPickerComponent;
            id: sbPicker

            Layout.fillHeight: true
            Layout.fillWidth: true

            Binding { target: sbPicker.item; property: "hueColor"; value: hueSlider.item.value}
        }

        // details column
        Item {
            Layout.fillHeight: true
            Layout.preferredWidth: parent.width*0.25

            ColumnLayout {
                anchors.fill: parent
                anchors.margins: Screen.pixelDensity * 0.5
                spacing: Screen.pixelDensity

                // current color/alpha display rectangle
                Item {
                    id: currentColorRect
                    Layout.fillWidth: true
                    Layout.preferredHeight: colorPicker.height*0.15
                    Loader {
                        id: colorBoard
                        sourceComponent: checkerBoard
                        anchors.fill: parent
                        Binding {
                            target: colorBoard.item
                            property: "cellSide"
                            value: currentColorRect.width * 0.0040
                        }
                    }
                    Rectangle {
                        anchors.fill: parent
                        border.width: 1; border.color: "black"
                        color: colorPicker.colorValue
                    }
                }

                // "#XXXXXXXX" color value box
                TextField {
                    id: colorEditBox

                    Layout.fillWidth: true
                    Layout.fillHeight: false
                    text: fullColorString(colorPicker.colorValue, alphaSlider.item.value)
                    maximumLength: 9
                    focus: true
                    //selectByMouse: true
                }

                // filler rectangle
                Item {
                    Layout.fillWidth: true
                    Layout.fillHeight: false
                }

                // H, S, B color values boxes
                Loader {
                    id: hBox
                    sourceComponent: numberBox;
                    Layout.fillWidth: true
                    Layout.fillHeight: false
                    onLoaded: item.caption = "H"
                    Binding { target: hBox.item; property: "value"; value: hueSlider.item.value.toFixed(2) }
                }
                Loader {
                    id: sBox
                    sourceComponent: numberBox;
                    Layout.fillWidth: true
                    Layout.fillHeight: false
                    onLoaded: item.caption = "S"
                    Binding { target: sBox.item; property: "value"; value: sbPicker.item.saturation.toFixed(2) }
                    }
                Loader {
                    id: vBox
                    Layout.fillWidth: true
                    Layout.fillHeight: false
                    onLoaded: item.caption = "V"
                    Binding { target: vBox.item; property: "value"; value: sbPicker.item.brightness.toFixed(2) }
                    }

                // filler rectangle
                Item {
                    Layout.fillWidth: true
                    Layout.fillHeight: false
                }

                // R, G, B color values boxes
                Loader {
                    id: rBox
                    sourceComponent: numberBox;
                    Layout.fillWidth: true;
                    Layout.fillHeight: false
                    onLoaded: { item.caption = "R"; item.min = 0; item.max = 255 }
                    Binding { target: rBox.item; property: "value"; value: getChannelStr(colorPicker.colorValue, 0) }
                }
                Loader {
                    id: gBox
                    sourceComponent: numberBox;
                    Layout.fillWidth: true;
                    Layout.fillHeight: false
                    onLoaded: { item.caption = "G"; item.min = 0; item.max = 255 }
                    Binding { target: gBox.item; property: "value"; value: getChannelStr(colorPicker.colorValue, 1) }
                }
                Loader {
                    id: bBox
                    sourceComponent: numberBox;
                    Layout.fillWidth: true;
                    Layout.fillHeight: false
                    onLoaded: { item.caption = "B"; item.min = 0; item.max = 255 }
                    Binding { target: bBox.item; property: "value"; value: getChannelStr(colorPicker.colorValue, 2) }
                }

                // alpha value box
                Loader {
                    id: aBox
                    sourceComponent: numberBox;
                    Layout.fillWidth: true;
                    Layout.fillHeight: false
                    visible: alphaVisible
                    onLoaded: { item.caption = "A"; item.min = 0; item.max = 255 }
                    Binding { target: aBox.item; property: "value"; value: Math.ceil(colorPicker.colorValue.a * 255) }
                }

                // filler rectangle
                Item {
                    Layout.fillWidth: true
                    Layout.fillHeight: true
                }
            }
        }
    }
}
