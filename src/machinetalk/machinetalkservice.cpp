#include "machinetalkservice.h"
#include <QDebug>
#include <QQmlComponent>
#include <dynamicobject.h>

#if defined(Q_OS_IOS)
namespace gpb = google_public::protobuf;
#else
namespace gpb = google::protobuf;
#endif

namespace qtquickvcp {

QString MachinetalkService::enumNameToCamelCase(const QString &name)
{
    QStringList partList;

    partList = name.toLower().split('_');

    for (int i = 0; i < partList.size(); ++i)
    {
        partList[i][0] = partList[i][0].toUpper();
    }

    return partList.join("");
}

QObject *MachinetalkService::recurseDescriptor(const google::protobuf::Descriptor *descriptor, QObject *parent, const QString &fieldFilter)
{
    int count = 0;
    const bool filterEnabled = !fieldFilter.isEmpty();
    auto object = new DynamicObject(parent);

    if (descriptor == machinetalk::Position::descriptor())  // add indexes to position messages
    {
        for (int i = 0; i < 9; ++i)
        {
            object->addProperty(QString::number(i).toLocal8Bit(), "double", QVariant::fromValue(0.0));
            count++;
        }
    }
    else if (descriptor == machinetalk::File::descriptor())
    {
        object->addProperty("url", "QString", QVariant::fromValue(QString("")));
        count++;
    }

    for (int i = 0; i < descriptor->field_count(); ++i)
    {
        const gpb::FieldDescriptor *field = descriptor->field(i);
        const auto &name = QByteArray::fromStdString(field->camelcase_name());
        QByteArray type;
        QVariant value;

        if ((name == "index") || (filterEnabled && (name != fieldFilter))) {
            continue;
        }

        if (field->is_repeated()) {
            object->addProperty(name, "QVariant", QVariant::fromValue(QList<QVariant>()));
            count++;
            continue;
        }

        switch (field->cpp_type())
        {
        case gpb::FieldDescriptor::CPPTYPE_BOOL:
            type = "bool";
            value = QVariant::fromValue(false);
            break;
        case gpb::FieldDescriptor::CPPTYPE_DOUBLE:
        case gpb::FieldDescriptor::CPPTYPE_FLOAT:
            type = "double";
            value = QVariant::fromValue(0.0);
            break;
        case gpb::FieldDescriptor::CPPTYPE_INT32:
        case gpb::FieldDescriptor::CPPTYPE_INT64:
        case gpb::FieldDescriptor::CPPTYPE_UINT32:
        case gpb::FieldDescriptor::CPPTYPE_UINT64:
            type = "int";
            value = QVariant::fromValue(0);
            break;
        case gpb::FieldDescriptor::CPPTYPE_STRING:
            type = "QString";
            value = QVariant::fromValue(QString(""));
            break;
        case gpb::FieldDescriptor::CPPTYPE_ENUM:
            type = "int";
            value = QVariant::fromValue(QString::number(field->enum_type()->value(0)->number(), 10).toLocal8Bit());
            break;
        case gpb::FieldDescriptor::CPPTYPE_MESSAGE:
            QObject* subObject = recurseDescriptor(field->message_type(), parent);
            type = "QObject*";
            value = QVariant::fromValue(subObject);
            break;
        }

        object->addProperty(name, type, value);
        count++;
    }

    if ((count == 1) && !filterEnabled) {
        object->deleteLater();
        return nullptr; // object had only the index field
    }
    else {
        object->ready();
        return object;
    }
}

int MachinetalkService::recurseMessage(const google::protobuf::Message &message, QObject *object, const QString &tempDir, const QString &fieldFilter)
{
    Q_ASSERT(object != nullptr);

    const bool filterEnabled = !fieldFilter.isEmpty();
    bool isPosition = false;
    const gpb::Reflection *reflection = message.GetReflection();
    gpb::vector< const gpb::FieldDescriptor * > output;
    reflection->ListFields(message, &output);

    if (message.GetDescriptor() == machinetalk::File::descriptor())  // handle files with binary data
    {
        machinetalk::File file;
        file.MergeFrom(message);
        fileToObject(file, object, tempDir);
        return static_cast<int>(output.size());
    }
    else if (message.GetDescriptor() == machinetalk::Position::descriptor()) // handle position vectors
    {
        isPosition = true;
    }

    for (const gpb::FieldDescriptor *field: output)
    {
        const auto &name = QByteArray::fromStdString(field->camelcase_name());

        if ((name == "index") || (filterEnabled && (name != fieldFilter))) {
            continue;
        }

        if (!field->is_repeated())
        {
            if (field->cpp_type() != gpb::FieldDescriptor::CPPTYPE_MESSAGE)
            {
                const auto &value = simpleFieldValueToVariant(message, field);
                object->setProperty(name, value);
                if (isPosition) {
                    object->setProperty(QString::number(field->index()).toLocal8Bit(), value);
                }
            }
            else {
                QObject *memberObject = qvariant_cast<QObject *>(object->property(name));
                Q_ASSERT(memberObject != nullptr);
                recurseMessage(reflection->GetMessage(message, field), memberObject, tempDir);
            }

        }
        else if (field->cpp_type() == gpb::FieldDescriptor::CPPTYPE_MESSAGE)
        {
            if (field->message_type()->field_count() != 2) // not only index and value field
            {
                updateComplexRepeatedField(object, message, field, tempDir);
            }
            else {
                updateSimpleRepeatedField(object, message, field);
            }
        }
    }

    return static_cast<int>(output.size());
}

void MachinetalkService::updateSimpleRepeatedField(QObject *object, const gpb::Message &message, const gpb::FieldDescriptor *field)
{
    const auto &name = QByteArray::fromStdString(field->camelcase_name());
    const gpb::Reflection *reflection = message.GetReflection();
    auto list = qvariant_cast<QVariant>(object->property(name)).toList();

    QList<int> removeList; // store index of items to remove
    for (int i = 0; i < reflection->FieldSize(message, field); ++i)
    {
        const gpb::Message &subMessage = reflection->GetRepeatedMessage(message, field, i);
        const gpb::Descriptor *subDescriptor = subMessage.GetDescriptor();
        const gpb::FieldDescriptor *subField = subDescriptor->FindFieldByName("index");
        const gpb::Reflection *subReflection = subMessage.GetReflection();
        const int index = subReflection->GetInt32(subMessage, subField);

        while (list.size() < (index + 1))
        {
            list.append(QVariant());
        }

        gpb::vector< const gpb::FieldDescriptor * > output;
        subReflection->ListFields(subMessage, &output);
        if (output.size() > 1) // index and value field
        {
            for (const auto subField: output) {
                if (subField->name() == "index") {
                    continue;
                }
                list[index] = simpleFieldValueToVariant(subMessage, subField);
            }
        }
        else // only index -> remove object
        {
            removeList.append(index);
        }
    }

    // remove marked items
    if (removeList.length() > 0)
    {
        std::sort(removeList.begin(), removeList.end());
        for (int k = (removeList.length() - 1); k >= 0; k--)
        {
            list.removeAt(removeList[k]);
        }
    }

    object->setProperty(name, QVariant::fromValue(list));
}

void MachinetalkService::updateComplexRepeatedField(QObject *object, const google::protobuf::Message &message, const google::protobuf::FieldDescriptor *field, const QString &tempDir)
{
    const auto &name = QByteArray::fromStdString(field->camelcase_name());
    const gpb::Reflection *reflection = message.GetReflection();
    auto list = qvariant_cast<QVariant>(object->property(name)).toList();

    QList<int> removeList; // store index of items to remove
    bool lengthChanged = false;
    for (int i = 0; i < reflection->FieldSize(message, field); ++i)
    {
        const gpb::Message &subMessage = reflection->GetRepeatedMessage(message, field, i);
        const gpb::Descriptor *subDescriptor = subMessage.GetDescriptor();
        const gpb::FieldDescriptor *subField = subDescriptor->FindFieldByName("index");
        const gpb::Reflection *subReflection = subMessage.GetReflection();
        const int index = subReflection->GetInt32(subMessage, subField);

        while (list.size() < (index + 1))
        {
            QObject *newObject = recurseDescriptor(subDescriptor, object);
            list.append(QVariant::fromValue(newObject));
            lengthChanged = true;
        }

        QObject *item = qvariant_cast<QObject*>(list.at(index));
        Q_ASSERT(item != nullptr);
        if (recurseMessage(subMessage, item, tempDir) <= 1) // only index -> remove object
        {
            removeList.append(index);
        }
    }

    // remove marked items
    if (removeList.length() > 0)
    {
        std::sort(removeList.begin(), removeList.end());
        for (int k = (removeList.length() - 1); k >= 0; k--)
        {
            QObject *item = qvariant_cast<QObject*>(list.takeAt(removeList[k]));
            Q_ASSERT(item != nullptr);
            item->deleteLater();
        }
        lengthChanged = true;
    }

    if (lengthChanged) // we need to notify property bindings about changes in length
    {
        object->setProperty(name, QVariant::fromValue(list));
    }
}

QVariant MachinetalkService::simpleFieldValueToVariant(const gpb::Message &message, const gpb::FieldDescriptor *field)
{
    const gpb::Reflection *reflection = message.GetReflection();

    QVariant value;
    switch (field->cpp_type())
    {
    case gpb::FieldDescriptor::CPPTYPE_BOOL:
        value = reflection->GetBool(message, field);
        break;
    case gpb::FieldDescriptor::CPPTYPE_DOUBLE:
        value = reflection->GetDouble(message, field);
        break;
    case gpb::FieldDescriptor::CPPTYPE_FLOAT:
        value = static_cast<double>(reflection->GetFloat(message, field));
        break;
    case gpb::FieldDescriptor::CPPTYPE_INT32:
        value = static_cast<int>(reflection->GetInt32(message, field));
        break;
    case gpb::FieldDescriptor::CPPTYPE_INT64:
        value = static_cast<int>(reflection->GetInt64(message, field));
        break;
    case gpb::FieldDescriptor::CPPTYPE_UINT32:
        value = static_cast<int>(reflection->GetUInt32(message, field));
        break;
    case gpb::FieldDescriptor::CPPTYPE_UINT64:
        value = static_cast<int>(reflection->GetUInt64(message, field));
        break;
    case gpb::FieldDescriptor::CPPTYPE_STRING:
        value = QString::fromStdString(reflection->GetString(message, field));
        break;
    case gpb::FieldDescriptor::CPPTYPE_ENUM:
        value = reflection->GetEnum(message, field)->number();
        break;
    case gpb::FieldDescriptor::CPPTYPE_MESSAGE:
        qCritical() << "called simple convert function with complex type";
        break;
    }

    return value;
}

void MachinetalkService::fileToObject(const machinetalk::File &file, QObject *object, const QString &tempDir)
{
    if (!(file.has_name() && file.has_blob() && file.has_encoding())) {
        return;
    }

    const QString &fileName = QString::fromStdString(file.name());
    QString uuid = QUuid::createUuid().toString();
    uuid = uuid.replace('{', '_').replace('}', '_');
    const QString &filePath = QDir(tempDir).filePath(uuid + fileName);
    QFile localFile(filePath);

    if (!localFile.open(QIODevice::WriteOnly))
    {
        qWarning() << "not able to create file" << filePath;
        return;
    }

    const auto &data = QByteArray::fromRawData(file.blob().data(), static_cast<int>(file.blob().size()));

    if (file.encoding() == machinetalk::ZLIB)
    {
        qWarning() << "zlib encoding may not work";
        const QByteArray uncompressedData = qUncompress(data); // TODO: zlib uncompress not working correctly
        localFile.write(uncompressedData);
        localFile.close();
    }
    else if (file.encoding() == machinetalk::CLEARTEXT) {

        localFile.write(data);
        localFile.close();
    }
    else
    {
        qWarning() << "unknown file encoding";
        localFile.close();
        return;
    }

    object->setProperty("name", QVariant::fromValue(fileName));
    object->setProperty("url", QVariant::fromValue(QUrl::fromLocalFile(filePath).toString()));
}

} // namespace qtquickvcp
