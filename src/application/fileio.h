#ifndef FILEIO_H
#define FILEIO_H

#include <QObject>
#include <QUrl>
#include <QTemporaryDir>
#include <memory>

namespace qtquickvcp {

class FileIO : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QUrl fileUrl READ fileUrl WRITE setFileUrl NOTIFY fileUrlChanged)
    Q_PROPERTY(QString text READ text WRITE setText NOTIFY textChanged)
    Q_PROPERTY(bool working READ working NOTIFY workingChanged)

public:
    explicit FileIO(QObject *parent = 0);
    ~FileIO();

    QUrl fileUrl() const;
    QString text() const;
    bool working() const;

public slots:
    void write();
    void read();

    void setFileUrl(const QUrl &fileUrl);
    void setText(const QString &text);

    QUrl createTempFile(const QString &fileName);

signals:
    void writingCompleted();
    void readingCompleted();
    void error(const QString &error);
    void fileUrlChanged(const QUrl &fileUrl);
    void textChanged(const QString &text);
    void workingChanged(bool working);

private:
    QUrl m_fileUrl;
    QString m_text;
    bool m_working;
    std::unique_ptr<QTemporaryDir> m_temporaryDir;

    void setWorking(bool working);

}; // class FileIO
} // namespace qtquickvcp

#endif  // FILEIO_H
