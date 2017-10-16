#include "machinetalkservice.h"
#include <QDebug>
#include <QQmlComponent>

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

void MachinetalkService::recurseDescriptor(const gpb::Descriptor *descriptor, QJsonObject &object)
{
    for (int i = 0; i < descriptor->field_count(); ++i)
    {
        const gpb::FieldDescriptor *field = descriptor->field(i);
        const QString &name = QString::fromStdString(field->camelcase_name());
        QJsonValue jsonValue;

        switch (field->cpp_type())
        {
        case gpb::FieldDescriptor::CPPTYPE_BOOL:
            jsonValue = false;
            break;
        case gpb::FieldDescriptor::CPPTYPE_DOUBLE:
        case gpb::FieldDescriptor::CPPTYPE_FLOAT:
            jsonValue = 0.0;
            break;
        case gpb::FieldDescriptor::CPPTYPE_INT32:
        case gpb::FieldDescriptor::CPPTYPE_INT64:
        case gpb::FieldDescriptor::CPPTYPE_UINT32:
        case gpb::FieldDescriptor::CPPTYPE_UINT64:
            jsonValue = 0;
            break;
        case gpb::FieldDescriptor::CPPTYPE_STRING:
            jsonValue = "";
            break;
        case gpb::FieldDescriptor::CPPTYPE_ENUM:
            jsonValue = field->enum_type()->value(0)->number();
            break;
        case gpb::FieldDescriptor::CPPTYPE_MESSAGE:
            QJsonObject jsonObject;

            if (field->message_type() == machinetalk::Position::descriptor())  // add indexes to position messages
            {
                for (int i = 0; i < 9; ++i)
                {
                    jsonObject.insert(QString::number(i), QJsonValue(0.0));
                }
            }

            recurseDescriptor(field->message_type(), jsonObject);
            jsonValue = jsonObject;
            break;
        }

        if (field->is_repeated())
        {
            QJsonArray jsonArray;
            QJsonObject jsonObject = jsonValue.toObject();

            jsonObject.remove("index");
            if (jsonObject.count() == 1)
            {
                jsonValue = jsonObject.value(jsonObject.keys().at(0));
            }
            else
            {
                jsonValue = jsonObject;
            }
            jsonArray.append(jsonValue);
            object.insert(name, jsonArray);
        }
        else
        {
            object.insert(name, jsonValue);
        }
    }
}

QByteArray MachinetalkService::recurseDescriptor(const google::protobuf::Descriptor *descriptor, QByteArray &qmlCode)
{
    QByteArray lastType;
    int count = 0;

    for (int i = 0; i < descriptor->field_count(); ++i)
    {
        const gpb::FieldDescriptor *field = descriptor->field(i);
        const auto &name = QByteArray::fromStdString(field->camelcase_name());
        QByteArray type;
        QByteArray value;

        if (name == "index") {
            continue;
        }

        switch (field->cpp_type())
        {
        case gpb::FieldDescriptor::CPPTYPE_BOOL:
            type = "bool";
            value = "false";
            break;
        case gpb::FieldDescriptor::CPPTYPE_DOUBLE:
        case gpb::FieldDescriptor::CPPTYPE_FLOAT:
            type = "double";
            value = "0.0";
            break;
        case gpb::FieldDescriptor::CPPTYPE_INT32:
        case gpb::FieldDescriptor::CPPTYPE_INT64:
        case gpb::FieldDescriptor::CPPTYPE_UINT32:
        case gpb::FieldDescriptor::CPPTYPE_UINT64:
            type = "int";
            value = "0";
            break;
        case gpb::FieldDescriptor::CPPTYPE_STRING:
            type = "string";
            value = "\"\"";
            break;
        case gpb::FieldDescriptor::CPPTYPE_ENUM:
            type = "int";
            value = QString::number(field->enum_type()->value(0)->number(), 10).toLocal8Bit();
            break;
        case gpb::FieldDescriptor::CPPTYPE_MESSAGE:
            value.append("QtObject {\n");
            const auto messageType = recurseDescriptor(field->message_type(), value);
            if (field->is_repeated() && (!messageType.isEmpty())) {
                type = messageType;
            }
            else {
                type = "QtObject";

                if (field->message_type() == machinetalk::Position::descriptor())  // add indexes to position messages
                {
                    value.append("property var data: [");
                    for (int i = 0; i < 9; ++i)
                    {
                        value.append("0.0,");
                    }
                    value.append("]\n");
                }
            }
            value.append("}");
            break;
        }

        if (field->is_repeated()) {
            qmlCode.append("property var " + name + ": ([])\n");
        }
        else {
            qmlCode.append("property " + type + " " + name + ": " + value + "\n");
        }

        lastType = type;
        count++;
    }

    return (count == 1) ? lastType : QByteArray();
}

