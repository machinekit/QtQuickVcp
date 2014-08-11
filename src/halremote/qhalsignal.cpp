#include "qhalsignal.h"

QHalSignal::QHalSignal(QObject *parent) :
    QObject(parent),
    m_name("default"),
    m_type(Bit),
    m_value(false),
    m_handle(0),
    m_enabled(true)
{
}
