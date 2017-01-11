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
#include "applicationconfigitem.h"

namespace qtquickvcp {

/*!
    \qmltype ApplicationConfigItem
    \instantiates QApplicationConfigItem
    \inqmlmodule Machinekit.HalRemote
    \brief Application configuration item
    \ingroup halremote

    This component is used to store application configuration data
    resolved by \l ApplicationConfig. It holds all necessary information
    to display and load remote applications.

    \sa ApplicationConfig
*/

/*! \qmlproperty string ApplicationConfigItem::name

    This property holds the name of the configuration. This name
    has to be used for selecting a specific application configuration.
*/

/*! \qmlproperty string ApplicationConfigItem::description

    This property holds the descriptive text provided with each
    application configuration.
*/

/*! \qmlproperty enumeration ApplicationConfigItem::type

    This property holds the type of the configuration.

    \list
    \li ApplicationConfigItem.Qt5QmlApplication - Qt5 QML application, can be loaded (default).
    \li ApplicationConfigItem.GladeVcpApplication - GladeVCP application.
    \li ApplicationConfigItem.JavaScriptApplication - JavaScript web application.
    \endlist
*/

/*! \qmlproperty string ApplicationConfigItem::webUri

    This property holds the web uri of the application. This property
    is only from use for javascvript type web applications. See \l type
    for more information about the configuration types.

    \sa type
*/

/*! \qmlproperty bool ApplicationConfigItem::loaded

    This property holds whether the configuration is loaded or not.
    A loaded application configuration means that the application
    files have been loaded and are ready to be executed.

    The default value is \c{false}.
*/

/*! \qmlproperty list<string> ApplicationConfigItem::files

    This property holds a list of the loaded file paths.
    Typically the loaded files are located within the temporary
    files directory. If the configuration is not \l loaded
    this property will be empty.

    \sa mainFile
*/

/*! \qmlproperty string ApplicationConfigItem::mainFile

    This property holds the main application file of the configuration.
    The main file is selected using the following succession:
    \list
    \li File with the name equal to \l{name}.qml .
    \li File with name \c main.qml .
    \li First entry of \l{files}.
    \endlist

    This property is empty if the configuration is not \l{loaded}.

    \sa files
*/

/*! \qmlproperty string ApplicationConfigItem::translationsPath

    This property holds the path where translation files are located.

    This property is empty if the configuration is not \l{loaded} or
    no translations ship with the configuration.
*/

ApplicationConfigItem::ApplicationConfigItem(QObject *parent) :
    QObject(parent),
    m_name(""),
    m_description(""),
    m_type(Qt5QmlApplication),
    m_webUri(""),
    m_loaded(false),
    m_loading(false),
    m_files(QStringList()),
    m_mainFile(""),
    m_translationsPath("")
{

}
}; // namespace qtquickvcp