QObject *MachinetalkService::recurseDescriptor(const google::protobuf::Descriptor *descriptor, QQmlEngine *engine, QObject *parent)
{
    QByteArray qmlCode;

    qmlCode.append("import QtQuick 2.0\n");
    qmlCode.append("QtObject {\n");
    recurseDescriptor(descriptor, qmlCode);
    qmlCode.append("}");

    QQmlComponent component(engine);
    component.setData(qmlCode, QUrl());

    auto childItem = component.create();
    if (childItem == nullptr) {
        qCritical() << "Failed to create QML component:" << component.errorString();
        return nullptr;
    }

    childItem->setParent(parent);

    return childItem;
}

int MachinetalkService::recurseMessage(const gpb::Message &message, QJsonObject &object, const QString &fieldFilter, const QString &tempDir)
{
    const bool filterEnabled = !fieldFilter.isEmpty();
    bool isPosition = false;
    const gpb::Reflection *reflection = message.GetReflection();
    gpb::vector< const gpb::FieldDescriptor * > output;
    reflection->ListFields(message, &output);

    if (message.GetDescriptor() == machinetalk::File::descriptor())  // handle files with binary data
    {
        machinetalk::File file;
        file.MergeFrom(message);
        fileToJson(file, object, tempDir);
        return static_cast<int>(output.size());
    }
    else if (message.GetDescriptor() == machinetalk::Position::descriptor()) // handle position vectors
    {
        isPosition = true;
    }

    for (const gpb::FieldDescriptor *field: output)
    {
        QJsonValue jsonValue;

        const auto &name = QString::fromStdString(field->camelcase_name());

        if (filterEnabled && (name != fieldFilter)) {
            continue;
        }

        if (!field->is_repeated())
        {
            switch (field->cpp_type())
            {
            case gpb::FieldDescriptor::CPPTYPE_BOOL:
                jsonValue = reflection->GetBool(message, field);
                break;
            case gpb::FieldDescriptor::CPPTYPE_DOUBLE:
                jsonValue = reflection->GetDouble(message, field);
                if (isPosition) {
                    object.insert(QString::number(field->index()), jsonValue);
                }
                break;
            case gpb::FieldDescriptor::CPPTYPE_FLOAT:
                jsonValue = static_cast<double>(reflection->GetFloat(message, field));
                break;
            case gpb::FieldDescriptor::CPPTYPE_INT32:
                jsonValue = static_cast<int>(reflection->GetInt32(message, field));
                break;
            case gpb::FieldDescriptor::CPPTYPE_INT64:
                jsonValue = static_cast<int>(reflection->GetInt64(message, field));
                break;
            case gpb::FieldDescriptor::CPPTYPE_UINT32:
                jsonValue = static_cast<int>(reflection->GetUInt32(message, field));
                break;
            case gpb::FieldDescriptor::CPPTYPE_UINT64:
                jsonValue = static_cast<int>(reflection->GetUInt64(message, field));
                break;
            case gpb::FieldDescriptor::CPPTYPE_STRING:
                jsonValue = QString::fromStdString(reflection->GetString(message, field));
                break;
            case gpb::FieldDescriptor::CPPTYPE_ENUM:
                jsonValue = reflection->GetEnum(message, field)->number();
                break;
            case gpb::FieldDescriptor::CPPTYPE_MESSAGE:
                QJsonObject jsonObject = object.value(name).toObject();
                recurseMessage(reflection->GetMessage(message, field), jsonObject, QString(), tempDir);
                jsonValue = jsonObject;
                break;
            }
            object.insert(name, jsonValue);
        }
        else if (field->cpp_type() == gpb::FieldDescriptor::CPPTYPE_MESSAGE)
        {
            QJsonArray jsonArray = object.value(name).toArray();
            QList<int> removeList; // store index of items to remove
            for (int i = 0; i < reflection->FieldSize(message, field); ++i)
            {
                QJsonObject jsonObject;
                QJsonValue jsonValue;
                const gpb::Message &subMessage = reflection->GetRepeatedMessage(message, field, i);
                const gpb::Descriptor *subDescriptor = subMessage.GetDescriptor();
                const gpb::FieldDescriptor *subField = subDescriptor->FindFieldByName("index");
                const gpb::Reflection *subReflection = subMessage.GetReflection();
                const int index = subReflection->GetInt32(subMessage, subField);

                while (jsonArray.size() < (index + 1))
                {
                    jsonArray.append(QJsonValue());
                }

                if (subDescriptor->field_count() != 2) // not only index and value field
                {
                    jsonObject = jsonArray.at(index).toObject(QJsonObject()); // use existing object values
                }

                if (recurseMessage(subMessage, jsonObject, QString(), tempDir) > 1)
                {
                    jsonObject.remove("index");

                    if (subDescriptor->field_count() != 2)
                    {
                        jsonValue = jsonObject; // use JSON object
                    }
                    else // index and value field
                    {
                        jsonValue = jsonObject.value(jsonObject.keys().at(0)); // use JSON value directly
                    }

                    jsonArray.replace(index, jsonValue);
                }
                else  // only index -> remove object
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
                    jsonArray.removeAt(removeList[k]);
                }
            }

            object.insert(name, QJsonValue(jsonArray));
        }
    }

    return static_cast<int>(output.size());
}

