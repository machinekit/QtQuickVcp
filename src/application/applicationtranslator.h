#ifndef APPLICATIONTRANSLATOR_H
#define APPLICATIONTRANSLATOR_H

#include <QObject>
#include <QTranslator>
#include <QCoreApplication>
#include <QUrl>
#include <QLocale>

namespace qtquickvcp {

class ApplicationTranslator : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QUrl translationsPath READ translationsPath WRITE setTranslationsPath NOTIFY translationsPathChanged)
    Q_PROPERTY(QString applicationName READ applicationName WRITE setApplicationName NOTIFY applicationNameChanged)

public:
    explicit ApplicationTranslator(QObject *parent = 0);

    QUrl translationsPath() const
    {
        return m_translationsPath;
    }

    QString applicationName() const
    {
        return m_applicationName;
    }

public slots:

    void setTranslationsPath(const QUrl &translationsPath)
    {
        if (m_translationsPath == translationsPath)
            return;

        m_translationsPath = translationsPath;
        emit translationsPathChanged(translationsPath);
    }

    void setApplicationName(const QString &applicationName)
    {
        if (m_applicationName == applicationName)
            return;

        m_applicationName = applicationName;
        emit applicationNameChanged(applicationName);
    }

private:
    QUrl m_translationsPath;
    QString m_applicationName;
    QTranslator m_translator;

private slots:
    void updateTranslation();

signals:
    void translationsPathChanged(const QUrl &translationsPath);
    void applicationNameChanged(const QString &applicationName);
};
} // namespace qtquickvcp

#endif // APPLICATIONTRANSLATOR_H
