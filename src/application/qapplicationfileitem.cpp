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

#include "qapplicationfileitem.h"

QApplicationFileItem::QApplicationFileItem():
    m_name(""),
    m_size(0),
    m_owner(""),
    m_group(""),
    m_lastModified(QDateTime()),
    m_dir(false)
{

}

QString QApplicationFileItem::name() const
{
    return m_name;
}

void QApplicationFileItem::setName(const QString &name)
{
    m_name = name;
}

qint64 QApplicationFileItem::size() const
{
    return m_size;
}

void QApplicationFileItem::setSize(qint64 size)
{
    m_size = size;
}

QString QApplicationFileItem::owner() const
{
    return m_owner;
}

void QApplicationFileItem::setOwner(const QString &owner)
{
    m_owner = owner;
}

QString QApplicationFileItem::group() const
{
    return m_group;
}

void QApplicationFileItem::setGroup(const QString &group)
{
    m_group = group;
}

QDateTime QApplicationFileItem::lastModified() const
{
    return m_lastModified;
}

void QApplicationFileItem::setLastModified(const QDateTime &dateTime)
{
    m_lastModified = dateTime;
}

bool QApplicationFileItem::isDir() const
{
    return m_dir;
}

void QApplicationFileItem::setDir(bool dir)
{
    m_dir = dir;
}
