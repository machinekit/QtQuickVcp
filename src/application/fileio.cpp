#include "fileio.h"

namespace qtquickvcp {

FileIO::FileIO(QObject *parent) :
    QObject(parent),
    m_fileUrl(""),
    m_text(""),
    m_working(false)
{

}

/* Write text to file */
void FileIO::write()
{

    if (!m_fileUrl.isValid() || !m_fileUrl.isLocalFile())
    {
        emit error(tr("File url is not valid"));
        return;
    }

    QString fileName = m_fileUrl.toLocalFile();
    QFile file(fileName);
    if (!file.open(QFile::WriteOnly | QFile::Text))
    {
        emit error(tr("Cannot open file"));
        return;
    }

    setWorking(true);
    QTextStream out(&file);
    out << m_text;
    file.close();
    setWorking(false);

    emit writingCompleted();
}

/* Read text from file */
void FileIO::read()
{    
    if (!m_fileUrl.isValid() || !m_fileUrl.isLocalFile())
    {
        emit error(tr("File url is not valid"));
        return;
    }

    QString fileName = m_fileUrl.toLocalFile();
    QFile file(fileName);
    if (!file.open(QFile::ReadOnly | QFile::Text))
    {
        emit error(tr("Cannot open file"));
        return;
    }

    setWorking(true);
    QTextStream in(&file);
    m_text = in.readAll();
    file.close();
    setWorking(false);

    emit textChanged(m_text);
    emit readingCompleted();
}
}; // namespace qtquickvcp
