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
    Q_PROPERTY(ApplicationType type READ type WRITE setType NOTIFY typeChanged)
    Q_PROPERTY(QString name READ name WRITE setName NOTIFY nameChanged)
    Q_PROPERTY(QString description READ description WRITE setDescription NOTIFY descriptionChanged)

public:
    enum ApplicationType {
        Qt5QmlApplication = ApplicationConfigItem::Qt5QmlApplication,
        GladeVcpApplication = ApplicationConfigItem::GladeVcpApplication,
        JavaScriptApplication = ApplicationConfigItem::JavaScriptApplication
    };
    Q_ENUM(ApplicationType)

    explicit ApplicationConfigFilter(QObject *parent = 0);

    ApplicationType type() const;
    QString name() const;
    QString description() const;

public slots:

    void setType(ApplicationType arg);
    void setName(const QString &arg);
    void setDescription(const QString &arg);

private:
    ApplicationType m_type;
    QString m_name;
    QString m_description;

signals:
    void typeChanged(ApplicationType arg);
    void nameChanged(const QString &arg);
    void descriptionChanged(const QString &arg);

}; // class ApplicationConfigItem
} // namespace qtquickvcp

#endif // APPLICATIONCONFIGFILTER_H
