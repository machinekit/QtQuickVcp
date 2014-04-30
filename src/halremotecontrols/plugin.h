#ifndef PLUGIN_H
#define PLUGIN_H

#include <QtQml/qqmlextensionplugin.h>
#include <QtQml/qqml.h>
#include <QtQml/qqmlengine.h>
#include <QFile>

class MachinekitHalRemoteControlsPlugin : public QQmlExtensionPlugin
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "org.qt-project.Qt.QQmlExtensionInterface")
public:
    void registerTypes(const char *uri);
    void initializeEngine(QQmlEngine *engine, const char *uri);
private:
    QString fileLocation() const;
    bool isLoadedFromResource() const;
};

#endif // PLUGIN_H

