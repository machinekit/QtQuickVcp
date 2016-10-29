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
#include "qapplicationconfig.h"
#include "service.h"

#if defined(Q_OS_IOS)
namespace gpb = google_public::protobuf;
#else
namespace gpb = google::protobuf;
#endif

using namespace nzmqt;

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

QApplicationConfig::QApplicationConfig(QQuickItem *parent) :
    QQuickItem(parent),
     m_componentCompleted(false),
     m_configUri(""),
     m_ready(false),
     m_connected(false),
     m_connectionState(Disconnected),
     m_error(NoError),
     m_errorString(""),
     m_selectedConfig(new QApplicationConfigItem(this)),
     m_filter(new QApplicationConfigFilter(this)),
     m_context(nullptr),
     m_configSocket(nullptr)
{
}

QApplicationConfig::~QApplicationConfig()
{
    disconnectSocket();
    cleanupFiles();
}

/** componentComplete is executed when the QML component is fully loaded */
void QApplicationConfig::componentComplete()
{
    m_componentCompleted = true;

    if (m_ready == true)    // the component was set to ready before it was completed
    {
        start();
    }
    QQuickItem::componentComplete();
}

QString QApplicationConfig::configUri() const
{
    return m_configUri;
}

bool QApplicationConfig::isReady() const
{
    return m_ready;
}

bool QApplicationConfig::isConnected() const
{
    return m_connected;
}

QApplicationConfigItem *QApplicationConfig::selectedConfig() const
{
    return m_selectedConfig;
}

QApplicationConfigFilter *QApplicationConfig::filter() const
{
    return m_filter;
}

QApplicationConfig::State QApplicationConfig::connectionState() const
{
    return m_connectionState;
}

QApplicationConfig::ConnectionError QApplicationConfig::error() const
{
    return m_error;
}

QString QApplicationConfig::errorString() const
{
    return m_errorString;
}

QQmlListProperty<QApplicationConfigItem> QApplicationConfig::configs()
{
    return QQmlListProperty<QApplicationConfigItem>(this, m_configs);
}

int QApplicationConfig::appConfigCount() const
{
    return m_configs.count();
}

QApplicationConfigItem *QApplicationConfig::appConfig(int index) const
{
    return m_configs.at(index);
}

/** If the ready property has a rising edge we try to connect
 *  if it is has a falling edge we disconnect and cleanup
 */
void QApplicationConfig::setReady(bool arg)
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

void QApplicationConfig::setSelectedConfig(QApplicationConfigItem *arg)
{
    if (m_selectedConfig != arg) {
        m_selectedConfig = arg;
        emit selectedConfigChanged(arg);
    }
}

void QApplicationConfig::setFilter(QApplicationConfigFilter *arg)
{
    if (m_filter == arg)
        return;

    m_filter = arg;
    emit filterChanged(arg);
}

void QApplicationConfig::start()
{
#ifdef QT_DEBUG
    qDebug() << "app config uri:" << m_configUri;
    qDebug() << "filter:" << m_filter->type();
#endif

    m_configs.clear();
    emit configsChanged(QQmlListProperty<QApplicationConfigItem>(this, m_configs));

    if (connectSocket())
    {
        request(pb::MT_LIST_APPLICATIONS);
    }
}

void QApplicationConfig::stop()
{
    // cleanup here
    disconnectSocket();

    updateState(Disconnected);
    updateError(NoError, "");   // clear the error here
}

void QApplicationConfig::updateState(QApplicationConfig::State state)
{
    if (state != m_connectionState)
    {
        m_connectionState = state;
        emit connectionStateChanged(m_connectionState);

        if (m_connectionState == Connected)
        {
            if (m_connected != true) {
                m_connected = true;
                emit connectedChanged(true);
            }
        }
        else if (m_connected != false)
        {
            m_connected = false;
            emit connectedChanged(false);
        }
    }
}

void QApplicationConfig::updateError(QApplicationConfig::ConnectionError error, QString errorString)
{
    if (m_errorString != errorString)
    {
        m_errorString = errorString;
        emit errorStringChanged(m_errorString);
    }

    if (m_error != error)
    {
        m_error = error;
        emit errorChanged(m_error);
    }
}

void QApplicationConfig::pollError(int errorNum, const QString &errorMsg)
{
    QString errorString;
    errorString = QString("Error %1: ").arg(errorNum) + errorMsg;
    updateError(SocketError, errorString);
    updateState(Error);
}

bool QApplicationConfig::connectSocket()
{
    m_context = new PollingZMQContext(this, 1);
    connect(m_context, &PollingZMQContext::pollError,
            this, &QApplicationConfig::pollError);
    m_context->start();

    m_configSocket = m_context->createSocket(ZMQSocket::TYP_DEALER, this);
    m_configSocket->setLinger(0);
    m_configSocket->setIdentity(QString("%1-%2").arg("appconfig").arg(QCoreApplication::applicationPid()).toLocal8Bit());

    try {
        m_configSocket->connectTo(m_configUri);
    }
    catch (const zmq::error_t &e) {
        QString errorString;
        errorString = QString("Error %1: ").arg(e.num()) + QString(e.what());
        updateError(SocketError, errorString);
        updateState(Error);
        return false;
    }

    connect(m_configSocket, &ZMQSocket::messageReceived,
            this, &QApplicationConfig::configMessageReceived);

    return true;
}

