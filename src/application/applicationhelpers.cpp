#include "applicationhelpers.h"
#include <QDesktopServices>
#include <QLocale>
#include <QProcess>
#include <QSettings>
#include <QCoreApplication>

namespace qtquickvcp {

ApplicationHelpers::ApplicationHelpers(QQmlEngine *engine, QObject *parent)
    : QObject(parent)
    , m_engine(engine)
{

}

bool ApplicationHelpers::openUrlWithDefaultApplication(const QUrl &url) const
{
    return QDesktopServices::openUrl(url);
}

void ApplicationHelpers::clearQmlComponentCache() const
{
    m_engine->clearComponentCache();
}

QString ApplicationHelpers::currentLanguage() const
{
    const auto languages = QLocale().uiLanguages();
    return languages.first();
}

void ApplicationHelpers::setLanguage(const QString &language)
{
    QSettings settings;
    settings.setValue("language", language);
    settings.sync();
}

void ApplicationHelpers::restartApplication()
{
    qApp->quit();
    QProcess::startDetached(qApp->arguments()[0], qApp->arguments());
}

} // namespace qtquickvcp
