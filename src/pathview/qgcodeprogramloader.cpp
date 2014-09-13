#include "qgcodeprogramloader.h"

QGCodeProgramLoader::QGCodeProgramLoader(QObject *parent) :
    QObject(parent),
    m_localFilePath(""),
    m_localPath(""),
    m_remotePath(""),
    m_model(NULL)
{
}

void QGCodeProgramLoader::load()
{
    if (m_model == NULL)
    {
        emit loadingFailed();
        return;
    }

    QString localFilePath = QUrl(m_localFilePath).toLocalFile();
    QString localPath = QUrl(m_localPath).toLocalFile();
    QString remotePath = QUrl(m_remotePath).toLocalFile();
    QString remoteFilePath;

    if (localFilePath.indexOf(localPath) == 0)
    {
        remoteFilePath = remotePath + localFilePath.mid(localPath.length() + 1);
    }
    else
    {
        QFileInfo fileInfo(localFilePath);
        remoteFilePath = QDir(remotePath).filePath(fileInfo.fileName());
    }

    QFile file(localFilePath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        emit loadingFailed();
        return;
    }

    int lineNumber = 0;

    while (!file.atEnd()) {
        file.readLine();
        lineNumber++;
    }

    m_model->beginUpdate();
    m_model->prepareFile(remoteFilePath, lineNumber);

    lineNumber = 0;
    file.reset();
    while (!file.atEnd()) {
        QByteArray line = file.readLine();
        lineNumber++;
        m_model->setData(remoteFilePath, lineNumber, QString(line), QGCodeProgramModel::GCodeRole);
    }

    m_model->endUpdate();

    file.close();
    emit loadingFinished();
}
