#ifndef FILEIO_H
#define FILEIO_H

#include <QObject>
#include <QUrl>

namespace qtquickvcp {

class FileIO : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QUrl fileUrl READ fileUrl WRITE setFileName NOTIFY fileNameChanged)
    Q_PROPERTY(QString text READ text WRITE setText NOTIFY textChanged)
    Q_PROPERTY(bool working READ working NOTIFY workingChanged)

public:
    explicit FileIO(QObject *parent = 0);
    ~FileIO();

    QUrl fileUrl() const
    {
        return m_fileUrl;
    }

    QString text() const
    {
        return m_text;
    }

    bool working() const
    {
        return m_working;
    }

public slots:
    void write();
    void read();

    void setFileName(const QUrl &fileName)
    {
        if (m_fileUrl == fileName)
            return;

        m_fileUrl = fileName;
        emit fileNameChanged(fileName);
    }

    void setText(const QString &text)
    {
        if (m_text == text)
            return;

        m_text = text;
        emit textChanged(text);
    }

    QUrl createTempFile(const QString &fileName);

signals:
    void writingCompleted();
    void readingCompleted();
    void error(const QString &error);
    void fileNameChanged(const QUrl &fileUrl);
    void textChanged(const QString &text);
    void workingChanged(bool working);

private:
    QUrl m_fileUrl;
    QString m_text;
    bool m_working;
    QTemporaryDir *m_temporaryDir;

    void setWorking(bool working)
    {
        if (m_working == working)
            return;

        m_working = working;
        emit workingChanged(working);
    }

}; // class FileIO
} // namespace qtquickvcp

#endif  // FILEIO_H
