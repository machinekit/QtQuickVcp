#ifndef QLOCALSETTINGS_H
#define QLOCALSETTINGS_H

#include <QObject>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonValueRef>
#include <QJsonDocument>
#include <QFileInfo>
#include <QDir>
#include <QStandardPaths>

class QLocalSettings : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QJsonObject values READ values WRITE setValues NOTIFY valuesChanged)
    Q_PROPERTY(QString name READ name WRITE setName NOTIFY nameChanged)
    Q_PROPERTY(QString application READ application WRITE setApplication NOTIFY applicatioChanged)

public:
    explicit QLocalSettings(QObject *parent = 0);
    ~QLocalSettings();

    QJsonObject values() const
    {
        return m_values;
    }

    QString application() const
    {
        return m_application;
    }

    QString name() const
    {
        return m_name;
    }

signals:
    void valuesChanged(QJsonObject arg);
    void applicatioChanged(QString arg);
    void nameChanged(QString arg);

public slots:
    void setValues(QJsonObject arg);
    void setValue(const QString &key, const QVariant &value, bool overwrite);
    void setApplication(QString arg);
    void setName(QString arg);
    void save();
    void load();
    void setValue(const QString &key, const QVariant &value);

private:
    QJsonObject m_values;
    QString     m_application;
    QString     m_name;
    QString     m_filePath;

    void loadSettings();
    void saveSettings();
    void updateFilePath();
};
#endif // QLOCALSETTINGS_H
