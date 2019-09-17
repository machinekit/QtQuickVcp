#ifndef REVISIONSINGLETON_H
#define REVISIONSINGLETON_H

#include <QObject>
#include <QQmlEngine>
#include <QJSEngine>
#include "revision.h"

namespace qtquickvcp {

class RevisionSingleton : public QObject
{
    Q_OBJECT
    Q_DISABLE_COPY(RevisionSingleton)
    Q_PROPERTY(QString name READ name CONSTANT)
    Q_PROPERTY(QString runtimeQtVersion READ runtimeQtVersion CONSTANT)
    Q_PROPERTY(QString compiledQtVersion READ compiledQtVersion CONSTANT)

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
        return QStringLiteral(REVISION);
    }

    QString runtimeQtVersion() const
    {
        return qVersion();
    }

    QString compiledQtVersion() const
    {
        return QT_VERSION_STR;
    }

}; // class RevisionSingleton
} // namespace qtquickvcp

#endif // REVISIONSINGLETON_H
