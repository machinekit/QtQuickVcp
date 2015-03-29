#ifndef QTKNOBSPLUGIN_H
#define QTKNOBSPLUGIN_H

#include <QQmlExtensionPlugin>
#include "../common/knob.h"

class QtKnobsPlugin : public QQmlExtensionPlugin
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "com.fv.QtKnobs")

public:
    QtKnobsPlugin();
    ~QtKnobsPlugin();

    void registerTypes(const char *uri);
};

#endif // QTKNOBSPLUGIN_H
