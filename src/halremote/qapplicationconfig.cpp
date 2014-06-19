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
            filters: [ ApplicationConfigFilter { type: ApplicationConfigItem.Qt5QmlApplication } ]
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

/*! \qmlproperty list<ApplicationConfigFilter> ApplicationConfig::filters

    This property holds a list of filters applied to the available configurations.
    The filters are OR connected.
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
     m_connectionState(Disconnected),
     m_error(NoError),
     m_errorString(""),
     m_selectedConfig(new QApplicationConfigItem(this)),
     m_context(NULL),
     m_configSocket(NULL)
{
}

QApplicationConfig::~QApplicationConfig()
{
    disconnectSocket();
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

QQmlListProperty<QApplicationConfigFilter> QApplicationConfig::filters()
{
    return QQmlListProperty<QApplicationConfigFilter>(this, m_filters);
}

int QApplicationConfig::filterCount() const
{
    return m_filters.count();
}

QApplicationConfigFilter *QApplicationConfig::filter(int index) const
{
    return m_filters.at(index);
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

void QApplicationConfig::start()
{
#ifdef QT_DEBUG
    qDebug() << "app config uri:" << m_configUri;
    foreach (QApplicationConfigFilter* filter, m_filters)
    {
        qDebug() << "filter:" << filter->type();
    }
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
    }
}

void QApplicationConfig::updateError(QApplicationConfig::ConnectionError error, QString errorString)
{
    m_error = error;
    m_errorString = errorString;

    emit errorStringChanged(m_errorString);
    emit errorChanged(m_error);
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
    connect(m_context, SIGNAL(pollError(int,QString)),
            this, SLOT(pollError(int,QString)));
    m_context->start();

    m_configSocket = m_context->createSocket(ZMQSocket::TYP_DEALER, this);
    m_configSocket->setLinger(0);
    m_configSocket->setIdentity(QString("%1-%2").arg("appconfig").arg(QCoreApplication::applicationPid()).toLocal8Bit());

    try {
        m_configSocket->connectTo(m_configUri);
    }
    catch (zmq::error_t e) {
        QString errorString;
        errorString = QString("Error %1: ").arg(e.num()) + QString(e.what());
        updateError(SocketError, errorString);
        updateState(Error);
        return false;
    }

    connect(m_configSocket, SIGNAL(messageReceived(QList<QByteArray>)),
            this, SLOT(configMessageReceived(QList<QByteArray>)));

    updateState(Connected);

    return true;
}

void QApplicationConfig::disconnectSocket()
{
    if (m_configSocket != NULL)
    {
        m_configSocket->close();
        m_configSocket->deleteLater();
        m_configSocket = NULL;
    }

    if (m_context != NULL)
    {
        m_context->stop();
        m_context->deleteLater();
        m_context = NULL;
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

            foreach (QApplicationConfigFilter *filter, m_filters)
            {
                if ((pb::ApplicationType)filter->type() == app.type())
                {
                    QApplicationConfigItem *appConfigItem;

                    appConfigItem = new QApplicationConfigItem(this);
                    appConfigItem->setName(QString::fromStdString(app.name()));
                    appConfigItem->setDescription(QString::fromStdString(app.description()));
                    appConfigItem->setType((QApplicationConfigItem::ApplicationType)app.type());
                    m_configs.append(appConfigItem);
                    emit configsChanged(QQmlListProperty<QApplicationConfigItem>(this, m_configs));

                    break;
                }
            }
        }
    }
    else if (m_rx.type() == pb::MT_APPLICATION_DETAIL) // receive the details of an application
    {
        for (int i = 0; i < m_rx.app_size(); ++i)
        {
            pb::Application app;

            app = m_rx.app(i);

            foreach (QApplicationConfigFilter *filter, m_filters)
            {
                if ((pb::ApplicationType)filter->type() == app.type())
                {
                    QString baseFilePath;
                    QStringList fileList;
                    QDir dir;

                    if (m_selectedConfig == NULL)
                    {
                        return;
                    }

                    m_selectedConfig->setName(QString::fromStdString(app.name()));
                    m_selectedConfig->setDescription(QString::fromStdString(app.description()));
                    m_selectedConfig->setType((QApplicationConfigItem::ApplicationType)app.type());

                    baseFilePath = QDir::tempPath() + "/machinekit/" + m_selectedConfig->name() + "/";
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
                        QFile localFile(filePath);
                        if (!localFile.open(QIODevice::WriteOnly))
                        {
                            qDebug() << "not able to create file" << filePath;
                            continue;
                        }

                        data = QByteArray(file.blob().data(), file.blob().size());//QString::fromStdString(file.blob()).toLocal8Bit();

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

                        //QByteArray testData =  qCompress(data);
                        //quint32 test = ((quint32)testData.at(0) << 24) + ((quint32)testData.at(1) << 16) + ((quint32)testData.at(2) << 8) + ((quint32)testData.at(3) << 0);
                        //qDebug() << test << (quint8)testData.at(0) << (quint8)testData.at(1) << (quint8)testData.at(2) << (quint8)testData.at(3);

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
                }
            }
        }
    }
}

void QApplicationConfig::sendConfigMessage(const QByteArray &data)
{
    try {
        m_configSocket->sendMessage(data);
    }
    catch (zmq::error_t e) {
        QString errorString;
        errorString = QString("Error %1: ").arg(e.num()) + QString(e.what());
        updateError(SocketError, errorString);
        updateState(Error);
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

    pb::Application *app = m_tx.add_app();

    app->set_name(name.toStdString());
    request(pb::MT_RETRIEVE_APPLICATION);
}

void QApplicationConfig::unselectConfig()
{
    m_selectedConfig->setName("");
    m_selectedConfig->setDescription("");
    m_selectedConfig->setFiles(QStringList());
    m_selectedConfig->setMainFile(QUrl(""));
    m_selectedConfig->setLoaded(false);
}
