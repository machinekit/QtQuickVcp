#include "qtquick2controlsapplicationviewer.h"
#include <QtQml>
#include "hal/qpin.h"
#include "hal/qcomponent.h"
#include "hal/qservice.h"
#include "hal/qservicediscovery.h"
#include "hal/qappconfig.h"
#include "hal/qappconfigitem.h"
#include "hal/qappconfigfilter.h"

int main(int argc, char *argv[])
{
    Application app(argc, argv);

    // Verify that the version of the library that we linked against is
    // compatible with the version of the headers we compiled against.
    GOOGLE_PROTOBUF_VERIFY_VERSION;

    qmlRegisterType<QService>("Hal", 1, 0, "Service");
    qmlRegisterType<QServiceDiscovery>("Hal", 1, 0, "ServiceDiscovery");
    qmlRegisterType<QComponent>("Hal", 1, 0, "RemoteComponent");
    qmlRegisterType<QAppConfig>("Hal", 1, 0, "AppConfig");
    qmlRegisterType<QAppConfigItem>("Hal", 1, 0, "AppConfigItem");
    qmlRegisterType<QAppConfigFilter>("Hal", 1, 0, "AppConfigFilter");
    qmlRegisterType<QPin>("Hal", 1, 0, "Pin");

    QtQuick2ControlsApplicationViewer viewer;
    viewer.addImportPath(QStringLiteral("qml"));
    viewer.addImportPath(QStringLiteral("assets:/qml"));
    viewer.setMainQmlFile(QStringLiteral("qml/main.qml"));
    viewer.show();

    return app.exec();
}
