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
    Q_PROPERTY(bool recursive READ recursive WRITE setRecursive NOTIFY recursiveChanged)
    Q_PROPERTY(QStringList nameFilters READ nameFilters WRITE setNameFilters NOTIFY nameFiltersChanged)

public:
    explicit FileWatcher(QObject *parent = nullptr);

    QUrl fileUrl() const;
    bool enabled() const;
    bool recursive() const;
    bool verbose() const;
    QStringList nameFilters() const;

signals:
    void fileChanged();
    void fileUrlChanged(const QUrl &fileUrl);
    void enabledChanged(bool enabled);
    void recursiveChanged(bool recursive);
    void nameFiltersChanged(const QStringList &nameFilters);

public slots:
    void setFileUrl(const QUrl &fileUrl);
    void setEnabled(bool enabled);
    void setRecursive(bool recursive);
    void setNameFilters(const QStringList &nameFilters);

private:
    QUrl m_fileUrl;
    bool m_enabled;
    bool m_recursive;
    QFileSystemWatcher m_fileSystemWatcher;
    QStringList m_nameFilters;

private slots:
    void updateWatchedFile();
    void onWatchedFileChanged();
    void onWatchedDirectoryChanged(const QString &);

}; // class FileWatcher
} // namespace qtquickvcp

#endif // FILEWATCHER_H
