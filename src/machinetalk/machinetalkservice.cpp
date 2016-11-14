#include "machinetalkservice.h"
#include <QDebug>

#if defined(Q_OS_IOS)
namespace gpb = google_public::protobuf;
#else
namespace gpb = google::protobuf;
#endif

namespace qtquickvcp {

MachinetalkService::MachinetalkService(QObject *parent) : QObject(parent)
{

}

QString MachinetalkService::applicationTempPath(const QString &name)
{
    return QString("%1/machinekit-%2/%3/").arg(QDir::tempPath())
            .arg(QCoreApplication::applicationPid())
            .arg(name);
}

bool MachinetalkService::removeTempPath(const QString &name)
{
    QDir dir(applicationTempPath(name));
    return dir.removeRecursively();
}

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

void MachinetalkService::recurseDescriptor(const gpb::Descriptor *descriptor, QJsonObject *object)
{
    for (int i = 0; i < descriptor->field_count(); ++i)
    {
        const gpb::FieldDescriptor *field;
        QString name;
        QJsonValue jsonValue;

        field = descriptor->field(i);
        name = QString::fromStdString(field->camelcase_name());
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
            //jsonValue = enumNameToCamelCase(QString::fromStdString(field->enum_type()->value(0)->name()));
            break;
        case gpb::FieldDescriptor::CPPTYPE_MESSAGE:
            QJsonObject jsonObject;

            if (field->message_type() == pb::Position::descriptor())  // add indexes to position messages
            {
                for (int i = 0; i < 9; ++i)
                {
                    jsonObject.insert(QString::number(i), QJsonValue(0.0));
                }
            }

            recurseDescriptor(field->message_type(), &jsonObject);
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
            object->insert(name, jsonArray);
        }
        else
        {
            object->insert(name, jsonValue);
        }
    }
}

int MachinetalkService::recurseMessage(const gpb::Message &message, QJsonObject *object, const QString &fieldFilter, const QString &tempDir)
{
    bool filterEnabled = !fieldFilter.isEmpty();
    bool isPosition = false;
    const gpb::Reflection *reflection = message.GetReflection();
    gpb::vector< const gpb::FieldDescriptor * > output;
    reflection->ListFields(message, &output);

    if (message.GetDescriptor() == pb::File::descriptor())  // handle files with binary data
    {
        pb::File file;
        file.MergeFrom(message);
        fileToJson(file, object, tempDir);
        return output.size();
    }
    else if (message.GetDescriptor() == pb::Position::descriptor()) // handle position vecotors
    {
        isPosition = true;
    }

    for (int i = 0; i < (int)output.size(); ++i)
    {
        QString name;
        QJsonValue jsonValue;
        const gpb::FieldDescriptor *field;

        field = output[i];
        name = QString::fromStdString(field->camelcase_name());

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
                    object->insert(QString::number(field->index()), jsonValue);
                }
                break;
            case gpb::FieldDescriptor::CPPTYPE_FLOAT:
                jsonValue = (double)reflection->GetFloat(message, field);
                break;
            case gpb::FieldDescriptor::CPPTYPE_INT32:
                jsonValue = (int)reflection->GetInt32(message, field);
                break;
            case gpb::FieldDescriptor::CPPTYPE_INT64:
                jsonValue = (int)reflection->GetInt64(message, field);
                break;
            case gpb::FieldDescriptor::CPPTYPE_UINT32:
                jsonValue = (int)reflection->GetUInt32(message, field);
                break;
            case gpb::FieldDescriptor::CPPTYPE_UINT64:
                jsonValue = (int)reflection->GetUInt64(message, field);
                break;
            case gpb::FieldDescriptor::CPPTYPE_STRING:
                jsonValue = QString::fromStdString(reflection->GetString(message, field));
                break;
            case gpb::FieldDescriptor::CPPTYPE_ENUM:
                jsonValue = reflection->GetEnum(message, field)->number();
                //jsonValue = enumNameToCamelCase(QString::fromStdString(reflection->GetEnum(message, field)->name()));
                break;
            case gpb::FieldDescriptor::CPPTYPE_MESSAGE:
                QJsonObject jsonObject = object->value(name).toObject();
                recurseMessage(reflection->GetMessage(message, field), &jsonObject, QString(), tempDir);
                jsonValue = jsonObject;
                break;
            }
            object->insert(name, jsonValue);
        }
        else
        {
            if (field->cpp_type() == gpb::FieldDescriptor::CPPTYPE_MESSAGE)
            {
                QJsonArray jsonArray = object->value(name).toArray();
                QList<int> removeList; // store index of items to remove
                for (int j = 0; j < reflection->FieldSize(message, field); ++j)
                {
                    QJsonObject jsonObject;
                    QJsonValue jsonValue;
                    const gpb::Message &subMessage = reflection->GetRepeatedMessage(message, field, j);
                    const gpb::Descriptor *subDescriptor = subMessage.GetDescriptor();
                    const gpb::FieldDescriptor *subField = subDescriptor->FindFieldByName("index");
                    const gpb::Reflection *subReflection = subMessage.GetReflection();
                    int index = subReflection->GetInt32(subMessage, subField);

                    while (jsonArray.size() < (index + 1))
                    {
                        jsonArray.append(QJsonValue());
                    }

                    if (subDescriptor->field_count() != 2) // not only index and value field
                    {
                        jsonObject = jsonArray.at(index).toObject(QJsonObject()); // use existing object values
                    }

                    if (recurseMessage(subMessage, &jsonObject, QString(), tempDir) > 1)
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
                    qSort(removeList.begin(), removeList.end());
                    for (int k = (removeList.length() - 1); k >= 0; k--)
                    {
                        jsonArray.removeAt(removeList[k]);
                    }
                }

                object->insert(name, QJsonValue(jsonArray));
            }
        }
    }

    return output.size();
}

