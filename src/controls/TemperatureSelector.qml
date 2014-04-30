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
import QtQuick.Controls 1.0
import Machinekit.Controls 1.0

Rectangle {
    id: main

    property double maximumValue: 300
    property double minimumValue: 0
    property double currentValue: 200
    property double selectedValue: tempSlider.value
    property bool   heatingEnabled: false
    property double lowTempValue: 0
    property double highTempValue: 200
    property double criticalTempValue: 300
    property string title: qsTr("Title")
    property double tempRange: maximumValue - minimumValue

    width: 70
    height: 250
    color: "#00000000"

    Text {
        id: titleLabel

        width: 29; height: 14
        anchors { top: parent.top; topMargin: 0; right: parent.right; rightMargin: 0; left: parent.left; leftMargin: 0}
        text: main.title
        horizontalAlignment: Text.AlignHCenter
        font.pixelSize: 12
    }

    Rectangle {
        id: tempRect

        width: 16
        radius: 0
        anchors.top: titleLabel.bottom
        anchors.topMargin: 5
        anchors.bottom: tempSpin.top
        anchors.bottomMargin: 5
        anchors.left: tempSlider.right
        anchors.leftMargin: 5
        gradient: Gradient {
            GradientStop {
                position: (lowTempValue-minimumValue)/tempRange
                color: "#ff0000"
            }

            GradientStop {
                position: (highTempValue-minimumValue)/tempRange
                color: "#f7ff00"
            }

            GradientStop {
                position: (criticalTempValue-minimumValue)/tempRange
                color: "#00d1ff"
            }
        }
        border.width: 1
        border.color: "#000000"
    }

    Slider {
        id: tempSlider
        width: 15
        anchors.top: titleLabel.bottom
        anchors.topMargin: 5
        anchors.left: parent.left
        anchors.leftMargin: 0
        anchors.bottom: tempSpin.top
        anchors.bottomMargin: 5
        updateValueWhileDragging: true
        stepSize: 5
        tickmarksEnabled: false
        orientation: 0
        minimumValue: main.minimumValue
        maximumValue: main.maximumValue
    }

    Text {
        id: maxLabel
        y: 34
        text: maximumValue
        anchors.left: tempRect.right
        anchors.leftMargin: 5
        anchors.top: tempRect.top
        anchors.topMargin: 0
        font.pixelSize: 12
    }

    Text {
        id: minLabel
        y: 220
        width: 15
        height: 14
        text: minimumValue
        anchors.left: tempRect.right
        anchors.leftMargin: 5
        anchors.bottom: tempRect.bottom
        anchors.bottomMargin: 0
        horizontalAlignment: Text.AlignRight
        font.pixelSize: 12
    }

    Button {
        id: onOffButton
        y: 0
        height: 22
        text: "On"
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 0
        anchors.right: parent.right
        anchors.rightMargin: 0
        anchors.left: parent.left
        anchors.leftMargin: 0
        checkable: true
        checked: false
    }

    SpinBox {
        id: tempSpin
        y: 240
        height: 22
        anchors.bottom: onOffButton.top
        anchors.bottomMargin: 5
        anchors.right: parent.right
        anchors.rightMargin: 0
        anchors.left: parent.left
        anchors.leftMargin: 0
        maximumValue: main.maximumValue
        minimumValue: main.minimumValue
        decimals: 1
        value: main.selectedValue
        onValueChanged: {
            tempSlider.value = value
        }
    }

    Rectangle {
        id: indicator1
        height: 2
        color: "#000000"
        anchors.bottom: tempRect.bottom
        anchors.bottomMargin: (currentValue-minimumValue)/tempRange*tempRect.height
        z: 1
        anchors.right: tempRect.right
        anchors.rightMargin: 0
        anchors.left: tempRect.left
        anchors.leftMargin: 0
    }

    Text {
        id: currentLabel
        text: currentValue
        anchors.left: tempRect.right
        anchors.leftMargin: 5
        anchors.bottom: tempRect.bottom
        anchors.bottomMargin: (currentValue-minimumValue)/tempRange*tempRect.height-height/2
        font.pixelSize: 12
    }

    Rectangle {
        id: indicator2
        y: 95
        height: 2
        color: "#a09999"
        anchors.bottom: tempRect.bottom
        anchors.bottomMargin: (selectedValue-minimumValue)/tempRange*tempRect.height
        anchors.right: tempRect.right
        anchors.rightMargin: 0
        anchors.left: tempRect.left
        anchors.leftMargin: 0
    }


}
