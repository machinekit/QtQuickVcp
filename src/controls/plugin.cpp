#include "plugin.h"

static void initResources()
{
    Q_INIT_RESOURCE(controls);
}

static const struct {
    const char *type;
    int major, minor;
} qmldir [] = {
    { "ColorPicker", 1, 0 },
    { "Line", 1, 0 },
    { "TemperatureSelector", 1, 0 },
    { "TouchButton", 1, 0 },
    { "VirtualJoystick", 1, 0 },
    { "ValueChart", 1, 0 },
    { "ValueModel", 1, 0 },
    { "LogChart", 1, 0 }
};

void MachinekitControlsPlugin::registerTypes(const char *uri)
{
    initResources();

    const QString filesLocation = fileLocation();
    for (int i = 0; i < int(sizeof(qmldir)/sizeof(qmldir[0])); i++) {
        // @uri Machinekit.Controls
        qmlRegisterType(QUrl(filesLocation + "/" + qmldir[i].type + ".qml"), uri, qmldir[i].major, qmldir[i].minor, qmldir[i].type);
        }
}

void MachinekitControlsPlugin::initializeEngine(QQmlEngine *engine, const char *uri)
{
    Q_UNUSED(uri);

    if (isLoadedFromResource())
        engine->addImportPath(QStringLiteral("qrc:/"));
}

QString MachinekitControlsPlugin::fileLocation() const
{
    if (isLoadedFromResource())
        return "qrc:/Machinekit/Controls";
    return baseUrl().toString();
}

bool MachinekitControlsPlugin::isLoadedFromResource() const
{
    // If one file is missing, it will load all the files from the resource
    QFile file(baseUrl().toLocalFile() + "/ColorPicker.qml");
    if (!file.exists())
        return true;
    return false;
}
