/****************************************************************************
** Knobs is a free Qt and QML based widget.
** Copyright (C) 2013 Ashish Dabhade
** Email: ashishd157@gmail.com
**
** This program is free software: you can redistribute it and/or modify
** it under the terms of the GNU Lesser General Public License as published by
** the Free Software Foundation, either version 2 of the License, or
** (at your option) any later version.
**
** This program is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
** GNU General Public License for more details.
**
** You should have received a copy of the GNU General Public License
** along with this program (in doc/ directory).
** If not, see <http://www.gnu.org/licenses/gpl-3.0.html>.
****************************************************************************/

#include <QtGui/QGuiApplication>
#include "qtquick2applicationviewer.h"
#include "arcstyle.h"
#include "piestyle.h"
#include "needlestyle.h"
#include "meter.h"

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);
    qmlRegisterType<ArcStyle>("ArcStyle", 1, 0, "ArcStyle");
    qmlRegisterType<PieStyle>("PieStyle", 1, 0, "PieStyle");
    qmlRegisterType<NeedleStyle>("NeedleStyle", 1, 0, "NeedleStyle");
    qmlRegisterType<Meter>("Meter", 1, 0, "Meter");

    QtQuick2ApplicationViewer viewer;
    viewer.setMainQmlFile(QStringLiteral("qml/Knobs/main.qml"));
    viewer.showExpanded();

    return app.exec();
}
