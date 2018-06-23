#ifndef APPLICATIONHELPERS_H
#define APPLICATIONHELPERS_H

#include <QObject>
#include <QQmlEngine>

namespace qtquickvcp {

class ApplicationHelpers : public QObject
{
    Q_OBJECT

    Q_PROPERTY(QString currentLanguage READ currentLanguage CONSTANT)

public:
    explicit ApplicationHelpers(QQmlEngine *engine, QObject *parent = nullptr);

    static QObject *qmlSingletonProvider(QQmlEngine *engine, QJSEngine *scriptEngine)
    {
        Q_UNUSED(engine)
        Q_UNUSED(scriptEngine)

        return  new ApplicationHelpers(engine);
    }

    /**
     * Opens an URL with the default application provided by the system.
     * E.g. a text file will be opened by the default text editor.
     */
    Q_INVOKABLE bool openUrlWithDefaultApplication(const QUrl &url) const;

    /**
     * Clear QML component cache of the main QQmlEngine.
     */
    Q_INVOKABLE void clearQmlComponentCache() const;

    /**
     * Holds the currently active UI language.
     */
    QString currentLanguage() const;

    /**
     * Sets the new UI language. Changes take effect after a restart.
     */
    Q_INVOKABLE void setLanguage(const QString &language);

    /**
     * Restarts the application.
     */
    Q_INVOKABLE void restartApplication();

private:
    QQmlEngine *m_engine;

}; // class ApplicationHelpers
} // namespace qtquickvcp

#endif // APPLICATIONHELPERS_H
