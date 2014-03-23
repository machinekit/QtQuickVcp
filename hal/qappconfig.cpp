#include "qappconfig.h"

QAppConfig::QAppConfig(QQuickItem *parent) :
    QQuickItem(parent)
{
    m_componentCompleted = false;
    m_uri = "";
    m_ready = false;
    m_timeout = 3000;
    m_selectedConfig = NULL;

    m_context = createDefaultContext(this);

    m_context->start();

    m_configSocket = m_context->createSocket(ZMQSocket::TYP_DEALER, this);
    m_configSocket->setLinger(0);

    m_timeoutTimer = new QTimer(this);
    connect(m_timeoutTimer, SIGNAL(timeout()),
            this, SLOT(timeoutTimerTick()));
}

/** componentComplete is executed when the QML component is fully loaded */
void QAppConfig::componentComplete()
{
    m_componentCompleted = true;

    if (m_ready == true)    // the component was set to ready before it was completed
    {
        start();
    }
    QQuickItem::componentComplete();
}

QQmlListProperty<QAppConfigItem> QAppConfig::appConfigs()
{
    return QQmlListProperty<QAppConfigItem>(this, m_appConfigs);
}

int QAppConfig::appConfigCount() const
{
    return m_appConfigs.count();
}

QAppConfigItem *QAppConfig::appConfig(int index) const
{
    return m_appConfigs.at(index);
}

QQmlListProperty<QAppConfigFilter> QAppConfig::filters()
{
    return QQmlListProperty<QAppConfigFilter>(this, m_filters);
}

int QAppConfig::filterCount() const
{
    return m_filters.count();
}

QAppConfigFilter *QAppConfig::filter(int index) const
{
    return m_filters.at(index);
}

/** If the ready property has a rising edge we try to connect
 *  if it is has a falling edge we disconnect and cleanup
 */
void QAppConfig::setReady(bool arg)
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

void QAppConfig::start()
{
#ifdef QT_DEBUG
    qDebug() << "app config uri:" << m_uri;
    foreach (QAppConfigFilter* filter, m_filters)
    {
        qDebug() << "filter:" << filter->type();
    }
#endif

    connectSocket();
    m_appConfigs.clear();
    emit appConfigsChanged(QQmlListProperty<QAppConfigItem>(this, m_appConfigs));
    request(pb::MT_LIST_APPLICATIONS);
}

void QAppConfig::stop()
{
    // cleanup here
    disconnectSocket();
}

void QAppConfig::timeoutTimerTick()
{
}

void QAppConfig::connectSocket()
{
    m_configSocket->setIdentity(QString("%1-%2").arg("appconfig").arg(QCoreApplication::applicationPid()).toLocal8Bit());

    m_configSocket->connectTo(m_uri);

    connect(m_configSocket, SIGNAL(messageReceived(QList<QByteArray>)),
            this, SLOT(configMessageReceived(QList<QByteArray>)));
}

void QAppConfig::disconnectSocket()
{
    disconnect(m_configSocket, SIGNAL(messageReceived(QList<QByteArray>)),
            this, SLOT(configMessageReceived(QList<QByteArray>)));
}

void QAppConfig::configMessageReceived(QList<QByteArray> messageList)
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

            foreach (QAppConfigFilter *filter, m_filters)
            {
                if ((pb::ApplicationType)filter->type() == app.type())
                {
                    QAppConfigItem *appConfigItem;

                    appConfigItem = new QAppConfigItem(this);
                    appConfigItem->setName(QString::fromStdString(app.name()));
                    appConfigItem->setDescription(QString::fromStdString(app.description()));
                    appConfigItem->setType((QAppConfigItem::ApplicationType)app.type());
                    m_appConfigs.append(appConfigItem);
                    emit appConfigsChanged(QQmlListProperty<QAppConfigItem>(this, m_appConfigs));

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

            foreach (QAppConfigFilter *filter, m_filters)
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
                    m_selectedConfig->setType((QAppConfigItem::ApplicationType)app.type());

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

                        data = QString::fromStdString(file.blob()).toLocal8Bit();

                        if (file.encoding() == pb::ZLIB)
                        {
                            quint32 test = ((quint32)data.at(0) << 24) + ((quint32)data.at(1) << 16) + ((quint32)data.at(2) << 8) + ((quint32)data.at(3) << 0);
                            qDebug() << test << (quint8)data.at(0) << (quint8)data.at(1) << (quint8)data.at(2) << (quint8)data.at(3);
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

                    m_selectedConfig->setFiles(fileList);
                    m_selectedConfig->setMainFile(QUrl("file:///" + baseFilePath + "main.qml"));  //TODO: this should not be hardcoded
                    m_selectedConfig->setLoaded(true);
                }
            }
        }
    }
}

void QAppConfig::request(pb::ContainerType type)
{
    m_tx.set_type(type);

#ifdef QT_DEBUG
    std::string s;
    gpb::TextFormat::PrintToString(m_tx, &s);
    qDebug() << "request:" << QString::fromStdString(s);
#endif

    m_configSocket->sendMessage(QByteArray(m_tx.SerializeAsString().c_str(), m_tx.ByteSize()));
    m_tx.Clear();
}

void QAppConfig::selectAppConfig(QString name)
{
    pb::Application *app = m_tx.add_app();

    app->set_name(name.toStdString());
    request(pb::MT_RETRIEVE_APPLICATION);
}

void QAppConfig::unselectAppConfig()
{
    if (m_selectedConfig == NULL)
    {
        return;
    }

    m_selectedConfig->setName("");
    m_selectedConfig->setDescription("");
    m_selectedConfig->setFiles(QStringList());
    m_selectedConfig->setMainFile(QUrl(""));
    m_selectedConfig->setLoaded(false);
}
