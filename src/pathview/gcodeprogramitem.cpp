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

#include "gcodeprogramitem.h"

namespace qtquickvcp {

GCodeProgramItem::GCodeProgramItem(const QString &fileName, int lineNumber):
    m_fileName(fileName),
    m_lineNumber(lineNumber),
    m_gcode(QString("")),
    m_selected(false),
    m_active(false),
    m_executed(false)
{
}

GCodeProgramItem::~GCodeProgramItem()
{
}

QString GCodeProgramItem::fileName() const
{
    return m_fileName;
}

void GCodeProgramItem::setFileName(const QString &fileName)
{
    m_fileName = fileName;
}
int GCodeProgramItem::lineNumber() const
{
    return m_lineNumber;
}

void GCodeProgramItem::setLineNumber(int lineNumber)
{
    m_lineNumber = lineNumber;
}
QString GCodeProgramItem::gcode() const
{
    return m_gcode;
}

void GCodeProgramItem::setGcode(const QString &gcode)
{
    m_gcode = gcode;
}

bool GCodeProgramItem::selected() const
{
    return m_selected;
}

void GCodeProgramItem::setSelected(bool selected)
{
    m_selected = selected;
}
bool GCodeProgramItem::active() const
{
    return m_active;
}

void GCodeProgramItem::setActive(bool active)
{
    m_active = active;
}
bool GCodeProgramItem::executed() const
{
    return m_executed;
}

void GCodeProgramItem::setExecuted(bool executed)
{
    m_executed = executed;
}
}; // namespace qtquickvcp
