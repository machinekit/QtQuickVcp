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
#include <machinetalk/protobuf/config.pb.h>
#include <machinetalk/protobuf/preview.pb.h>

namespace qtquickvcp {

class MachinetalkService : public QObject
{
    Q_OBJECT
    Q_ENUMS(SocketState)
    Q_ENUMS(State)
    Q_ENUMS(ConnectionError)

public:
    explicit MachinetalkService(QObject *parent = 0);

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
    static void recurseDescriptor(const google::protobuf::Descriptor *descriptor,
                                  QJsonObject *object);
    static int recurseMessage(const google::protobuf::Message &message,
                               QJsonObject *object,
                               const QString &fieldFilter = QString(),
                               const QString &tempDir = QString("json"));
    static void updateValue(const google::protobuf::Message &message,
                            QJsonValue *value,
                            const QString &field,
                            const QString &tempDir = QString("json"));
    static void fileToJson(const pb::File &file,
                           QJsonObject *object,
                           const QString tempDir);
};
}; // namespace qtquickvcp

#endif // SERVICE_H
