#include "applicationtranslator.h"

namespace qtquickvcp {

ApplicationTranslator::ApplicationTranslator(QObject *parent) :
    QObject(parent),
    m_translationsPath(QLatin1String("")),
    m_applicationName(QLatin1String(""))
{
    connect(this, &ApplicationTranslator::translationsPathChanged,
            this, &ApplicationTranslator::updateTranslation);
    connect(this, &ApplicationTranslator::applicationNameChanged,
            this, &ApplicationTranslator::updateTranslation);
}

void ApplicationTranslator::updateTranslation()
{
    QString path;
    QString name;

    if (m_translationsPath.isLocalFile())
    {
        path = m_translationsPath.toLocalFile();
    }
    else
    {
        path = m_translationsPath.toString();
        if (path.indexOf(QLatin1String("qrc")) == 0) {
            path = path.mid(6);
            path.prepend(":");
        }
    }

    name = m_applicationName.toLower();
    name.replace(' ', QLatin1String(""));

    if (!path.isEmpty()) {
        QCoreApplication::removeTranslator(&m_translator);
        if (m_translator.load(QLocale(), name, QLatin1String("_"), path))
        {
             QCoreApplication::installTranslator(&m_translator);
        }
    }
}

} // namespace qtquickvcp
