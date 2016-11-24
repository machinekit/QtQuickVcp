#include "applicationtranslator.h"

namespace qtquickvcp {

ApplicationTranslator::ApplicationTranslator(QObject *parent) :
    QObject(parent),
    m_translationsPath(""),
    m_applicationName("")
{
    connect(this, &ApplicationTranslator::translationsPathChanged,
            this, &ApplicationTranslator::updateTranslation);
    connect(this, &ApplicationTranslator::applicationNameChanged,
            this, &ApplicationTranslator::updateTranslation);
}

void ApplicationTranslator::updateTranslation()
{
    const QString &localPath = m_translationsPath.toLocalFile();

    if (!localPath.isEmpty()) {
        QCoreApplication::removeTranslator(&m_translator);
        if (m_translator.load(QLocale(), m_applicationName.toLower(),
            QLatin1String("_"), localPath))
        {
             QCoreApplication::installTranslator(&m_translator);
        }
    }
}

} // namespace qtquickvcp
