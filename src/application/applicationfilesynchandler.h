#ifndef APPLICATIONFILESYNCHANDLER_H
#define APPLICATIONFILESYNCHANDLER_H

#include <QObject>
#include <QUrl>

namespace qtquickvcp {

class ApplicationFileSyncHandler : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString remoteFilePath READ remoteFilePath WRITE setRemoteFilePath NOTIFY remoteFilePathChanged)
    Q_PROPERTY(QString remotePath READ remotePath WRITE setRemotePath NOTIFY remotePathChanged)
    Q_PROPERTY(bool ignoreNextChange READ ignoreNextChange WRITE setIgnoreNextChange NOTIFY ingoreNextChangeChanged)
    Q_PROPERTY(bool ready READ isReady WRITE setReady NOTIFY readyChanged)

public:
    explicit ApplicationFileSyncHandler(QObject *parent = nullptr);

    QString remoteFilePath() const;
    QString remotePath() const;
    bool ignoreNextChange() const;
    bool isReady() const;

signals:
    void remoteFilePathChanged(const QString &remoteFilePath);
    void remotePathChanged(const QString &remotePath);
    void ingoreNextChangeChanged(bool ignoreNextChange);
    void readyChanged(bool ready);

    void startFileDownload(const QString &filePath);
    void programUnloaded();

public slots:
    void setRemoteFilePath(const QString &remoteFilePath);
    void setRemotePath(const QString &remotePath);
    void setIgnoreNextChange(bool ignoreNextChange);
    void setReady(bool ready);

private:
    QString m_remoteFilePath;
    QString m_remotePath;
    bool m_ignoreNextChange;
    bool m_ready;
    QString m_lastRemoteFilePath;

private slots:
    void checkRemoteFile();

}; // class ApplicationFileSyncHandler
} // namespace qtquickvcp

#endif // APPLICATIONFILESYNCHANDLER_H
