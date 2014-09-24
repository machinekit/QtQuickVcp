#include "qhalsignal.h"

QHalSignal::QHalSignal(QObject *parent) :
    QObject(parent),
    m_name("default"),
    m_type(Bit),
    m_value(false),
    m_handle(0),
    m_enabled(true),
    m_synced(false)
{
}

void QHalSignal::setName(QString arg)
{
    if (m_name != arg) {
        m_name = arg;
        emit nameChanged(arg);
    }
}

void QHalSignal::setType(QHalSignal::ValueType arg)
{
    if (m_type != arg) {
        m_type = arg;
        emit typeChanged(arg);

        switch (m_type) {
        case Bit:
            m_value = m_value.toBool();
            break;
        case Float:
            m_value = m_value.toDouble();
            break;
        case S32:
            m_value = m_value.toInt();
            break;
        case U32:
            m_value = m_value.toUInt();
            break;
        }
        emit valueChanged(m_value);
    }
}

void QHalSignal::setValue(QVariant arg)
{
    if ((m_value != arg) || (m_value.type() != arg.type())) {
        m_value = arg;
        emit valueChanged(arg);
    }
}

void QHalSignal::setHandle(int arg)
{
    if (m_handle != arg) {
        m_handle = arg;
        emit handleChanged(arg);
    }
}

void QHalSignal::setEnabled(bool arg)
{
    if (m_enabled != arg) {
        m_enabled = arg;
        emit enabledChanged(arg);
    }
}

void QHalSignal::setSynced(bool arg)
{
    if (m_synced != arg) {
        m_synced = arg;
        emit syncedChanged(arg);
    }
}
