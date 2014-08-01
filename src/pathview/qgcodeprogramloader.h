#ifndef QGCODEPROGRAMLOADER_H
#define QGCODEPROGRAMLOADER_H

#include <QObject>
#include <QFile>
#include "qgcodeprogrammodel.h"

class QGCodeProgramLoader : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString fileName READ fileName WRITE setFileName NOTIFY fileNameChanged)
    Q_PROPERTY(QGCodeProgramModel *model READ model WRITE setModel NOTIFY modelChanged)

public:
    explicit QGCodeProgramLoader(QObject *parent = 0);

    QString fileName() const
    {
        return m_fileName;
    }

    QGCodeProgramModel * model() const
    {
        return m_model;
    }

signals:
    void fileNameChanged(QString arg);

    void modelChanged(QGCodeProgramModel * arg);

public slots:
    void load();

    void setFileName(QString arg)
    {
        if (m_fileName != arg) {
            m_fileName = arg;
            emit fileNameChanged(arg);
        }
    }

    void setModel(QGCodeProgramModel * arg)
    {
        if (m_model != arg) {
            m_model = arg;
            emit modelChanged(arg);
        }
    }

private:
    QString m_fileName;
    QGCodeProgramModel * m_model;
};

#endif // QGCODEPROGRAMLOADER_H