void MachinetalkService::updateValue(const gpb::Message &message, QJsonValue *value, const QString &field, const QString &tempDir)
{
    QJsonObject object;
    object.insert(field, *value);
    recurseMessage(message, &object, field, tempDir);
    *value = object.value(field);
}

/** Converts a protobuf File object to a json file descriptor
 *  stores the data to a temporary directory
 **/
void MachinetalkService::fileToJson(const pb::File &file, QJsonObject *object, const QString tempDir)
{
    QDir dir;
    QString fileName;
    QString tmpPath;
    QString filePath;
    QString uuid;
    QByteArray data;

    if (!(file.has_name() && file.has_blob() && file.has_encoding())) {
        return;
    }

    fileName = QString::fromStdString(file.name());
    tmpPath = applicationTempPath(tempDir);
    uuid = QUuid::createUuid().toString();
    uuid = uuid.replace('{', '_').replace('}', '_');
    filePath = tmpPath + uuid + fileName;
    QFile localFile(filePath);

    if (!dir.mkpath(tmpPath))
    {
        qWarning() << "not able to create directory";
        return;
    }

    if (!localFile.open(QIODevice::WriteOnly))
    {
        qWarning() << "not able to create file" << filePath;
        return;
    }

    data = QByteArray(file.blob().data(), file.blob().size());

    if (file.encoding() == pb::ZLIB)
    {
        quint32 test = ((quint32)data.at(0) << 24) + ((quint32)data.at(1) << 16) + ((quint32)data.at(2) << 8) + ((quint32)data.at(3) << 0);
        qWarning() << test << (quint8)data.at(0) << (quint8)data.at(1) << (quint8)data.at(2) << (quint8)data.at(3);   // TODO
        data = qUncompress(data);
    }
    else if (file.encoding() != pb::CLEARTEXT)
    {
        qWarning() << "unknown encoding";
        localFile.close();
        return;
    }

    localFile.write(data);
    localFile.close();

    object->insert("url", QUrl::fromLocalFile(filePath).toString());
}
}; // namespace qtquickvcp
