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
#include <QApplication>
#include <QQmlApplicationEngine>
#include <QCommandLineParser>
#include <QIcon>
#include <QSettings>

#include "../../src/application/revision.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    app.setOrganizationName("Machinekit Project");
    app.setOrganizationDomain("machinekit.io");
    app.setApplicationName("MachinekitClient");
    app.setApplicationVersion(QString(REVISION));
    app.setWindowIcon(QIcon(":/icons/machinekit"));

    // load language from settings
    QSettings settings;
    const auto languages = QLocale().uiLanguages();
    const auto &language = settings.value("language", languages.first()).toString();
    QLocale::setDefault(QLocale(language));

    QCommandLineParser parser;
    parser.setApplicationDescription(QCoreApplication::translate("main", "MachinekitClient - A generic client for QtQuickVcp based user interfaces."));
    parser.addHelpOption();
    parser.addVersionOption();
    QCommandLineOption configOption(QStringList() << "c" << "config",
        QCoreApplication::translate("main", "Load configuration from <file>."),
        QCoreApplication::translate("main", "file"));
    parser.addOption(configOption);
#ifdef QT_DEBUG
    QCommandLineOption testDeploymentOption(QStringList() << "t" << "test-deployment",
        QCoreApplication::translate("main", "Test the application deployment and exit application."));
    parser.addOption(testDeploymentOption);
#endif
#ifdef Q_OS_WIN
    QCommandLineOption forceAngleOption(QStringList() << "a" << "angle",
          QCoreApplication::translate("main", "Force ANGLE OpenGL backend."));
    parser.addOption(forceAngleOption);
#endif
    parser.process(app);

#ifdef Q_OS_WIN
    if (parser.isSet(forceAngleOption)) {
        app.setAttribute(Qt::AA_UseOpenGLES);
    }
#endif

    QQmlApplicationEngine engine;
    engine.addImportPath(QStringLiteral("qml"));
    engine.addImportPath(QStringLiteral("assets:/qml"));
    engine.load(QUrl(QStringLiteral("qrc:///init.qml")));
#ifdef Q_OS_WIN
    engine.addImportPath(QStringLiteral("../../../imports")); // for in place execution
#else
    engine.addImportPath(QStringLiteral("../../imports")); // for in place execution
#endif
    return app.exec();
}
