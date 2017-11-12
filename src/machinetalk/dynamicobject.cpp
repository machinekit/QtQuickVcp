#include "dynamicobject.h"
#include <private/qmetaobjectbuilder_p.h>
#include <cstring>
#include <QtDebug>
#include <QLoggingCategory>

Q_LOGGING_CATEGORY(loggingCategory, "DynamicObject");

DynamicObject::DynamicObject(QObject *parent)
    : QObject(parent)
    , m_metaObject(nullptr)
{
}

DynamicObject::~DynamicObject()
{
    free(m_metaObject);
}

void DynamicObject::addProperty(const QByteArray &name, const QByteArray &type, const QVariant &value)
{
    if (m_metaObject != nullptr) {
        qCWarning(loggingCategory) << "Can't add property after calling ready()";
        return;
    }

    m_properties.emplace_back(DynamicProperty{name, type, QMetaType::type(type), value});
}

void DynamicObject::ready()
{
    if (m_metaObject != nullptr) {
        qCWarning(loggingCategory) << "ready() should be called only once.";
        return;
    }

    QMetaObjectBuilder builder;
    builder.setClassName("DynamicObject");  // TODO: find out if it is legit to have single class name
    builder.setSuperClass(&QObject::staticMetaObject);

    for (const auto &dynamicProperty: m_properties) {
        auto property = builder.addProperty(dynamicProperty.name, dynamicProperty.type);
        property.setWritable(true);
        auto signal = builder.addSignal(dynamicProperty.name + "Changed()");
        property.setNotifySignal(signal);
    }

    m_metaObject = builder.toMetaObject();
}

const QMetaObject* DynamicObject::metaObject() const {
    return m_metaObject;
}

int DynamicObject::qt_metacall(QMetaObject::Call call, int id, void **argv)
{
    const int realId = id - m_metaObject->propertyOffset();
    if (realId < 0) {
        return QObject::qt_metacall(call, id, argv);
    }

    if (call == QMetaObject::ReadProperty) {
        const auto &property = m_properties.at(static_cast<size_t>(realId));
        QMetaType::construct(property.typeId, argv[0], property.variant.data());
    }
    else if (call == QMetaObject::WriteProperty) {
        auto &property = m_properties.at(static_cast<size_t>(realId));
        property.variant = QVariant(property.typeId, argv[0]);
        *reinterpret_cast<int*>(argv[2]) = 1;  // setProperty return value
        QMetaObject::activate(this, m_metaObject, realId, nullptr);
    }
    else {
        // not handled
    }

    return -1;
}

void* DynamicObject::qt_metacast(const char *name)
{
    if (strcmp(name, m_metaObject->className()) == 0) {
        return this;
    }
    else {
        return QObject::qt_metacast(name);
    }
}
