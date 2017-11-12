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
#include <QQmlEngine>
#include <google/protobuf/text_format.h>
#include <google/protobuf/message.h>
#include <google/protobuf/descriptor.h>
#include <machinetalk/protobuf/config.pb.h>
#include <machinetalk/protobuf/preview.pb.h>

namespace qtquickvcp {

namespace MachinetalkService
{
    QString enumNameToCamelCase(const QString &name);
    void recurseDescriptor(const google::protobuf::Descriptor *descriptor, QJsonObject &object);
    QObject* recurseDescriptor(const google::protobuf::Descriptor *descriptor, QQmlEngine *engine, QObject *parent);
    int recurseMessage(const google::protobuf::Message &message, QJsonObject &object,
                       const QString &fieldFilter = QString(), const QString &tempDir = QString("json"));
    int recurseMessage(const google::protobuf::Message &message, QQmlEngine *engine, QObject *object,
                       const QString &tempDir = QString("json"), const QString &fieldFilter = QString());
    QObject* recurseDescriptor(const google::protobuf::Descriptor *descriptor, QObject *parent,
                               const QString &fieldFilter = QString());
    int recurseMessage(const google::protobuf::Message &message, QObject *object,
                       const QString &tempDir = QString("json"), const QString &fieldFilter = QString());
    void updateValue(const google::protobuf::Message &message, QJsonValue &value,
                     const QString &field, const QString &tempDir = QString("json"));

    void updateSimpleRepeatedField(QObject *object, const google::protobuf::Message &message, const google::protobuf::FieldDescriptor *field);
    void updateComplexRepeatedField(QObject *object, const google::protobuf::Message &message, const google::protobuf::FieldDescriptor *field,
                                    QQmlEngine *engine, const QString &tempDir);
    void updateComplexRepeatedField(QObject *object, const google::protobuf::Message &message, const google::protobuf::FieldDescriptor *field,
                                    const QString &tempDir);
    QVariant simpleFieldValueToVariant(const google::protobuf::Message &message, const google::protobuf::FieldDescriptor *field);

    /** Converts a protobuf File object to a json file descriptor
     *  stores the data to a temporary directory
     **/
    void fileToJson(const machinetalk::File &file, QJsonObject &object, const QString &tempDir);
    void fileToObject(const machinetalk::File &file, QObject *object, const QString &tempDir);

    QByteArray recurseDescriptor(const google::protobuf::Descriptor *descriptor, QByteArray &qmlCode);
};
} // namespace qtquickvcp

#endif // SERVICE_H
