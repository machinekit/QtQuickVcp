#ifndef QPIN_H
#define QPIN_H

#include <QQuickItem>
/*#include "types.pb.h"
#include "emcclass.pb.h"
#include "value.pb.hh"
#include "motcmds.pb.hh"
#include "canon.pb.hh"
#include "halrcomp.pb.hh"
#include "halupdate.pb.hh"
#include "log.pb.hh"
#include "object.pb.hh"
#include "rtapi_message.pb.hh"
#include "rtapicommand.pb.hh"
#include "rtmessage.pb.hh"
#include "task.pb.hh"
#include "test.pb.hh"*/
#include "message.pb.h"

class QPin : public QQuickItem
{
    Q_OBJECT
    Q_PROPERTY(QString name READ name WRITE setName NOTIFY nameChanged)
    Q_PROPERTY(ValueType type READ type WRITE setType NOTIFY typeChanged)
    Q_PROPERTY(HalPinDirection direction READ direction WRITE setDirection NOTIFY directionChanged)
    Q_PROPERTY(QVariant value READ value WRITE setValue NOTIFY valueChanged)
    Q_PROPERTY(int handle READ handle WRITE setHandle NOTIFY handleChanged)
    Q_ENUMS(ValueType)
    Q_ENUMS(HalPinDirection)

public:
    explicit QPin(QQuickItem *parent = 0);

    enum ValueType {
        HAL_BIT = pb::HAL_BIT,
        HAL_FLOAT = pb::HAL_FLOAT,
        HAL_S32 = pb::HAL_S32,
        HAL_U32 = pb::HAL_U32,
        HAL_WT = pb::HAL_IO
    };

    enum HalPinDirection {
      HAL_IN = pb::HAL_IN,
      HAL_OUT = pb::HAL_OUT,
      HAL_IO = pb::HAL_IO
    };

    QString name() const
    {
        return m_name;
    }

    ValueType type() const
    {
        return m_type;
    }

    HalPinDirection direction() const
    {
        return m_direction;
    }

    QVariant value() const
    {
        return m_value;
    }

    int handle() const
    {
        return m_handle;
    }

signals:

    void nameChanged(QString arg);

    void typeChanged(ValueType arg);

    void directionChanged(HalPinDirection arg);

    void valueChanged(QVariant arg);

    void handleChanged(int arg);

public slots:

void setType(ValueType arg)
{
    if (m_type != arg) {
        m_type = arg;
        emit typeChanged(arg);
    }
}
void setName(QString arg)
{
    if (m_name != arg) {
        m_name = arg;
        emit nameChanged(arg);
    }
}
void setDirection(HalPinDirection arg)
{
    if (m_direction != arg) {
        m_direction = arg;
        emit directionChanged(arg);
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

private:
    QString m_name;
    ValueType m_type;
    HalPinDirection m_direction;
    QVariant m_value;

    int m_handle;
};



#endif // QPIN_H
