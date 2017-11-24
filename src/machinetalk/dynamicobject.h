#ifndef DYNAMICOBJECT_H
#define DYNAMICOBJECT_H

#include <QObject>
#include <QVariant>

class DynamicObject : public QObject
{
    Q_OBJECT
public:
    explicit DynamicObject(QObject *parent = nullptr);

    ~DynamicObject();

    /** Adds a new property to the object, must be called before ready(). */
    void addProperty(const QByteArray &name, const QByteArray &type, const QVariant &value);

    /** Marks the object as ready and creates the metaObject. */
    void ready();

signals:

public slots:

private:
    struct DynamicProperty {
        QByteArray name;
        QByteArray type;
        int typeId;
        QVariant variant;
    };

    QMetaObject *m_metaObject;
    std::vector<DynamicProperty> m_properties;
};

#endif // DYNAMICOBJECT_H
