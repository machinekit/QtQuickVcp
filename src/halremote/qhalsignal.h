#ifndef QHALSIGNAL_H
#define QHALSIGNAL_H

#include <QObject>
#include <QVariant>
#include "message.pb.h"

class QHalSignal : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString name READ name WRITE setName NOTIFY nameChanged)
    Q_PROPERTY(ValueType type READ type NOTIFY typeChanged)
    Q_PROPERTY(QVariant value READ value NOTIFY valueChanged)
    Q_PROPERTY(int handle READ handle NOTIFY handleChanged)
    Q_PROPERTY(bool enabled READ enabled WRITE setEnabled NOTIFY enabledChanged)
    Q_ENUMS(ValueType)

public:
    explicit QHalSignal(QObject *parent = 0);

    enum ValueType {
        Bit = pb::HAL_BIT,
        Float = pb::HAL_FLOAT,
        S32 = pb::HAL_S32,
        U32 = pb::HAL_U32
    };

    QString name() const
    {
        return m_name;
    }

    ValueType type() const
    {
        return m_type;
    }

    QVariant value() const
    {
        return m_value;
    }

    int handle() const
    {
        return m_handle;
    }

    bool enabled() const
    {
        return m_enabled;
    }

public slots:
    void setName(QString arg)
    {
        if (m_name != arg) {
            m_name = arg;
            emit nameChanged(arg);
        }
    }

    void setType(ValueType arg)
    {
        if (m_type != arg) {
            m_type = arg;
            emit typeChanged(arg);
        }
    }

    void setValue(QVariant arg)
    {
        if (m_value != arg) {
            m_value = arg;
            emit valueChanged(arg);
        }
    }

    void setHandle(int arg)
    {
        if (m_handle != arg) {
            m_handle = arg;
            emit handleChanged(arg);
        }
    }

    void setEnabled(bool arg)
    {
        if (m_enabled != arg) {
            m_enabled = arg;
            emit enabledChanged(arg);
        }
    }

signals:
    void nameChanged(QString arg);
    void typeChanged(ValueType arg);
    void valueChanged(QVariant arg);
    void handleChanged(int arg);
    void enabledChanged(bool arg);

private:
    QString m_name;
    ValueType m_type;
    QVariant m_value;
    int m_handle;
    bool m_enabled;
};

#endif // QHALSIGNAL_H
