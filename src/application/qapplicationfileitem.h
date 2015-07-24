/****************************************************************************
**
** Copyright (C) 2015 Alexander Rössler
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

#ifndef QAPPLICATIONFILEITEM_H
#define QAPPLICATIONFILEITEM_H

#include <QVariant>
#include <QDateTime>

class QApplicationFileItem : public QObject
{
public:
    explicit QApplicationFileItem();

    QString name() const;
    void setName(const QString &name);

    qint64 size() const;
    void setSize(qint64 size);

    QString owner() const;
    void setOwner(const QString &owner);

    QString group() const;
    void setGroup(const QString &group);

    QDateTime lastModified() const;
    void setLastModified(const QDateTime &dateTime);

    bool isDir() const;
    void setDir(bool dir);

private:
    QString m_name;
    qint64 m_size;
    QString m_owner;
    QString m_group;
    QDateTime m_lastModified;
    bool m_dir;
};

#endif // QAPPLICATIONFILEITEM_H
