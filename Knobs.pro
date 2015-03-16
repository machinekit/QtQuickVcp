#/************************************************************************************
#** The MIT License (MIT)
#**
#** Knobs is a free Qt and QML based widget.
#** Copyright (c) 2015 Ashish Dabhade
#** Email: ashishd157@gmail.com
#**
#** Permission is hereby granted, free of charge, to any person obtaining a copy
#** of this software and associated documentation files (the "Software"), to deal
#** in the Software without restriction, including without limitation the rights
#** to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
#** copies of the Software, and to permit persons to whom the Software is
#** furnished to do so, subject to the following conditions:
#**
#** The above copyright notice and this permission notice shall be included in all
#** copies or substantial portions of the Software.
#**
#** THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
#** IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
#** FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
#** AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
#** LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
#** OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
#** SOFTWARE.
#************************************************************************************/

TEMPLATE = lib
QT += qml quick

uri = Knobs
include(QtKnobs/plugin.pri)

OBJECTS_DIR = tmp
MOC_DIR = tmp

SOURCES += QtKnobs/styles/piestyle.cpp \
    QtKnobs/styles/needlestyle.cpp \
    QtKnobs/styles/arcstyle.cpp \
    QtKnobs/common/meter.cpp \
    QtKnobs/common/knob.cpp \
    QtKnobs/plugin/qtknobsplugin.cpp \
    QtKnobs/common/dial.cpp

CONFIG += c++11

RESOURCES += QtKnobs/qml.qrc

HEADERS += \
    QtKnobs/styles/piestyle.h \
    QtKnobs/styles/needlestyle.h \
    QtKnobs/styles/arcstyle.h \
    QtKnobs/common/meter.h \
    QtKnobs/common/knob.h \
    QtKnobs/common/component.h \
    QtKnobs/plugin/qtknobsplugin.h \
    QtKnobs/common/dial.h

DISTFILES += \
    QtKnobs/examples/main.qml

QML_INFRA_FILES = Knobs/qmldir

include(QtKnobs/deployment.pri)
