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

#include "qgcodeprogrammodel.h"
#include <QDebug>

QGCodeProgramModel::QGCodeProgramModel(QObject *parent) :
    QAbstractListModel(parent)
{
}

QGCodeProgramModel::~QGCodeProgramModel()
{
    qDeleteAll(m_items);
}

QVariant QGCodeProgramModel::data(const QModelIndex &index, int role) const
{
    return internalData(index, role);
}

bool QGCodeProgramModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    return internalSetData(index, value, role);
}

QModelIndex QGCodeProgramModel::index(const QString &fileName, int lineNumber) const
{
    FileIndex fileIndex;

    if (!m_fileIndices.contains(fileName))
    {
        return QModelIndex();
    }

    fileIndex = m_fileIndices.value(fileName);

    if (lineNumber > fileIndex.count)
    {
        return QModelIndex();
    }

    return createIndex(fileIndex.index + (lineNumber-1), 0);
}

QModelIndex QGCodeProgramModel::index(int row)
{
    return createIndex(row, 0);
}

Qt::ItemFlags QGCodeProgramModel::flags(const QModelIndex &index) const
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

int QGCodeProgramModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return m_items.count();
}

QHash<int, QByteArray> QGCodeProgramModel::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles[FileNameRole] = "fileName";
    roles[LineNumberRole] = "lineNumber";
    roles[GCodeRole] = "gcode";
    roles[PreviewRole] = "preview";
    roles[SelectedRole] = "selected";
    roles[ActiveRole] = "active";
    roles[ExecutedRole] = "executed";
    return roles;
}

void QGCodeProgramModel::prepareFile(const QString &fileName, int lineCount)
{
    FileIndex fileIndex;

    if (m_fileIndices.contains(fileName))
    {
        fileIndex = m_fileIndices.value(fileName);
    }
    else
    {
        fileIndex.index = m_items.count();
        fileIndex.count = 0;
    }

    int firstRow = fileIndex.index + fileIndex.count;
    int lastRow = (fileIndex.index + lineCount - 1);
    int rowCount = lastRow - firstRow + 1;

    beginInsertRows(QModelIndex(), firstRow, lastRow);
    for (int i = firstRow; i <= lastRow; ++i)
    {
        m_items.insert(i, new QGCodeProgramItem(fileName, (i - fileIndex.index + 1)));
    }
    endInsertRows();

    QHashIterator<QString, FileIndex> i(m_fileIndices);
    while (i.hasNext()) {
        i.next();
        FileIndex currentFileIndex = i.value();
        if (currentFileIndex.index > fileIndex.index)    // if file is after the changed file
        {
            currentFileIndex.index += rowCount;
            m_fileIndices.insert(i.key(), currentFileIndex);
        }
    }

    fileIndex.count = lineCount;
    m_fileIndices.insert(fileName, fileIndex);
}

void QGCodeProgramModel::removeFile(const QString &fileName)
{
    FileIndex fileIndex;

    if (!m_fileIndices.contains(fileName))
    {
        return;
    }

    fileIndex = m_fileIndices.value(fileName);

    int firstRow = fileIndex.index;
    int lastRow = fileIndex.index + fileIndex.count - 1;
    int rowCount = lastRow - firstRow + 1;

    beginRemoveRows(QModelIndex(), firstRow, lastRow);
    for (int i = lastRow; i >= firstRow; i--)
    {
        delete m_items.takeAt(i);
    }
    endRemoveRows();

    QHashIterator<QString, FileIndex> i(m_fileIndices);
    while (i.hasNext()) {
        i.next();
        FileIndex currentFileIndex = i.value();
        if (currentFileIndex.index > fileIndex.index)    // if file is after the changed file
        {
            currentFileIndex.index -= rowCount;
            m_fileIndices.insert(i.key(), currentFileIndex);
        }
    }

    m_fileIndices.remove(fileName);
}

void QGCodeProgramModel::addLine(const QString &fileName)
{
    FileIndex fileIndex;

    if (!m_fileIndices.contains(fileName))
    {
        prepareFile(fileName, 1);
        return;
    }

    fileIndex = m_fileIndices.value(fileName);
    fileIndex.count++;

    int row = fileIndex.index + fileIndex.count - 1;

    beginInsertRows(QModelIndex(), row, row);
    m_items.insert(row, new QGCodeProgramItem(fileName, fileIndex.count));
    endInsertRows();

    QHashIterator<QString, FileIndex> i(m_fileIndices);
    while (i.hasNext()) {
        i.next();
        FileIndex currentFileIndex = i.value();
        if (currentFileIndex.index > fileIndex.index)    // if file is after the changed file
        {
            currentFileIndex.index++;
            m_fileIndices.insert(i.key(), currentFileIndex);
        }
    }

    m_fileIndices.insert(fileName, fileIndex);
}

QVariant QGCodeProgramModel::data(const QString &fileName, int lineNumber, int role) const
{
    QModelIndex modelIndex;
    modelIndex = index(fileName, lineNumber);

    return internalData(modelIndex, role);
}

bool QGCodeProgramModel::setData(const QString &fileName, int lineNumber, const QVariant &value, int role)
{
    QModelIndex modelIndex;
    modelIndex = index(fileName, lineNumber);

    return internalSetData(modelIndex, value, role);
}

void QGCodeProgramModel::clear()
{
    if (m_items.count() == 0)
    {
        return;
    }

    beginRemoveRows(QModelIndex(), 0, (m_items.count()-1));
    qDeleteAll(m_items);
    m_items.clear();
    endRemoveRows();

    m_fileIndices.clear();
}

void QGCodeProgramModel::beginUpdate()
{
    beginResetModel();
}

void QGCodeProgramModel::endUpdate()
{
    endResetModel();
}

QVariant QGCodeProgramModel::internalData(const QModelIndex &index, int role) const
{
    if (!index.isValid() || (index.row() > (m_items.count() - 1)))
    {
        return QVariant();
    }

    QGCodeProgramItem *item = m_items.at(index.row());

    switch (role)
    {
    case LineNumberRole:
        return QVariant(item->lineNumber());
    case FileNameRole:
        return QVariant(item->fileName());
    case GCodeRole:
        return QVariant(item->gcode());
    case PreviewRole:
        return QVariant::fromValue(static_cast<void*>(item->previewList()));
    case SelectedRole:
        return QVariant(item->selected());
    case ActiveRole:
        return QVariant(item->active());
    case ExecutedRole:
        return QVariant(item->executed());
    default:
        return QVariant();
    }

    return QVariant();
}

bool QGCodeProgramModel::internalSetData(const QModelIndex &index, const QVariant &value, int role)
{
    if (!index.isValid())
    {
        return false;
    }

    QGCodeProgramItem *item = m_items.at(index.row());


    switch (role)
    {
    case LineNumberRole:
        item->setLineNumber(value.toInt());
        break;
    case FileNameRole:
        item->setFileName(value.toString());
        break;
    case GCodeRole:
        item->setGcode(value.toString());
        break;
    case PreviewRole:
        item->setPreviewList(static_cast<QList<pb::Preview>* >(value.value<void*>()));
        break;
    case SelectedRole:
        item->setSelected(value.toBool());
        break;
    case ActiveRole:
        item->setActive(value.toBool());
        break;
    case ExecutedRole:
        item->setExecuted(value.toBool());
        break;
    default:
        return false;
    }

    QVector<int> changedRoles;
    changedRoles.append(role);
    emit dataChanged(index, index, changedRoles);

    return true;
}
