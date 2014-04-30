#include "plugin.h"
#include "qhalpin.h"
#include "qhalremotecomponent.h"
#include "qservice.h"
#include "qservicediscovery.h"
#include "qappconfig.h"
#include "qappconfigitem.h"
#include "qappconfigfilter.h"

#include <qqml.h>

void MachinekitHalRemotePlugin::registerTypes(const char *uri)
{
    // Verify that the version of the library that we linked against is
    // compatible with the version of the headers we compiled against.
    GOOGLE_PROTOBUF_VERIFY_VERSION;

    // @uri Machinekit.HalRemote
    qmlRegisterType<QService>(uri, 1, 0, "Service");
    qmlRegisterType<QServiceDiscovery>(uri, 1, 0, "ServiceDiscovery");
    qmlRegisterType<QHalRemoteComponent>(uri, 1, 0, "HalRemoteComponent");
    qmlRegisterType<QAppConfig>(uri, 1, 0, "AppConfig");
    qmlRegisterType<QAppConfigItem>(uri, 1, 0, "AppConfigItem");
    qmlRegisterType<QAppConfigFilter>(uri, 1, 0, "AppConfigFilter");
    qmlRegisterType<QHalPin>(uri, 1, 0, "HalPin");
}


