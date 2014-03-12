#include "qtquick2controlsapplicationviewer.h"
#include <QtQml>
#include "hal/qpin.h"
#include "hal/qcomponent.h"

int main(int argc, char *argv[])
{
    Application app(argc, argv);

    qmlRegisterType<QComponent>("Hal", 1, 0, "Component");
    qmlRegisterType<QPin>("Hal", 1, 0, "Pin");

    QtQuick2ControlsApplicationViewer viewer;
    viewer.setMainQmlFile(QStringLiteral("qml/QtQuickVcp/main.qml"));
    viewer.show();

    return app.exec();
}
