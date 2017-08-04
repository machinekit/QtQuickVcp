/****************************************************************************
**
** Copyright (C) 2014 Alexander Rössler
** License: LGPL version 2.1
**
** This file is part of QtQuickVcp.
**
** All rights reserved. This program and the accompanying materials
** are made available under the terms of the GNU Lesser General Public License
** (LGPL) version 2.1 which accompanies this distribution, and is available at
** http://www.gnu.org/licenses/lgpl-2.1.html
**
** This library is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
** Lesser General Public License for more details.
**
** Contributors:
** Alexander Rössler @ The Cool Tool GmbH <mail DOT aroessler AT gmail DOT com>
**
****************************************************************************/
#ifndef SERVICELIST_H
#define SERVICELIST_H

#include <QObject>
#include <QQmlListProperty>
#include <QList>
#include "service.h"

namespace qtquickvcp {

class ServiceList : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QQmlListProperty<qtquickvcp::Service> services READ services)
    Q_CLASSINFO("DefaultProperty", "services")

public:
    explicit ServiceList(QObject *parent = 0);

    QQmlListProperty<Service> services();
    int serviceCount() const;
    Service *service(int index) const;

    inline QList<Service*>::iterator begin() { return m_services.begin(); }
    inline QList<Service*>::iterator end() { return m_services.end(); }

private:
    QList<Service*> m_services;

}; // class ServiceList
} // namespace qtquickvcp

#endif // SERVICELIST_H
