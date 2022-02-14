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

#include "gcodeprogrammodel.h"
#include <QDebug>
#include <QDir>

using namespace machinetalk;

namespace qtquickvcp {

GCodeProgramModel::GCodeProgramModel(QObject *parent) :
    QAbstractListModel(parent)
{
}

GCodeProgramModel::~GCodeProgramModel()
{
    qDeleteAll(m_items);
}

QVariant GCodeProgramModel::data(const QModelIndex &index, int role) const
{
    return internalData(index, role);
}

bool GCodeProgramModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    return internalSetData(index, value, role);
}

QVariant GCodeProgramModel::getData(const QModelIndex &index, int role) const
{
    return data(index, role);
}

QModelIndex GCodeProgramModel::index(const QString &fileName, int lineNumber) const
{
    FileIndex fileIndex;
    const QString &cleanPath = QDir::cleanPath(fileName);

    if (!m_fileIndices.contains(cleanPath))
    {
        return QModelIndex();
    }

    fileIndex = m_fileIndices.value(cleanPath);

    if (lineNumber > fileIndex.count)
    {
        return QModelIndex();
    }

    return createIndex(fileIndex.index + (lineNumber-1), 0);
}

QModelIndex GCodeProgramModel::index(int row, int column, const QModelIndex &parent) const
{
    Q_UNUSED(column)
    Q_UNUSED(parent)
    return createIndex(row, 0);
}

Qt::ItemFlags GCodeProgramModel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
    {
        return Qt::ItemFlags();
    }
    else
    {
        return (Qt::ItemIsEditable | Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemNeverHasChildren);
    }
}

int GCodeProgramModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return m_items.count();
}

QHash<int, QByteArray> GCodeProgramModel::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles[FileNameRole] = "fileName";
    roles[LineNumberRole] = "lineNumber";
    roles[GCodeRole] = "gcode";
    roles[SelectedRole] = "selected";
    roles[ActiveRole] = "active";
    roles[ExecutedRole] = "executed";
    return roles;
}

std::list<GCodeProgramModel::PreviewItem> GCodeProgramModel::previewItems() const
{
    return m_previewItems;
}

void GCodeProgramModel::prepareFile(const QString &fileName, int lineCount)
{
    FileIndex fileIndex;
    const QString &cleanPath = QDir::cleanPath(fileName);

    if (m_fileIndices.contains(cleanPath))
    {
        fileIndex = m_fileIndices.value(cleanPath);
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
        m_items.insert(i, new GCodeProgramItem(cleanPath, (i - fileIndex.index + 1)));
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
    m_fileIndices.insert(cleanPath, fileIndex);
}

void GCodeProgramModel::removeFile(const QString &fileName)
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

void GCodeProgramModel::addLine(const QString &fileName)
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
    m_items.insert(row, new GCodeProgramItem(fileName, fileIndex.count));
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

void GCodeProgramModel::addPreviewItem(const QModelIndex &index, const Preview &previewItem)
{
    PreviewItem item;
    item.modelIndex = index;
    item.previewItem = previewItem;
    m_previewItems.push_back(item);
}

QVariant GCodeProgramModel::data(const QString &fileName, int lineNumber, int role) const
{
    QModelIndex modelIndex;
    modelIndex = index(fileName, lineNumber);

    return internalData(modelIndex, role);
}

bool GCodeProgramModel::setData(const QString &fileName, int lineNumber, const QVariant &value, int role)
{
    QModelIndex modelIndex;
    modelIndex = index(fileName, lineNumber);

    return internalSetData(modelIndex, value, role);
}

void GCodeProgramModel::clear()
{
    if (m_items.count() == 0)
    {
        return;
    }

    beginRemoveRows(QModelIndex(), 0, (m_items.count()-1));
    qDeleteAll(m_items);
    m_items.clear();
    m_previewItems.clear();
    endRemoveRows();

    m_fileIndices.clear();
}

void GCodeProgramModel::clearPreview(bool update)
{
    if (update)
    {
        beginUpdate();
    }
    m_previewItems.clear();
    if (update)
    {
        endUpdate();
    }
}

void GCodeProgramModel::clearBackplot(bool update)
{
    if (update)
    {
        beginUpdate();
    }
    for (auto item: qAsConst(m_items)) {
        item->setExecuted(false);
        item->setActive(false);
        item->setSelected(false);
    }
    if (update)
    {
        endUpdate();
    }
}

void GCodeProgramModel::clearSelectionAndSelectLine(const QString &fileName, int lineNumber)
{
    clearSelectionAndSelectLine(index(fileName, lineNumber));
}

void GCodeProgramModel::clearSelectionAndSelectLine(const QModelIndex &index)
{
    for (auto item: qAsConst(m_items)) {
        if (item->selected()) {
            item->setSelected(false);
            setData(item->fileName(), item->lineNumber(), false, SelectedRole);
        }
    }
    setData(index, true, SelectedRole);
}

void GCodeProgramModel::beginUpdate()
{
    beginResetModel();
}

void GCodeProgramModel::endUpdate()
{
    endResetModel();
}

QVariant GCodeProgramModel::internalData(const QModelIndex &index, int role) const
{
    if (!index.isValid() || (index.row() > (m_items.count() - 1)))
    {
        return QVariant();
    }

    GCodeProgramItem *item = m_items.at(index.row());

    switch (role)
    {
    case LineNumberRole:
        return QVariant(item->lineNumber());
    case FileNameRole:
        return QVariant(item->fileName());
    case GCodeRole:
        return QVariant(item->gcode());
    case SelectedRole:
        return QVariant(item->selected());
    case ActiveRole:
        return QVariant(item->active());
    case ExecutedRole:
        return QVariant(item->executed());
    default:
        return QVariant();
    }
}

bool GCodeProgramModel::internalSetData(const QModelIndex &index, const QVariant &value, int role)
{
    if (!index.isValid())
    {
        return false;
    }

    GCodeProgramItem *item = m_items.at(index.row());


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
} // namespace qtquickvcp
