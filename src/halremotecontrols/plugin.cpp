#include "plugin.h"
#include "dummy.h"

static void initResources()
{
    Q_INIT_RESOURCE(halremotecontrols);
}

static const struct {
    const char *type;
    int major, minor;
} qmldir [] = {
    { "HalApplicationWindow", 1, 0 },
    { "HalButton", 1, 0 },
    { "HalCheckBox", 1, 0 },
    { "HalColorPicker", 1, 0 },
    { "HalComboBox", 1, 0 },
    { "HalContainer", 1, 0 },
    { "HalDiscoveryPage", 1, 0 },
    { "HalGauge", 1, 0 },
    { "HalLabel", 1, 0 },
    { "HalLed", 1, 0 },
    { "HalLogChart", 1, 0 },
    { "HalProgressBar", 1, 0 },
    { "HalRadioButton", 1, 0 },
    { "HalSlider", 1, 0 },
    { "HalSpinBox", 1, 0 },
    { "HalVirtualJoystick", 1, 0 }
};

void MachinekitHalRemoteControlsPlugin::registerTypes(const char *uri)
{
    initResources();

    // @uri Machinekit.HalRemote.Controls
    qmlRegisterType<Dummy>(uri, 1, 0, "Dummy");

    const QString filesLocation = fileLocation();
    for (int i = 0; i < int(sizeof(qmldir)/sizeof(qmldir[0])); i++) {
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
