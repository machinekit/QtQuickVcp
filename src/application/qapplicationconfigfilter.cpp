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
#include "qapplicationconfigfilter.h"

/*!
    \qmltype ApplicationConfigFilter
    \instantiates QApplicationConfigFilter
    \inqmlmodule Machinekit.HalRemote
    \brief Application configuration filter
    \ingroup halremote

    This component can be used to filter the results of
    an application configuration query for the configuration
    type.

    \sa ApplicationConfig
*/

/*! \qmlproperty enumeration ApplicationConfigFilter::type

    This property holds the configuration type that should be filtered.

    \list
    \li ApplicationConfigItem.Qt5QmlApplication - Qt5 QML application, can be loaded (default).
    \li ApplicationConfigItem.GladeVcpApplication - GladeVCP application.
    \li ApplicationConfigItem.JavaScriptApplication - JavaScript web application.
    \endlist
*/

/*! \qmlproperty string ApplicationConfigFilter::name

    Application configuration names will be filtered with this property.
    Only configurations with a name that matches this string will be accepted.
    If this property is empty the configuration name will not be filtered.
*/

/*! \qmlproperty string ApplicationConfigFilter::description

    Application configuration descriptions will be filtered with this property.
    Only configurations with a description that contains this string will be accepted.
    If this property is empty the configuration description will not be filtered.
*/

QApplicationConfigFilter::QApplicationConfigFilter(QObject *parent) :
    QObject(parent),
    m_type(QApplicationConfigItem::Qt5QmlApplication),
    m_name(""),
    m_description("")
{
}
