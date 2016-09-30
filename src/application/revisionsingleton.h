#ifndef REVISIONSINGLETON_H
#define REVISIONSINGLETON_H

#include <QObject>
#include <QQmlEngine>
#include <QJSEngine>
#include "revision.h"

class RevisionSingleton : public QObject
{
    Q_OBJECT
    Q_DISABLE_COPY(RevisionSingleton)
    Q_PROPERTY(QString name READ name NOTIFY nameChanged)

    RevisionSingleton() {
    }

public:
    static QObject *qmlInstance(QQmlEngine *engine, QJSEngine *scriptEngine)
    {
        Q_UNUSED(engine);
        Q_UNUSED(scriptEngine);

        return new RevisionSingleton;
    }

    QString name() const
    {
        return QString(REVISION);
    }

signals:
    void nameChanged(QString name);
};

#endif // REVISIONSINGLETON_H
