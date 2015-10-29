#ifndef SERVICE_H
#define SERVICE_H

#include <QObject>
#include <QJsonObject>
#include <QJsonArray>
#include <QStringList>
#include <QDir>
#include <QCoreApplication>
#include <QUuid>
#include <QUrl>
#include <google/protobuf/text_format.h>
#include <google/protobuf/message.h>
#include <google/protobuf/descriptor.h>
#include "config.pb.h"
#include "preview.pb.h"

#if defined(Q_OS_IOS)
namespace gpb = google_public::protobuf;
#else
namespace gpb = google::protobuf;
#endif

class Service : public QObject
{
    Q_OBJECT
    Q_ENUMS(SocketState)
    Q_ENUMS(State)
    Q_ENUMS(ConnectionError)

public:
    explicit Service(QObject *parent = 0);

    enum SocketState {
        Down = 1,
        Trying = 2,
        Up = 3
    };

    enum State {
        Disconnected = 0,
        Connecting = 1,
        Connected = 2,
        Timeout = 3,
        Error = 4
    };

    enum ConnectionError {
        NoError = 0,
        BindError = 1,
        PinChangeError = 2,
        CommandError = 3,
        SocketError = 4,
        ServiceError = 5
    };

    static QString applicationTempPath(const QString &name);
    static bool removeTempPath(const QString &name);
    static QString enumNameToCamelCase(const QString &name);
    static void recurseDescriptor(const gpb::Descriptor *descriptor,
                                  QJsonObject *object);
    static void recurseMessage(const gpb::Message &message,
                               QJsonObject *object,
                               const QString &fieldFilter = QString(),
                               const QString &tempDir = QString("json"));
    static void updateValue(const gpb::Message &message,
                            QJsonValue *value,
                            const QString &field,
                            const QString &tempDir = QString("json"));
    static void fileToJson(const pb::File &file,
                           QJsonObject *object,
                           const QString tempDir);
};

#endif // SERVICE_H
