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
#include "pluginprivate.h"

static void initResources()
{
    Q_INIT_RESOURCE(applicationcontrols);
    Q_INIT_RESOURCE(applicationcontrolsprivate);
}

static const struct {
    const char *type;
    int major, minor;
} qmldir [] = {
    { "ConnectionWindow", 1, 0 },
    { "ServiceWindow", 1, 0 },
    { "EstopAction", 1, 0 },
    { "PowerAction", 1, 0 },
    { "OpenAction", 1, 0 },
    { "ReopenAction", 1, 0 },
    { "RunProgramAction", 1, 0 },
    { "StepProgramAction", 1, 0 },
    { "PauseResumeProgramAction", 1, 0 },
    { "StopProgramAction", 1, 0 },
    { "ApplicationNotifications", 1, 0 },
    { "ApplicationProgressBar", 1, 0 },
    { "ApplicationFileDialog", 1, 0 },
    { "JogStick", 1, 0 },
    { "AbstractDigitalReadOut", 1, 0 },
    { "DigitalReadOut", 1, 0 },
    { "FeedrateSlider", 1, 0 },
    { "FeedrateHandler", 1, 0 },
    { "SpindlerateSlider", 1, 0 },
    { "SpindlerateHandler", 1, 0 },
    { "MdiCommandAction", 1, 0 },
    { "JogAction", 1, 0 },
    { "JogButton", 1, 0 },
    { "JogDistanceComboBox", 1, 0 },
    { "JogDistanceHandler", 1, 0},
    { "SpindleCwAction", 1, 0 },
    { "SpindleCcwAction", 1, 0 },
    { "StopSpindleAction", 1, 0 },
    { "IncreaseSpindleSpeedAction", 1, 0 },
    { "DecreaseSpindleSpeedAction", 1, 0 },
    { "HomeAxisAction", 1, 0 },
    { "MistAction", 1, 0 },
    { "FloodAction", 1, 0 },
    { "MaximumVelocitySlider", 1, 0 },
    { "MaximumVelocityHandler", 1, 0 },
    { "TouchOffAction", 1, 0 },
    { "TouchOffDialog", 1, 0 },
    { "GCodeLabel", 1, 0 },
    { "JogVelocityHandler", 1, 0 },
    { "JogVelocitySlider", 1, 0 },
    { "UnhomeAxisAction", 1, 0 },
    { "MdiHistoryTable", 1, 0 },
    { "MdiCommandEdit", 1, 0 },
    { "AxisRadioGroup", 1, 0 },
    { "AxisComboBox", 1, 0 },
    { "TeleopAction", 1, 0 },
    { "ShutdownAction", 1, 0 },
    { "UserCommandAction", 1, 0 },
    { "ApplicationRemoteFileDialog", 1, 0 },
    { "OverrideLimitsAction", 1, 0 },
    { "OptionalStopAction", 1, 0 },
    { "BlockDeleteAction", 1, 0 },
    { "FeedHoldAction", 1, 0 },
    { "FeedOverrideAction", 1, 0 },
    { "SpindleOverrideAction", 1, 0 },
    { "EstopPowerAction", 1, 0},
    { "RapidrateHandler", 1, 0},
    { "RapidrateSlider", 1, 0},
    { "EditWithSystemEditorAction", 1, 0},
    { "ToolTableEditor", 1, 0},
    { "ToolTableEditorDialog", 1, 0},
    { "EditToolTableAction", 1, 0}
};

void MachinekitApplicationControlsPlugin::registerTypes(const char *uri)
{
    initResources();

    // @uri Machinekit.Application.Controls
    Q_ASSERT(uri == QLatin1String("Machinekit.Application.Controls"));
    const QString filesLocation = fileLocation();
    for (int i = 0; i < int(sizeof(qmldir)/sizeof(qmldir[0])); i++) {
        qmlRegisterType(QUrl(filesLocation + "/" + qmldir[i].type + ".qml"), uri, qmldir[i].major, qmldir[i].minor, qmldir[i].type);
    }
}

void MachinekitApplicationControlsPlugin::initializeEngine(QQmlEngine *engine, const char *uri)
{
    Q_UNUSED(uri);

    if (isLoadedFromResource())
        engine->addImportPath(QStringLiteral("qrc:/"));


    const QString filesLocation = fileLocation();
    const char *private_uri = "Machinekit.Application.Controls.Private";
    for (int i = 0; i < int(sizeof(qmldirprivate)/sizeof(qmldirprivate[0])); i++) {
        qmlRegisterType(QUrl(filesLocation + "/Private/" + qmldirprivate[i].type + ".qml"), private_uri,
                        qmldirprivate[i].major, qmldirprivate[i].minor, qmldirprivate[i].type);
    }

    qDebug() << QLocale().name();

    if (m_translator.load(QLocale(), QLatin1String("machinekitapplicationcontrols"),
                          QLatin1String("_"), QLibraryInfo::location(QLibraryInfo::TranslationsPath)))
    {
        QCoreApplication::installTranslator(&m_translator);
    }
}

QString MachinekitApplicationControlsPlugin::fileLocation() const
{
    if (isLoadedFromResource())
        return QStringLiteral("qrc:/Machinekit/Application/Controls");
    return baseUrl().toString();
}

bool MachinekitApplicationControlsPlugin::isLoadedFromResource() const
{
    // If one file is missing, it will load all the files from the resource
    QFile file(baseUrl().toLocalFile() + "/EstopAction.qml");
    if (!file.exists())
        return true;
    return false;
}
