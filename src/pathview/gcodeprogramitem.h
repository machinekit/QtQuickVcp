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

#ifndef GCODEPROGRAMITEM_H
#define GCODEPROGRAMITEM_H
#include <QVariant>
#include <QList>
#include <machinetalk/protobuf/preview.pb.h>

namespace qtquickvcp {

class GCodeProgramItem
{
public:
    explicit GCodeProgramItem(const QString &fileName, int lineNumber);
    ~GCodeProgramItem();

    QString fileName() const;
    void setFileName(const QString &fileName);

    int lineNumber() const;
    void setLineNumber(int lineNumber);

    QString gcode() const;
    void setGcode(const QString &gcode);

    bool selected() const;
    void setSelected(bool selected);

    bool active() const;
    void setActive(bool active);

    bool executed() const;
    void setExecuted(bool executed);

private:
    QString m_fileName;
    int m_lineNumber;
    QString m_gcode;
    bool m_selected;
    bool m_active;
    bool m_executed;
}; // class GCodeProgramItem
}; // namespace qtquickvcp

#endif // GCODEPROGRAMITEM_H
