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
#ifndef QAPPCONFIGFILTER_H
#define QAPPCONFIGFILTER_H

#include <QObject>
#include "qapplicationconfigitem.h"

class QApplicationConfigFilter : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QApplicationConfigItem::ApplicationType type READ type WRITE setType NOTIFY typeChanged)


public:
    explicit QApplicationConfigFilter(QObject *parent = 0);

    QApplicationConfigItem::ApplicationType type() const
    {
        return m_type;
    }

public slots:

    void setType(QApplicationConfigItem::ApplicationType arg)
    {
        if (m_type != arg) {
            m_type = arg;
            emit typeChanged(arg);
        }
    }

private:
    QApplicationConfigItem::ApplicationType m_type;

signals:

    void typeChanged(QApplicationConfigItem::ApplicationType arg);
};

#endif // QAPPCONFIGFILTER_H
