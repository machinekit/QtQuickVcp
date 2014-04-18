#ifndef QMLHALPLUGIN_PLUGIN_H
#define QMLHALPLUGIN_PLUGIN_H

#include <QQmlExtensionPlugin>

class MachinekitVideoViewPlugin : public QQmlExtensionPlugin
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "org.qt-project.Qt.QQmlExtensionInterface/1.0")

public:
    void registerTypes(const char *uri);
};

#endif // QMLHALPLUGIN_PLUGIN_H

