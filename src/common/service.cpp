#include "service.h"

Service::Service(QObject *parent) : QObject(parent)
{

}

QString Service::enumNameToCamelCase(const QString &name)
{
    QStringList partList;

    partList = name.toLower().split('_');

    for (int i = 0; i < partList.size(); ++i)
    {
        partList[i][0] = partList[i][0].toUpper();
    }

    return partList.join("");
}

void Service::recurseDescriptor(const gpb::Descriptor *descriptor, QJsonObject *object)
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


void Service::recurseMessage(const gpb::Message &message, QJsonObject *object, const QString &fieldFilter)
{
    bool filterEnabled = !fieldFilter.isEmpty();
    const gpb::Reflection *reflection = message.GetReflection();
    gpb::vector< const gpb::FieldDescriptor * > output;
    reflection->ListFields(message, &output);

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
                recurseMessage(reflection->GetMessage(message, field), &jsonObject);
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

                    if (subDescriptor->field_count() == 2)  // index and value field
                    {
                        recurseMessage(subMessage, &jsonObject);
                        jsonObject.remove("index");
                        jsonValue = jsonObject.value(jsonObject.keys().at(0));
                    }
                    else
                    {
                        jsonObject = jsonArray.at(index).toObject(QJsonObject());
                        recurseMessage(subMessage, &jsonObject);
                        jsonObject.remove("index");
                        jsonValue = jsonObject;
                    }

                    jsonArray.replace(index, jsonValue);
                }
                object->insert(name, QJsonValue(jsonArray));
            }
        }
    }
}

void Service::updateValue(const google::protobuf::Message &message, QJsonValue *value, const QString &field)
{
    QJsonObject object;
    object.insert(field, *value);
    recurseMessage(message, &object, field);
    *value = object.value(field);
}

