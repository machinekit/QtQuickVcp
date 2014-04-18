#ifndef QAPPCONFIGITEM_H
#define QAPPCONFIGITEM_H

#include <QQuickItem>
#include "message.pb.h"
#include "types.pb.h"
#include "config.pb.h"

class QAppConfigItem : public QQuickItem
{
    Q_OBJECT
    Q_PROPERTY(QString name READ name WRITE setName NOTIFY nameChanged)
    Q_PROPERTY(QString description READ description WRITE setDescription NOTIFY descriptionChanged)
    Q_PROPERTY(ApplicationType type READ type WRITE setType NOTIFY typeChanged)
    Q_PROPERTY(QUrl webUri READ webUri WRITE setWebUri NOTIFY webUriChanged)
    Q_PROPERTY(bool loaded READ isLoaded WRITE setLoaded NOTIFY loadedChanged)
    Q_PROPERTY(QStringList files READ files WRITE setFiles NOTIFY filesChanged)
    Q_PROPERTY(QUrl mainFile READ mainFile WRITE setMainFile NOTIFY mainFileChanged)
    Q_ENUMS(ApplicationType)

public:
    explicit QAppConfigItem(QQuickItem *parent = 0);

    enum ApplicationType {
        QT5_QML = pb::QT5_QML,
        GLADEVCP = pb::GLADEVCP,
        JAVASCRIPT = pb::JAVASCRIPT
    };

    QString name() const
    {
        return m_name;
    }

    QString description() const
    {
        return m_description;
    }

    ApplicationType type() const
    {
        return m_type;
    }

    QUrl webUri() const
    {
        return m_webUri;
    }

    bool isLoaded() const
    {
        return m_loaded;
    }

    QStringList files() const
    {
        return m_files;
    }

    QUrl mainFile() const
    {
        return m_mainFile;
    }

public slots:

    void setName(QString arg)
    {
        if (m_name != arg) {
            m_name = arg;
            emit nameChanged(arg);
        }
    }
    void setDescription(QString arg)
    {
        if (m_description != arg) {
            m_description = arg;
            emit descriptionChanged(arg);
        }
    }
    void setType(ApplicationType arg)
    {
        if (m_type != arg) {
            m_type = arg;
            emit typeChanged(arg);
        }
    }
    void setWebUri(QUrl arg)
    {
        if (m_webUri != arg) {
            m_webUri = arg;
            emit webUriChanged(arg);
        }
    }
    void setLoaded(bool arg)
    {
        if (m_loaded != arg) {
            m_loaded = arg;
            emit loadedChanged(arg);
        }
    }
    void setFiles(QStringList arg)
    {
        if (m_files != arg) {
            m_files = arg;
            emit filesChanged(arg);
        }
    }
    void setMainFile(QUrl arg)
    {
        if (m_mainFile != arg) {
            m_mainFile = arg;
            emit mainFileChanged(arg);
        }
    }

private:
    QString m_name;
    QString m_description;
    ApplicationType m_type;
    QUrl m_webUri;
    bool m_loaded;
    QStringList m_files;
    QUrl m_mainFile;

signals:

    void nameChanged(QString arg);

    void descriptionChanged(QString arg);

    void typeChanged(ApplicationType arg);

    void webUriChanged(QUrl arg);

    void loadedChanged(bool arg);

    void filesChanged(QStringList arg);

    void mainFileChanged(QUrl arg);


};

#endif // QAPPCONFIGITEM_H
