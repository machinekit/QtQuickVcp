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

#include "applicationfilemodel.h"

namespace qtquickvcp {

ApplicationFileModel::ApplicationFileModel(QObject *parent):
    QAbstractListModel(parent)
{

}

ApplicationFileModel::~ApplicationFileModel()
{
    qDeleteAll(m_items);
}

QVariant ApplicationFileModel::data(const QModelIndex &index, int role) const
{
    return internalData(index, role);
}

QModelIndex ApplicationFileModel::index(int row, int column, const QModelIndex &parent) const
{
    Q_UNUSED(column)
    Q_UNUSED(parent)
    return createIndex(row, 0);
}

Qt::ItemFlags ApplicationFileModel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
    {
        return 0;
    }
    else
    {
        return (Qt::ItemIsEditable | Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemNeverHasChildren);
    }
}

int ApplicationFileModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return m_items.count();
}

QHash<int, QByteArray> ApplicationFileModel::roleNames() const
{

    QHash<int, QByteArray> roles;
    roles[NameRole] = "name";
    roles[SizeRole] = "size";
    roles[OwnerRole] = "owner";
    roles[GroupRole] = "group";
    roles[LastModifiedRole] = "lastModified";
    roles[DirRole] = "dir";
    return roles;
}

void ApplicationFileModel::addItem(ApplicationFileItem *item)
{
    beginUpdate();
    m_items.append(item);
    endUpdate();
}

QString ApplicationFileModel::getName(int row)
{
    return data(createIndex(row, 0), NameRole).toString();
}

bool ApplicationFileModel::getIsDir(int row)
{
    return data(createIndex(row, 0), DirRole).toBool();
}

void ApplicationFileModel::clear()
{
    if (m_items.count() == 0)
    {
        return;
    }

    beginRemoveRows(QModelIndex(), 0, (m_items.count()-1));
    qDeleteAll(m_items);
    m_items.clear();
    endRemoveRows();
}

void ApplicationFileModel::beginUpdate()
{
    beginResetModel();
}

void ApplicationFileModel::endUpdate()
{
    endResetModel();
}

QVariant ApplicationFileModel::internalData(const QModelIndex &index, int role) const
{
    if (!index.isValid() || (index.row() > (m_items.count() - 1)))
    {
        return QVariant();
    }

    ApplicationFileItem *item = m_items.at(index.row());

    switch (role)
    {
    case NameRole:
        return QVariant(item->name());
    case SizeRole:
        return QVariant(formatByteSize(item->size()));
    case OwnerRole:
        return QVariant(item->owner());
    case GroupRole:
        return QVariant(item->group());
    case LastModifiedRole:
        return QVariant(item->lastModified().toString(Qt::SystemLocaleShortDate));
    case DirRole:
        return QVariant(item->isDir());
    default:
        return QVariant();
    }
}

QString ApplicationFileModel::formatByteSize(qint64 bytes) const
{
    const qint64 B = 1;
    const qint64 kB = B * 1024;
    const qint64 MB = kB * 1024;
    const qint64 GB = MB * 1024;
    const qint64 TB = GB * 1024;

    if (bytes > TB) {
        return QString::number((double)bytes/(double)TB, 'f', 2) + "TB";
    }
    else if (bytes > GB) {
        return QString::number((double)bytes/(double)GB, 'f', 2) + "GB";
    }
    else if (bytes > MB) {
        return QString::number((double)bytes/(double)MB, 'f', 2) + "MB";
    }
    else if (bytes > kB) {
        return QString::number((double)bytes/(double)kB, 'f', 2) + "kB";
    }
    else {
        return QString::number((double)bytes, 'f', 0) + "B";
    }
}
} // namespace qtquickvcp
