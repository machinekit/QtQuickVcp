#include "plugin.h"
#include "pathview3d.h"
#include <qqml.h>

void MachinekitPathViewPlugin::registerTypes(const char *uri)
{
    // @uri Machinekit.PathView
    qmlRegisterType<PathView3d>(uri, 1, 0, "PathView3d");
}
