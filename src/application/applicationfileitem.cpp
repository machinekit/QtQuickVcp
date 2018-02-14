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

#include "applicationfileitem.h"

namespace qtquickvcp {

ApplicationFileItem::ApplicationFileItem():
    m_name(QLatin1String("")),
    m_size(0),
    m_owner(QLatin1String("")),
    m_group(QLatin1String("")),
    m_lastModified(QDateTime()),
    m_dir(false)
{

}

QString ApplicationFileItem::name() const
{
    return m_name;
}

void ApplicationFileItem::setName(const QString &name)
{
    m_name = name;
}

qint64 ApplicationFileItem::size() const
{
    return m_size;
}

void ApplicationFileItem::setSize(qint64 size)
{
    m_size = size;
}

QString ApplicationFileItem::owner() const
{
    return m_owner;
}

void ApplicationFileItem::setOwner(const QString &owner)
{
    m_owner = owner;
}

QString ApplicationFileItem::group() const
{
    return m_group;
}

void ApplicationFileItem::setGroup(const QString &group)
{
    m_group = group;
}

QDateTime ApplicationFileItem::lastModified() const
{
    return m_lastModified;
}

void ApplicationFileItem::setLastModified(const QDateTime &dateTime)
{
    m_lastModified = dateTime;
}

bool ApplicationFileItem::isDir() const
{
    return m_dir;
}

void ApplicationFileItem::setDir(bool dir)
{
    m_dir = dir;
}
} // namespace qtquickvcp
