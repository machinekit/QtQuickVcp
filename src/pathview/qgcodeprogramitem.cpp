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

#include "qgcodeprogramitem.h"

QGCodeProgramItem::QGCodeProgramItem(const QString &fileName, int lineNumber):
    m_fileName(fileName),
    m_lineNumber(lineNumber),
    m_gcode(QString("")),
    m_selected(false),
    m_active(false),
    m_executed(false)
{
}

QGCodeProgramItem::~QGCodeProgramItem()
{
}

QString QGCodeProgramItem::fileName() const
{
    return m_fileName;
}

void QGCodeProgramItem::setFileName(const QString &fileName)
{
    m_fileName = fileName;
}
int QGCodeProgramItem::lineNumber() const
{
    return m_lineNumber;
}

void QGCodeProgramItem::setLineNumber(int lineNumber)
{
    m_lineNumber = lineNumber;
}
QString QGCodeProgramItem::gcode() const
{
    return m_gcode;
}

void QGCodeProgramItem::setGcode(const QString &gcode)
{
    m_gcode = gcode;
}

bool QGCodeProgramItem::selected() const
{
    return m_selected;
}

void QGCodeProgramItem::setSelected(bool selected)
{
    m_selected = selected;
}
bool QGCodeProgramItem::active() const
{
    return m_active;
}

void QGCodeProgramItem::setActive(bool active)
{
    m_active = active;
}
bool QGCodeProgramItem::executed() const
{
    return m_executed;
}

void QGCodeProgramItem::setExecuted(bool executed)
{
    m_executed = executed;
}






