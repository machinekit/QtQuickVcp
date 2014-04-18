#include "plugin.h"
#include "mjpegstreamerclient.h"
#include <qqml.h>

void MachinekitVideoViewPlugin::registerTypes(const char *uri)
{
    // Verify that the version of the library that we linked against is
    // compatible with the version of the headers we compiled against.
    GOOGLE_PROTOBUF_VERIFY_VERSION;

    // @uri Machinekit.VideoView
    qmlRegisterType<MjpegStreamerClient>(uri, 1, 0, "MjpegStreamerClient");
}
