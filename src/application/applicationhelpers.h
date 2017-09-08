#ifndef APPLICATIONHELPERS_H
#define APPLICATIONHELPERS_H

#include <QObject>
#include <QQmlEngine>

namespace qtquickvcp {

class ApplicationHelpers : public QObject
{
    Q_OBJECT
public:
    explicit ApplicationHelpers(QObject *parent = nullptr);

    static QObject *qmlSingletonProvider(QQmlEngine *engine, QJSEngine *scriptEngine)
    {
        Q_UNUSED(engine)
        Q_UNUSED(scriptEngine)

        return  new ApplicationHelpers();
    }

    /**
     * Opens an URL with the default application provided by the system.
     * E.g. a text file will be opened by the default text editor.
     */
    Q_INVOKABLE bool openUrlWithDefaultApplication(const QUrl &url) const;

}; // class ApplicationHelpers
} // namespace qtquickvcp

#endif // APPLICATIONHELPERS_H
