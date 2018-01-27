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
    QObject* recurseDescriptor(const google::protobuf::Descriptor *descriptor, QObject *parent,
                               const QString &fieldFilter = QString());
    int recurseMessage(const google::protobuf::Message &message, QObject *object,
                       const QString &tempDir = QStringLiteral("json"), const QString &fieldFilter = QString());

    void updateSimpleRepeatedField(QObject *object, const google::protobuf::Message &message, const google::protobuf::FieldDescriptor *field);
    void updateComplexRepeatedField(QObject *object, const google::protobuf::Message &message, const google::protobuf::FieldDescriptor *field,
                                    const QString &tempDir);
    QVariant simpleFieldValueToVariant(const google::protobuf::Message &message, const google::protobuf::FieldDescriptor *field);

    /** Converts a protobuf File object to a json file descriptor
     *  stores the data to a temporary directory
     **/
    void fileToObject(const machinetalk::File &file, QObject *object, const QString &tempDir);

    QByteArray recurseDescriptor(const google::protobuf::Descriptor *descriptor, QByteArray &qmlCode);
};
} // namespace qtquickvcp

#endif // SERVICE_H
