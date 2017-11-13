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

#ifndef GCODEPROGRAMMODEL_H
#define GCODEPROGRAMMODEL_H

#include <QAbstractListModel>
#include <QLinkedList>
#include "gcodeprogramitem.h"

namespace qtquickvcp {

class GCodeProgramModel : public QAbstractListModel
{
    Q_OBJECT
    Q_ENUMS(GCodeProgramRoles)

public:
    class PreviewItem {
    public:
        QModelIndex modelIndex;
        machinetalk::Preview previewItem;
    };

    enum GCodeProgramRoles {
            FileNameRole = Qt::UserRole,
            LineNumberRole,
            GCodeRole,
            SelectedRole,
            ActiveRole,
            ExecutedRole
        };

    explicit GCodeProgramModel(QObject *parent = 0);
    ~GCodeProgramModel();

    QVariant data(const QModelIndex &index, int role) const;
    bool setData(const QModelIndex &index, const QVariant &value, int role);
    Q_INVOKABLE QVariant getData(const QModelIndex &index, int role) const;
    QModelIndex index(const QString &fileName, int lineNumber) const;
    QModelIndex index(int row, int column = 0, const QModelIndex &parent = QModelIndex()) const;
    Qt::ItemFlags flags(const QModelIndex &index) const;
    int rowCount(const QModelIndex &parent = QModelIndex()) const;
    QHash<int, QByteArray> roleNames() const;
    QLinkedList<PreviewItem> previewItems() const;

public slots:
    void prepareFile(const QString &fileName, int lineCount);
    void removeFile(const QString &fileName);
    void addLine(const QString &fileName);
    void addPreviewItem(const QModelIndex &index, const machinetalk::Preview &previewItem);
    QVariant data(const QString &fileName, int lineNumber, int role) const;
    bool setData(const QString &fileName, int lineNumber, const QVariant &value, int role);
    void clear();
    void clearPreview(bool update = true);
    void clearBackplot(bool update = true);
    void clearSelectionAndSelectLine(const QString &fileName, int lineNumber);
    void clearSelectionAndSelectLine(const QModelIndex &index);
    void beginUpdate();
    void endUpdate();

private:
    typedef struct {
        int index;
        int count;
    } FileIndex;

    QList<GCodeProgramItem*> m_items;
    QHash<QString, FileIndex> m_fileIndices;
    QLinkedList<PreviewItem> m_previewItems;

    QVariant internalData(const QModelIndex &index, int role) const;
    bool internalSetData(const QModelIndex &index, const QVariant &value, int role);
}; // class GCodeProgramModel
} // namespace qtquickvcp

#endif // GCODEPROGRAMMODEL_H
