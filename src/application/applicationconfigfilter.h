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
#ifndef APPLICATIONCONFIGFILTER_H
#define APPLICATIONCONFIGFILTER_H

#include <QObject>
#include "applicationconfigitem.h"

namespace qtquickvcp {

class ApplicationConfigFilter : public QObject
{
    Q_OBJECT
    Q_PROPERTY(ApplicationConfigItem::ApplicationType type READ type WRITE setType NOTIFY typeChanged)
    Q_PROPERTY(QString name READ name WRITE setName NOTIFY nameChanged)
    Q_PROPERTY(QString description READ description WRITE setDescription NOTIFY descriptionChanged)

public:
    explicit ApplicationConfigFilter(QObject *parent = 0);

    ApplicationConfigItem::ApplicationType type() const
    {
        return m_type;
    }

    QString name() const
    {
        return m_name;
    }

    QString description() const
    {
        return m_description;
    }

public slots:

    void setType(ApplicationConfigItem::ApplicationType arg)
    {
        if (m_type != arg) {
            m_type = arg;
            emit typeChanged(arg);
        }
    }

    void setName(QString arg)
    {
        if (m_name == arg)
            return;

        m_name = arg;
        emit nameChanged(arg);
    }

    void setDescription(QString arg)
    {
        if (m_description == arg)
            return;

        m_description = arg;
        emit descriptionChanged(arg);
    }

private:
    ApplicationConfigItem::ApplicationType m_type;
    QString m_name;
    QString m_description;

signals:
    void typeChanged(ApplicationConfigItem::ApplicationType arg);
    void nameChanged(QString arg);
    void descriptionChanged(QString arg);

}; // class ApplicationConfigItem
}; // namespace qtquickvcp

#endif // APPLICATIONCONFIGFILTER_H
