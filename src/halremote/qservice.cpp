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
#include "qservice.h"

QService::QService(QObject *parent) :
    QObject(parent)
{
    m_uri = "";
    m_version = 0;
    m_minVersion = 0;
    m_api = SA_ZMQ_PROTOBUF;
    m_description = "";
    m_ready = false;
}

void QService::setData(QString uri, int version, QService::ServiceApi api, QString description, bool found)
{
    m_uri = uri;
    m_version = version;
    m_api = api;
    m_description = description;
    m_ready = found;

    emit uriChanged(m_uri);
    emit versionChanged(m_version);
    emit apiChanged(m_api);
    emit descriptionChanged(m_description);
    emit readyChanged(m_ready);
}

void QService::clearFound()
{
    m_ready = false;
    emit readyChanged(m_ready);
}
