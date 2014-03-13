#include "qtquick2controlsapplicationviewer.h"
#include <QtQml>
#include "hal/qpin.h"
#include "hal/qcomponent.h"
#include "hal/qservice.h"
#include "hal/qservicediscovery.h"

int main(int argc, char *argv[])
{
    Application app(argc, argv);

    qmlRegisterType<QService>("Hal", 1, 0, "Service");
    qmlRegisterType<QServiceDiscovery>("Hal", 1, 0, "ServiceDiscovery");
    qmlRegisterType<QComponent>("Hal", 1, 0, "RemoteComponent");
    qmlRegisterType<QPin>("Hal", 1, 0, "Pin");

    QtQuick2ControlsApplicationViewer viewer;
    viewer.setMainQmlFile(QStringLiteral("qml/QtQuickVcp/main.qml"));
    viewer.show();

    return app.exec();
}
