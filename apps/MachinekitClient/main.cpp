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
#include <QIcon>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    app.setOrganizationName("Machinekit Project");
    app.setOrganizationDomain("machinekit.io");
    app.setApplicationName("MachinekitClient");
#ifdef Q_OS_WIN
    app.setAttribute(Qt::AA_UseOpenGLES);
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