void QApplicationConfig::disconnectSocket()
{
    if (m_configSocket != nullptr)
    {
        m_configSocket->close();
        m_configSocket->deleteLater();
        m_configSocket = nullptr;
    }

    if (m_context != nullptr)
    {
        m_context->stop();
        m_context->deleteLater();
        m_context = nullptr;
    }
}

void QApplicationConfig::configMessageReceived(QList<QByteArray> messageList)
{
    m_rx.ParseFromArray(messageList.at(0).data(), messageList.at(0).size());

#ifdef QT_DEBUG
    std::string s;
    gpb::TextFormat::PrintToString(m_rx, &s);
    qDebug() << "server message" << QString::fromStdString(s);
#endif

    if (m_rx.type() == pb::MT_DESCRIBE_APPLICATION) // receive all provided applications
    {
        for (int i = 0; i < m_rx.app_size(); ++i)
        {
            pb::Application app;

            app = m_rx.app(i);

            QApplicationConfigItem::ApplicationType type;
            QString name;
            QString description;

            type = (QApplicationConfigItem::ApplicationType)app.type();
            name = QString::fromStdString(app.name());
            description = QString::fromStdString(app.description());

            if ((m_filter->type() == type)
                 && (m_filter->name().isEmpty() || (name == m_filter->name()))
                 && (m_filter->description().isEmpty() || description.contains(m_filter->description())))
            {
                QApplicationConfigItem *appConfigItem;

                appConfigItem = new QApplicationConfigItem(this);
                appConfigItem->setName(name);
                appConfigItem->setDescription(description);
                appConfigItem->setType(type);
                m_configs.append(appConfigItem);
                emit configsChanged(QQmlListProperty<QApplicationConfigItem>(this, m_configs));
            }
        }

        updateState(Connected); // now we are connected
    }
    else if (m_rx.type() == pb::MT_APPLICATION_DETAIL) // receive the details of an application
    {
        for (int i = 0; i < m_rx.app_size(); ++i)
        {
            pb::Application app;

            app = m_rx.app(i);

            QApplicationConfigItem::ApplicationType type;

            type = (QApplicationConfigItem::ApplicationType)app.type();

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

                baseFilePath = Service::applicationTempPath(m_selectedConfig->name());
                if (!dir.mkpath(baseFilePath))
                {
                    qDebug() << "not able to create directory";
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

                QApplicationDescription applicationDescription;

                applicationDescription.setSourceDir(QUrl("file:///" + baseFilePath));
                // TODO check validity

                m_selectedConfig->setFiles(fileList);
                m_selectedConfig->setMainFile(applicationDescription.mainFile());
                m_selectedConfig->setLoaded(true);
                m_selectedConfig->setLoading(false);
            }
        }
    }
}

void QApplicationConfig::sendConfigMessage(const QByteArray &data)
{
    if (m_configSocket == nullptr) {  // disallow sending messages when not connected
        return;
    }

    try {
        m_configSocket->sendMessage(data);
    }
    catch (const zmq::error_t &e) {
        QString errorString;
        errorString = QString("Error %1: ").arg(e.num()) + QString(e.what());
        updateError(SocketError, errorString);
        updateState(Error);
    }
}

void QApplicationConfig::cleanupFiles()
{
    if (!m_selectedConfig->name().isEmpty())
    {
        QString path = Service::applicationTempPath(m_selectedConfig->name());
        QDir dir(path);
        dir.removeRecursively();
    }
}

void QApplicationConfig::request(pb::ContainerType type)
{
    m_tx.set_type(type);

#ifdef QT_DEBUG
    std::string s;
    gpb::TextFormat::PrintToString(m_tx, &s);
    qDebug() << "request:" << QString::fromStdString(s);
#endif

    sendConfigMessage(QByteArray(m_tx.SerializeAsString().c_str(), m_tx.ByteSize()));
    m_tx.Clear();
}

void QApplicationConfig::selectConfig(QString name)
{
    m_selectedConfig->setLoaded(false);
    m_selectedConfig->setLoading(true);
    m_selectedConfig->setName(name);

    pb::Application *app = m_tx.add_app();

    app->set_name(name.toStdString());
    request(pb::MT_RETRIEVE_APPLICATION);
}

void QApplicationConfig::unselectConfig()
{
    cleanupFiles();

    m_selectedConfig->setName("");
    m_selectedConfig->setDescription("");
    m_selectedConfig->setFiles(QStringList());
    m_selectedConfig->setMainFile(QUrl(""));
    m_selectedConfig->setLoaded(false);
    m_selectedConfig->setLoading(false);
}

void QApplicationConfig::setConfigUri(QString arg)
{
    if (m_configUri != arg) {
        m_configUri = arg;
        emit configUriChanged(arg);
    }
}
