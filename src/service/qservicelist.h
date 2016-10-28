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
#ifndef QSERVICELIST_H
#define QSERVICELIST_H

#include <QObject>
#include <QQmlListProperty>
#include "qservice.h"

class QServiceList : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QQmlListProperty<QService> services READ services)
    Q_CLASSINFO("DefaultProperty", "services")

public:
    explicit QServiceList(QObject *parent = 0);

    QQmlListProperty<QService> services();
    int serviceCount() const;
    QService *service(int index) const;

private:
    QList<QService*> m_services;

};

#endif // QSERVICELIST_H
