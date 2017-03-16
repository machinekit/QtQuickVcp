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

#include "applicationerror.h"
#include <google/protobuf/text_format.h>
#include "debughelper.h"

using namespace machinetalk;

namespace qtquickvcp {

ApplicationError::ApplicationError(QObject *parent) :
    application::ErrorBase(parent),
    m_connected(false),
    m_channels(ErrorChannel | TextChannel | DisplayChannel)
{
}

void ApplicationError::errorMessageReceived(const Container &rx)
{
    for (int i = 0; i < rx.note_size(); ++i)
    {
        messageReceived(static_cast<ErrorType>(rx.type()), QString::fromStdString(rx.note(i)));
    }
}

void ApplicationError::emcNmlErrorReceived(const QByteArray &topic, const Container &rx)
{
    Q_UNUSED(topic);
    errorMessageReceived(rx);
}

void ApplicationError::emcNmlTextReceived(const QByteArray &topic, const Container &rx)
{
    Q_UNUSED(topic);
    errorMessageReceived(rx);
}

void ApplicationError::emcNmlDisplayReceived(const QByteArray &topic, const Container &rx)
{
    Q_UNUSED(topic);
    errorMessageReceived(rx);
}

void ApplicationError::emcOperatorTextReceived(const QByteArray &topic, const Container &rx)
{
    Q_UNUSED(topic);
    errorMessageReceived(rx);
}

void ApplicationError::emcOperatorErrorReceived(const QByteArray &topic, const Container &rx)
{
    Q_UNUSED(topic);
    errorMessageReceived(rx);
}

void ApplicationError::emcOperatorDisplayReceived(const QByteArray &topic, const Container &rx)
{
    Q_UNUSED(topic);
    errorMessageReceived(rx);
}

void ApplicationError::updateTopics()
{
    clearErrorTopics();
    if (m_channels & ErrorChannel) {
        addErrorTopic("error");
    }
    if (m_channels & TextChannel) {
        addErrorTopic("text");
    }
    if (m_channels & DisplayChannel) {
        addErrorTopic("display");
    }
}

void ApplicationError::setConnected()
{
    m_connected = true;
    emit connectedChanged(m_connected);
}

void ApplicationError::clearConnected()
{
    m_connected = false;
    emit connectedChanged(m_connected);
}
} // namespace qtquickvcp
