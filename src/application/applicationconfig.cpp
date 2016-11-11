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
#include "applicationconfig.h"
#include "machinetalkservice.h"

#if defined(Q_OS_IOS)
namespace gpb = google_public::protobuf;
#else
namespace gpb = google::protobuf;
#endif

using namespace nzmqt;

namespace qtquickvcp {

/*!
    \qmltype ApplicationConfig
    \instantiates QApplicationConfig
    \inqmlmodule Machinekit.HalRemote
    \brief Application configuration client
    \ingroup halremote

    This component acts as client for a config-server providing
    application configurations. The ApplicationConfig component
    provides a interface for gathering available configurations
    as well as fetching QML files from the remote server.

    The following example demonstrates a basic configuration for
    displaying and selecting available application configurations
    in a list view.

    \qml
    Item {
        ListView {
            id: listView

            anchors.fill: parent
            model: applicationConfig.configs
            delegate: Button {
                        anchors.left: parent.left
                        anchors.right: parent.right
                        text: name

                        onClicked: applicationConfig.selectConfig(name)
                    }
        }

        ApplicationConfig {
            id: applicationConfig

            configUri: "tcp://192.168.1.2:4999"
            ready: true
            filter: ApplicationConfigFilter { type: ApplicationConfigItem.Qt5QmlApplication }
        }
    }
    \endqml

    The above example can be extended with a \l Loader component to load the
    loaded QML files.

    \qml
    Loader {
        id: applicationLoader

        anchors.fill: parent
        active: applicationConfig.selectedConfig.loaded
        source: applicationConfig.selectedConfig.mainFile
    }
    \endqml

    \sa ApplicationConfigItem, ApplicationConfigFilter
*/

/*! \qmlproperty string ApplicationConfig::configUri

    This property holds the config service uri.
*/

/*! \qmlproperty bool ApplicationConfig::connected

    This property holds wheter the component is connected or not. This is the
    same as \l{connectionState} == \c{ApplicationConfig.Connected}.
 */

/*! \qmlproperty bool ApplicationConfig::ready

    This property holds whether the application config is ready or not.
    If the property is set to \c true the component will try to connect. If the
    property is set to \c false all connections will be closed.

    The default value is \c{false}.
*/

/*! \qmlproperty enumeration ApplicationConfig::connectionState

    This property holds the connection state of the application config.

    \list
    \li ApplicationConfig.Disconnected - The component is not connected.
    \li ApplicationConfig.Connected - The component is connected.
    \li ApplicationConfig.Error - An error has happened. See \l error and \l errorString for details about the error.
    \endlist
*/

/*! \qmlproperty enumeration ApplicationConfig::error

    This property holds the currently active error. See \l errorString
    for a description of the active error.

    \list
    \li ApplicationConfig.NoError - No error happened.
    \li ApplicationConfig.SocketError - An error related to the socket happened.
    \endlist

    \sa errorString
*/

/*! \qmlproperty string ApplicationConfig::errorString

    This property holds a text description of the last error that occured.
    If \l error holds a error value check this property for the description.

    \sa error
*/

/*! \qmlproperty ApplicationConfigItem ApplicationConfig::selectedConfig

    This property holds the selected configuration. This property can be
    used to get the files and description of an application.
*/

/*! \qmlproperty list<ApplicationConfigItem> ApplicationConfig::configs

    This property holds a list of all available configurations.
*/

/*! \qmlproperty ApplicationConfigFilter ApplicationConfig::filter

    This property holds the filters that will be applied to available configurations.
    The filter values are AND connected.
*/

/*! \qmlmethod void QApplicationConfig::selectConfig(QString name)

    Selects the configuration with the given name and updates \l{selectedConfig}.
*/

/*! \qmlmethod void QApplicationConfig::unselectConfig()

    Unselects the configuration with the given name and updates \l{selectedConfig}.
*/
ApplicationConfig::ApplicationConfig(QQuickItem *parent) :
    application::ConfigBase(parent),
     m_synced(false),
     m_selectedConfig(new ApplicationConfigItem(this)),
     m_filter(new ApplicationConfigFilter(this))
{
}

ApplicationConfig::~ApplicationConfig()
{
    cleanupFiles();
}

void ApplicationConfig::cleanupFiles()
{
    if (!m_selectedConfig->name().isEmpty())
    {
        QString path = MachinetalkService::applicationTempPath(m_selectedConfig->name());
        QDir dir(path);
        dir.removeRecursively();
    }
}

void ApplicationConfig::describeApplicationReceived(pb::Container *rx)
{
    for (int i = 0; i < rx->app_size(); ++i)
    {
        pb::Application app;

        app = rx->app(i);

        ApplicationConfigItem::ApplicationType type;
        QString name;
        QString description;

        type = static_cast<ApplicationConfigItem::ApplicationType>(app.type());
        name = QString::fromStdString(app.name());
        description = QString::fromStdString(app.description());

        if ((m_filter->type() == type)
             && (m_filter->name().isEmpty() || (name == m_filter->name()))
             && (m_filter->description().isEmpty() || description.contains(m_filter->description())))
        {
            ApplicationConfigItem *appConfigItem;

            appConfigItem = new ApplicationConfigItem(this);
            appConfigItem->setName(name);
            appConfigItem->setDescription(description);
            appConfigItem->setType(type);
            m_configs.append(appConfigItem);
            emit configsChanged(QQmlListProperty<ApplicationConfigItem>(this, m_configs));
        }
    }
}

void ApplicationConfig::applicationDetailReceived(pb::Container *rx)
{
    for (int i = 0; i < rx->app_size(); ++i)
    {
        pb::Application app;

        app = rx->app(i);

        ApplicationConfigItem::ApplicationType type;

        type = (ApplicationConfigItem::ApplicationType)app.type();

        if (m_filter->type() == type)     // detail comes when application was already filtered, so we only check the type to make sure it is compatible
        {
            QString baseFilePath;
            QStringList fileList;
            QDir dir;

            if (m_selectedConfig == nullptr)
            {
                return;
            }

            m_selectedConfig->setName(QString::fromStdString(app.name()));
            m_selectedConfig->setDescription(QString::fromStdString(app.description()));
            m_selectedConfig->setType(type);

            baseFilePath = MachinetalkService::applicationTempPath(m_selectedConfig->name());
            if (!dir.mkpath(baseFilePath))
            {
                qWarning() << "unable to create directory " << baseFilePath;
            }

#ifdef QT_DEBUG
            qDebug() << "base file path:" << baseFilePath;
#endif

            for (int j = 0; j < app.file_size(); ++j)
            {
                pb::File file;
                QString filePath;
                QByteArray data;

                file = app.file(j);
                filePath = baseFilePath + QString::fromStdString(file.name());

                QFileInfo fileInfo(filePath);
                if (!dir.mkpath(fileInfo.absolutePath()))
                {
                    qDebug() << "not able to create directory";
                }

                QFile localFile(filePath);
                if (!localFile.open(QIODevice::WriteOnly))
                {
                    qDebug() << "not able to create file" << filePath;
                    continue;
                }

                data = QByteArray(file.blob().data(), file.blob().size());

                if (file.encoding() == pb::ZLIB)
                {
                    quint32 test = ((quint32)data.at(0) << 24) + ((quint32)data.at(1) << 16) + ((quint32)data.at(2) << 8) + ((quint32)data.at(3) << 0);
                    qDebug() << test << (quint8)data.at(0) << (quint8)data.at(1) << (quint8)data.at(2) << (quint8)data.at(3);   // TODO
                    data = qUncompress(data);
                }
                else if (file.encoding() != pb::CLEARTEXT)
                {
                    qDebug() << "unknown encoding";
                    localFile.close();
                    continue;
                }

                localFile.write(data);
                localFile.close();

                fileList.append(filePath);

#ifdef QT_DEBUG
                qDebug() << "created file: " << filePath;
#endif
            }

            ApplicationDescription applicationDescription;

            applicationDescription.setSourceDir(QUrl("file:///" + baseFilePath));
            // TODO check validity

            m_selectedConfig->setFiles(fileList);
            m_selectedConfig->setMainFile(applicationDescription.mainFile());
            m_selectedConfig->setLoaded(true);
            m_selectedConfig->setLoading(false);
        }
    }
}

void ApplicationConfig::syncConfig()
{
    m_synced = true;
    emit syncedChanged(m_synced);
}

void ApplicationConfig::unsyncConfig()
{
    m_synced = false;
    emit syncedChanged(m_synced);

    m_configs.clear();
    emit configsChanged(QQmlListProperty<ApplicationConfigItem>(this, m_configs));
}

void ApplicationConfig::selectConfig(QString name)
{
    m_selectedConfig->setLoaded(false);
    m_selectedConfig->setLoading(true);
    m_selectedConfig->setName(name);

    pb::Application *app = m_tx.add_app();

    app->set_name(name.toStdString());
    sendRetrieveApplication(&m_tx);
}

void ApplicationConfig::unselectConfig()
{
    cleanupFiles();

    m_selectedConfig->setName("");
    m_selectedConfig->setDescription("");
    m_selectedConfig->setFiles(QStringList());
    m_selectedConfig->setMainFile(QUrl(""));
    m_selectedConfig->setLoaded(false);
    m_selectedConfig->setLoading(false);
}

}; // namespace qtquickvcp
