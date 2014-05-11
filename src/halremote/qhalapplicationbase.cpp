#include "qhalapplicationbase.h"

QHalApplicationBase::QHalApplicationBase(QQuickItem *parent) :
    QQuickItem(parent)
{
}

QQmlListProperty<QService> QHalApplicationBase::services()
{
    return QQmlListProperty<QService>(this, m_services);
}

int QHalApplicationBase::serviceCount() const
{
    return m_services.count();
}

QService *QHalApplicationBase::service(int index) const
{
    return m_services.at(index);
}
