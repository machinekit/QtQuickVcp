#ifndef QGCODEPROGRAMLOADER_H
#define QGCODEPROGRAMLOADER_H

#include <QObject>
#include <QFile>
#include <QFileInfo>
#include <QDir>
#include <QUrl>
#include "qgcodeprogrammodel.h"

class QGCodeProgramLoader : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString localFilePath READ localFilePath WRITE setLocalFilePath NOTIFY localFilePathChanged)
    Q_PROPERTY(QString localPath READ localPath WRITE setLocalPath NOTIFY localPathChanged)
    Q_PROPERTY(QString remotePath READ remotePath WRITE setRemotePath NOTIFY remotePathChanged)
    Q_PROPERTY(QGCodeProgramModel *model READ model WRITE setModel NOTIFY modelChanged)

public:
    explicit QGCodeProgramLoader(QObject *parent = 0);

    QString localFilePath() const
    {
        return m_localFilePath;
    }

    QString localPath() const
    {
        return m_localPath;
    }

    QString remotePath() const
    {
        return m_remotePath;
    }

    QGCodeProgramModel * model() const
    {
        return m_model;
    }

signals:
    void localFilePathChanged(QString arg);
    void localPathChanged(QString arg);
    void remotePathChanged(QString arg);
    void modelChanged(QGCodeProgramModel * arg);
    void loadingFinished();
    void loadingFailed();

public slots:
    void load();

    void setLocalFilePath(QString arg)
    {
        if (m_localFilePath != arg) {
            m_localFilePath = arg;
            emit localFilePathChanged(arg);
        }
    }

    void setLocalPath(QString arg)
    {
        if (m_localPath == arg)
            return;

        m_localPath = arg;
        emit localPathChanged(arg);
    }

    void setRemotePath(QString arg)
    {
        if (m_remotePath == arg)
            return;

        m_remotePath = arg;
        emit remotePathChanged(arg);
    }

    void setModel(QGCodeProgramModel * arg)
    {
        if (m_model != arg) {
            m_model = arg;
            emit modelChanged(arg);
        }
    }

private:
    QString m_localFilePath;
    QString m_localPath;
    QString m_remotePath;
    QGCodeProgramModel * m_model;
};

#endif // QGCODEPROGRAMLOADER_H
