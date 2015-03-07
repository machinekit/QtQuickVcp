#include "qtknobsplugin.h"

QtKnobsPlugin::QtKnobsPlugin()
{}

QtKnobsPlugin::~QtKnobsPlugin()
{}

void QtKnobsPlugin::registerTypes(const char *uri)
{
    Q_ASSERT(uri == QLatin1String("Knobs"));
    qmlRegisterType<Knob>(uri, 1, 0, "Knob");
    qmlRegisterType<ArcStyle>(uri, 1, 1, "ArcStyle");
    qmlRegisterType<PieStyle>(uri, 1, 1, "PieStyle");
    qmlRegisterType<NeedleStyle>(uri, 1, 1, "NeedleStyle");
    qmlRegisterType<Meter>(uri, 1, 0, "Meter");
}

