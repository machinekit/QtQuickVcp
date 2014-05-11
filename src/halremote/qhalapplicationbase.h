#ifndef QHALAPPLICATIONBASE_H
#define QHALAPPLICATIONBASE_H

#include <QQuickItem>
#include "qservice.h"

class QHalApplicationBase : public QQuickItem
{
    Q_OBJECT
    Q_PROPERTY(QQmlListProperty<QService> services READ services)

public:
    explicit QHalApplicationBase(QQuickItem *parent = 0);

    QQmlListProperty<QService> services();
    int serviceCount() const;
    QService *service(int index) const;

private:
    QList<QService*> m_services;

};

#endif // QHALAPPLICATIONBASE_H
