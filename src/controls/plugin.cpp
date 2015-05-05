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
#include "plugin.h"
#include "dummy.h"

static void initResources()
{
    Q_INIT_RESOURCE(controls);
}

static const struct {
    const char *type;
    int major, minor;
} qmldir [] = {
    { "Dial", 1, 0 },
    { "Gauge", 1, 0 },
    { "ColorPicker", 1, 0 },
    { "Knob", 1, 0 },
    { "Led", 1, 0 },
    { "Line", 1, 0 },
    { "LogChart", 1, 0 },
    { "RoundGauge", 1, 0 },
    { "SlideView", 1, 0 },
    { "SlidePage", 1, 0 },
    { "TemperatureSelector", 1, 0 },
    { "TouchButton", 1, 0 },
    { "VirtualJoystick", 1, 0 },
    { "ValueChart", 1, 0 },
    { "ValueModel", 1, 0 },
    { "LogChart", 1, 0 }
};

void MachinekitControlsPlugin::registerTypes(const char *uri)
{
    initResources();

    // @uri Machinekit.Controls
    Q_ASSERT(uri == QLatin1String("Machinekit.Controls"));
    qmlRegisterType<Dummy>(uri, 1, 0, "Dummy");

    const QString filesLocation = fileLocation();
    for (int i = 0; i < int(sizeof(qmldir)/sizeof(qmldir[0])); i++) {
        qmlRegisterType(QUrl(filesLocation + "/" + qmldir[i].type + ".qml"), uri, qmldir[i].major, qmldir[i].minor, qmldir[i].type);
        }
}

void MachinekitControlsPlugin::initializeEngine(QQmlEngine *engine, const char *uri)
{
    Q_UNUSED(uri);

    if (isLoadedFromResource())
        engine->addImportPath(QStringLiteral("qrc:/"));
}

QString MachinekitControlsPlugin::fileLocation() const
{
    if (isLoadedFromResource())
        return "qrc:/Machinekit/Controls";
    return baseUrl().toString();
}

bool MachinekitControlsPlugin::isLoadedFromResource() const
{
    // If one file is missing, it will load all the files from the resource
    QFile file(baseUrl().toLocalFile() + "/ColorPicker.qml");
    if (!file.exists())
        return true;
    return false;
}
