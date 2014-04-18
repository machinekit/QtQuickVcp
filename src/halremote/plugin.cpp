#include "plugin.h"
#include "qpin.h"
#include "qcomponent.h"
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
    qmlRegisterType<QComponent>(uri, 1, 0, "RemoteComponent");
    qmlRegisterType<QAppConfig>(uri, 1, 0, "AppConfig");
    qmlRegisterType<QAppConfigItem>(uri, 1, 0, "AppConfigItem");
    qmlRegisterType<QAppConfigFilter>(uri, 1, 0, "AppConfigFilter");
    qmlRegisterType<QPin>(uri, 1, 0, "Pin");
}