int MachinetalkService::recurseMessage(const google::protobuf::Message &message, QQmlEngine *engine, QObject *object, const QString &tempDir, const QString &fieldFilter)
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
                    auto data = qvariant_cast<QVariant>(object->property("data")).toList();
                    data[field->index()] = value;
                    object->setProperty("data", data); // FIXME: this call might cause multiple updates if position vector is changed
                }
            }
            else {
                QObject *memberObject = qvariant_cast<QObject *>(object->property(name));
                Q_ASSERT(memberObject != nullptr);
                recurseMessage(reflection->GetMessage(message, field), engine, memberObject, tempDir);
            }

        }
        else if (field->cpp_type() == gpb::FieldDescriptor::CPPTYPE_MESSAGE)
        {
            if (field->message_type()->field_count() != 2) // not only index and value field
            {
                updateComplexRepeatedField(object, message, field, engine, tempDir);
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

void MachinetalkService::updateComplexRepeatedField(QObject *object, const google::protobuf::Message &message, const google::protobuf::FieldDescriptor *field, QQmlEngine *engine, const QString &tempDir)
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
            QObject *newObject = recurseDescriptor(subDescriptor, engine, object);
            list.append(QVariant::fromValue(newObject));
            lengthChanged = true;
        }

        QObject *item = qvariant_cast<QObject*>(list.at(index));
        Q_ASSERT(item != nullptr);
        if (recurseMessage(subMessage, engine, item, tempDir) <= 1) // only index -> remove object
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

void MachinetalkService::updateValue(const gpb::Message &message, QJsonValue &value, const QString &field, const QString &tempDir)
{
    QJsonObject object;
    object.insert(field, value);
    recurseMessage(message, object, field, tempDir);
    value = object.value(field);
}

void MachinetalkService::fileToJson(const machinetalk::File &file, QJsonObject &object, const QString &tempDir)
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

    object.insert("url", QUrl::fromLocalFile(filePath).toString());
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

    object->setProperty("url", QUrl::fromLocalFile(filePath).toString());
}

} // namespace qtquickvcp
