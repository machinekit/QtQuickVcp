#ifndef FILEWATCHER_H
#define FILEWATCHER_H

#include <QObject>
#include <QUrl>
#include <QFileSystemWatcher>

namespace qtquickvcp {

class FileWatcher : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QUrl fileUrl READ fileUrl WRITE setFileUrl NOTIFY fileUrlChanged)
    Q_PROPERTY(bool enabled READ enabled WRITE setEnabled NOTIFY enabledChanged)

public:
    explicit FileWatcher(QObject *parent = nullptr);

    QUrl fileUrl() const;
    bool enabled() const;

signals:
    void fileChanged();
    void fileUrlChanged(const QUrl &fileUrl);
    void enabledChanged(bool enabled);

public slots:
    void setFileUrl(const QUrl &fileUrl);
    void setEnabled(bool enabled);

private:
    QUrl m_fileUrl;
    bool m_enabled;
    QFileSystemWatcher m_fileSystemWatcher;

private slots:
    void updateWatchedFile();
    void onWatchedFileChanged();

}; // class FileWatcher
} // namespace qtquickvcp

#endif // FILEWATCHER_H
