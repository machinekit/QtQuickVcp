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

#include "abstractserviceimplementation.h"

namespace qtquickvcp {

AbstractServiceImplementation::AbstractServiceImplementation(QObject *parent) :
    QObject(parent),
    QQmlParserStatus(),
    m_componentCompleted(false),
    m_required(false),
    m_ready(false)
{
}

AbstractServiceImplementation::~AbstractServiceImplementation()
{
    cleanup();
}

/** componentComplete is executed when the QML component is fully loaded */
void AbstractServiceImplementation::componentComplete()
{
    m_componentCompleted = true;

    if (m_ready == true)    // the component was set to ready before it was completed
    {
        start();
    }
}

/** If the ready property has a rising edge we try to connect
 *  if it is has a falling edge we disconnect and cleanup
 */
void AbstractServiceImplementation::setReady(bool arg)
{
    if (m_ready != arg) {
        m_ready = arg;
        emit readyChanged(arg);

        if (m_componentCompleted == false)
        {
            return;
        }

        if (m_ready)
        {
            start();
        }
        else
        {
            stop();
        }
    }
}

/** Resets the connection state of a component.
 *  If an error was active it is cleared. If the
 *  component was connected a reconnect is issued.
 **/
void AbstractServiceImplementation::reset()
{
    if (m_ready)
    {
        stop();
        start();
    }
}
}; // namespace qtquickvcp
