#include "qpin.h"

QPin::QPin(QQuickItem *parent) :
    QQuickItem(parent)
{
    /* Initialize default variables */
    m_name = "Default";
    m_type = HAL_BIT;
    m_direction = HAL_OUT;
    m_value = false;
}
