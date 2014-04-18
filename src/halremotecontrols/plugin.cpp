#include "plugin.h"

static void initResources()
{
    Q_INIT_RESOURCE(halremotecontrols);
}

static const struct {
    const char *type;
    int major, minor;
} qmldir [] = {
    { "ApplicationWindow", 1, 0 },
    { "Button", 1, 0 },
    { "ColorPicker", 1, 0 },
    { "DiscoveryPage", 1, 0 },
    { "Gauge", 1, 0 },
    { "Led", 1, 0 },
    { "LogChart", 1, 0 },
    { "ProgressBar", 1, 0 },
    { "RadioButton", 1, 0 },
    { "Slider", 1, 0 },
    { "ToggleButton", 1, 0 },
    { "VirtualJoystick", 1, 0 }
};

void MachinekitHalRemoteControlsPlugin::registerTypes(const char *uri)
{
    initResources();

    const QString filesLocation = fileLocation();
    for (int i = 0; i < int(sizeof(qmldir)/sizeof(qmldir[0])); i++) {
        // @uri Machinekit.HalRemote.Controls
        qmlRegisterType(QUrl(filesLocation + "/" + qmldir[i].type + ".qml"), uri, qmldir[i].major, qmldir[i].minor, qmldir[i].type);
        }
}

void MachinekitHalRemoteControlsPlugin::initializeEngine(QQmlEngine *engine, const char *uri)
{
    Q_UNUSED(uri);

    if (isLoadedFromResource())
        engine->addImportPath(QStringLiteral("qrc:/"));
}

QString MachinekitHalRemoteControlsPlugin::fileLocation() const
{
    if (isLoadedFromResource())
        return "qrc:/Machinekit/HalRemote/Controls";
    return baseUrl().toString();
}

bool MachinekitHalRemoteControlsPlugin::isLoadedFromResource() const
{
    // If one file is missing, it will load all the files from the resource
    QFile file(baseUrl().toLocalFile() + "/ApplicationWindow.qml");
    if (!file.exists())
        return true;
    return false;
}
