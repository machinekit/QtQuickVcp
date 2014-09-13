#include "qapplicationstatus.h"
#include "debughelper.h"

QApplicationStatus::QApplicationStatus(QQuickItem *parent) :
    QQuickItem(parent),
    m_statusUri(""),
    m_ready(false),
    m_sState(Down),
    m_connected(false),
    m_connectionState(Disconnected),
    m_error(NoError),
    m_errorString(""),
    m_running(false),
    m_synced(false),
    m_channels(MotionChannel | ConfigChannel | IoChannel | TaskChannel | InterpChannel),
    m_componentCompleted(false),
    m_context(NULL),
    m_statusSocket(NULL),
    m_statusHeartbeatTimer(new QTimer(this)),
    m_statusPingOutstanding(false)
{
    connect(m_statusHeartbeatTimer, SIGNAL(timeout()),
            this, SLOT(statusHeartbeatTimerTick()));

    connect(this, SIGNAL(taskChanged(QJsonObject)),
            this, SLOT(updateRunning(QJsonObject)));
    connect(this, SIGNAL(interpChanged(QJsonObject)),
            this, SLOT(updateRunning(QJsonObject)));


    initializeObject(MotionChannel);
    initializeObject(ConfigChannel);
    initializeObject(IoChannel);
    initializeObject(TaskChannel);
    initializeObject(InterpChannel);
}

QApplicationStatus::~QApplicationStatus()
{
    disconnectSockets();
}

/** componentComplete is executed when the QML component is fully loaded */
void QApplicationStatus::componentComplete()
{
    m_componentCompleted = true;

    if (m_ready == true)    // the component was set to ready before it was completed
    {
        start();
    }

    QQuickItem::componentComplete();
}

void QApplicationStatus::start()
{
#ifdef QT_DEBUG
   DEBUG_TAG(1, "status", "start")
#endif
    updateState(Connecting);


    if (connectSockets())
    {
        subscribe();
    }
}

void QApplicationStatus::stop()
{
#ifdef QT_DEBUG
    DEBUG_TAG(1, "status", "stop")
#endif

    // cleanup here
    stopStatusHeartbeat();
    disconnectSockets();

    updateState(Disconnected);
    updateError(NoError, "");   // clear the error here
}

void QApplicationStatus::startStatusHeartbeat(int interval)
{
    m_statusHeartbeatTimer->stop();
    m_statusPingOutstanding = false;

    if (interval > 0)
    {
        m_statusHeartbeatTimer->setInterval(interval);
        m_statusHeartbeatTimer->start();
    }
}

void QApplicationStatus::stopStatusHeartbeat()
{
    m_statusHeartbeatTimer->stop();
}

void QApplicationStatus::refreshStatusHeartbeat()
{
    if (m_statusHeartbeatTimer->isActive())
    {
        m_statusHeartbeatTimer->stop();
        m_statusHeartbeatTimer->start();
    }
}

void QApplicationStatus::updateState(QApplicationStatus::State state)
{
    if (state != m_connectionState)
    {
        if (m_connectionState == Connected) // we are not connected anymore
        {
            stopStatusHeartbeat();
            clearSync();
            if (m_connected != false) {
                m_connected = false;
                emit connectedChanged(false);
            }
            initializeObject(MotionChannel);
            initializeObject(ConfigChannel);
            initializeObject(IoChannel);
            initializeObject(TaskChannel);
            initializeObject(InterpChannel);
        }
        else if (m_connected != true) {
            m_connected = true;
            emit connectedChanged(true);
        }

        m_connectionState = state;
        emit connectionStateChanged(m_connectionState);
    }
}

void QApplicationStatus::updateError(QApplicationStatus::ConnectionError error, const QString &errorString)
{
    m_error = error;
    m_errorString = errorString;

    emit errorStringChanged(m_errorString);
    emit errorChanged(m_error);
}

void QApplicationStatus::updateSync(QApplicationStatus::StatusChannel channel)
{
    m_syncedChannels |= channel;

    if (m_syncedChannels == m_channels) {
        m_synced = true;
        emit syncedChanged(m_synced);
    }
}

void QApplicationStatus::clearSync()
{
    m_synced = false;
    m_syncedChannels = 0;
    emit syncedChanged(m_synced);
}

void QApplicationStatus::updateMotion(const pb::EmcStatusMotion &motion)
{
    recurseMessage(motion, &m_motion);
    emit motionChanged(m_motion);
}

void QApplicationStatus::updateConfig(const pb::EmcStatusConfig &config)
{
    recurseMessage(config, &m_config);
    emit configChanged(m_config);
}

void QApplicationStatus::updateIo(const pb::EmcStatusIo &io)
{
    recurseMessage(io, &m_io);
    emit ioChanged(m_io);
}

void QApplicationStatus::updateTask(const pb::EmcStatusTask &task)
{
    recurseMessage(task, &m_task);
    emit taskChanged(m_task);
}

void QApplicationStatus::updateInterp(const pb::EmcStatusInterp &interp)
{
    recurseMessage(interp, &m_interp);
    emit interpChanged(m_interp);
}

QString QApplicationStatus::enumNameToCamelCase(const QString &name)
{
    QStringList partList;

    partList = name.toLower().split('_');

    for (int i = 0; i < partList.size(); ++i)
    {
        partList[i][0] = partList[i][0].toUpper();
    }

    return partList.join("");
}

void QApplicationStatus::recurseDescriptor(const google::protobuf::Descriptor *descriptor, QJsonObject *object)
{
    for (int i = 0; i < descriptor->field_count(); ++i)
    {
        const gpb::FieldDescriptor *field;
        QString name;
        QJsonValue jsonValue;

        field = descriptor->field(i);
        name = QString::fromStdString(field->camelcase_name());

        switch (field->cpp_type())
        {
        case gpb::FieldDescriptor::CPPTYPE_BOOL:
            jsonValue = false;
            break;
        case gpb::FieldDescriptor::CPPTYPE_DOUBLE:
        case gpb::FieldDescriptor::CPPTYPE_FLOAT:
            jsonValue = 0.0;
            break;
        case gpb::FieldDescriptor::CPPTYPE_INT32:
        case gpb::FieldDescriptor::CPPTYPE_INT64:
        case gpb::FieldDescriptor::CPPTYPE_UINT32:
        case gpb::FieldDescriptor::CPPTYPE_UINT64:
            jsonValue = 0;
            break;
        case gpb::FieldDescriptor::CPPTYPE_STRING:
            jsonValue = "";
            break;
        case gpb::FieldDescriptor::CPPTYPE_ENUM:
            jsonValue = field->enum_type()->value(0)->number();
            //jsonValue = enumNameToCamelCase(QString::fromStdString(field->enum_type()->value(0)->name()));
            break;
        case gpb::FieldDescriptor::CPPTYPE_MESSAGE:
            QJsonObject jsonObject;
            recurseDescriptor(field->message_type(), &jsonObject);
            jsonValue = jsonObject;
            break;
        }

        if (field->is_repeated())
        {
            QJsonArray jsonArray;
            QJsonObject jsonObject = jsonValue.toObject();

            jsonObject.remove("index");
            if (jsonObject.count() == 1)
            {
                jsonValue = jsonObject.value(jsonObject.keys().at(0));
            }
            else
            {
                jsonValue = jsonObject;
            }
            jsonArray.append(jsonValue);
            object->insert(name, jsonArray);
        }
        else
        {
            object->insert(name, jsonValue);
        }
    }
}

void QApplicationStatus::recurseMessage(const google::protobuf::Message &message, QJsonObject *object)
{
    const gpb::Reflection *reflection = message.GetReflection();
    gpb::vector< const gpb::FieldDescriptor * > output;
    reflection->ListFields(message, &output);

    for (int i = 0; i < (int)output.size(); ++i)
    {
        QString name;
        QJsonValue jsonValue;
        const gpb::FieldDescriptor *field;

        field = output[i];
        name = QString::fromStdString(field->camelcase_name());

        if (!field->is_repeated())
        {
            switch (field->cpp_type())
            {
            case gpb::FieldDescriptor::CPPTYPE_BOOL:
                jsonValue = reflection->GetBool(message, field);
                break;
            case gpb::FieldDescriptor::CPPTYPE_DOUBLE:
                jsonValue = reflection->GetDouble(message, field);
                break;
            case gpb::FieldDescriptor::CPPTYPE_FLOAT:
                jsonValue = (double)reflection->GetFloat(message, field);
                break;
            case gpb::FieldDescriptor::CPPTYPE_INT32:
                jsonValue = (int)reflection->GetInt32(message, field);
                break;
            case gpb::FieldDescriptor::CPPTYPE_INT64:
                jsonValue = (int)reflection->GetInt64(message, field);
                break;
            case gpb::FieldDescriptor::CPPTYPE_UINT32:
                jsonValue = (int)reflection->GetUInt32(message, field);
                break;
            case gpb::FieldDescriptor::CPPTYPE_UINT64:
                jsonValue = (int)reflection->GetUInt64(message, field);
                break;
            case gpb::FieldDescriptor::CPPTYPE_STRING:
                jsonValue = QString::fromStdString(reflection->GetString(message, field));
                break;
            case gpb::FieldDescriptor::CPPTYPE_ENUM:
                jsonValue = reflection->GetEnum(message, field)->number();
                //jsonValue = enumNameToCamelCase(QString::fromStdString(reflection->GetEnum(message, field)->name()));
                break;
            case gpb::FieldDescriptor::CPPTYPE_MESSAGE:
                QJsonObject jsonObject = object->value(name).toObject();
                recurseMessage(reflection->GetMessage(message, field), &jsonObject);
                jsonValue = jsonObject;
                break;
            }
            object->insert(name, jsonValue);
        }
        else
        {
            if (field->cpp_type() == gpb::FieldDescriptor::CPPTYPE_MESSAGE)
            {
                QJsonArray jsonArray = object->value(name).toArray();
                for (int j = 0; j < reflection->FieldSize(message, field); ++j)
                {
                    QJsonObject jsonObject;
                    QJsonValue jsonValue;
                    const gpb::Message &subMessage = reflection->GetRepeatedMessage(message, field, j);
                    const gpb::Descriptor *subDescriptor = subMessage.GetDescriptor();
                    const gpb::FieldDescriptor *subField = subDescriptor->FindFieldByName("index");
                    const gpb::Reflection *subReflection = subMessage.GetReflection();
                    int index = subReflection->GetInt32(subMessage, subField);

                    while (jsonArray.size() < (index + 1))
                    {
                        jsonArray.append(QJsonValue());
                    }

                    if (subDescriptor->field_count() == 2)  // index and value field
                    {
                        recurseMessage(subMessage, &jsonObject);
                        jsonObject.remove("index");
                        jsonValue = jsonObject.value(jsonObject.keys().at(0));
                    }
                    else
                    {
                        jsonObject = jsonArray.at(index).toObject(QJsonObject());
                        recurseMessage(subMessage, &jsonObject);
                        jsonObject.remove("index");
                        jsonValue = jsonObject;
                    }

                    jsonArray.replace(index, jsonValue);
                }
                object->insert(name, QJsonValue(jsonArray));
            }
        }
    }
}

void QApplicationStatus::statusMessageReceived(const QList<QByteArray> &messageList)
{
    QByteArray topic;

    topic = messageList.at(0);
    m_rx.ParseFromArray(messageList.at(1).data(), messageList.at(1).size());

#ifdef QT_DEBUG
    std::string s;
    gpb::TextFormat::PrintToString(m_rx, &s);
    DEBUG_TAG(3, "status", "update" << topic << QString::fromStdString(s))
#endif

    if ((m_rx.type() == pb::MT_EMCSTAT_FULL_UPDATE)
        || (m_rx.type() == pb::MT_EMCSTAT_INCREMENTAL_UPDATE))
    {
        if ((topic == "motion") && m_rx.has_emc_status_motion()) {
            updateMotion(m_rx.emc_status_motion());
            if (m_rx.type() == pb::MT_EMCSTAT_FULL_UPDATE) {
                updateSync(MotionChannel);
            }
        }

        if ((topic == "config") && m_rx.has_emc_status_config()) {
            updateConfig(m_rx.emc_status_config());
            if (m_rx.type() == pb::MT_EMCSTAT_FULL_UPDATE) {
                updateSync(ConfigChannel);
            }
        }

        if ((topic == "io") && m_rx.has_emc_status_io()) {
            updateIo(m_rx.emc_status_io());
            if (m_rx.type() == pb::MT_EMCSTAT_FULL_UPDATE) {
                updateSync(IoChannel);
            }
        }

        if ((topic == "task") && m_rx.has_emc_status_task()) {
            updateTask(m_rx.emc_status_task());
            if (m_rx.type() == pb::MT_EMCSTAT_FULL_UPDATE) {
                updateSync(TaskChannel);
            }
        }

        if ((topic == "interp") && m_rx.has_emc_status_interp()) {
            updateInterp(m_rx.emc_status_interp());
            if (m_rx.type() == pb::MT_EMCSTAT_FULL_UPDATE) {
                updateSync(InterpChannel);
            }
        }

        if (m_sState != Up)
        {
            m_sState = Up;
            updateError(NoError, "");
            updateState(Connected);
        }

        if (m_rx.type() == pb::MT_EMCSTAT_FULL_UPDATE)
        {
            if (m_rx.has_pparams())
            {
                pb::ProtocolParameters pparams = m_rx.pparams();
                startStatusHeartbeat(pparams.keepalive_timer() * 2); // wait double the time of the hearbeat interval
            }
        }
        else
        {
            refreshStatusHeartbeat();
        }

        return;
    }
    else if (m_rx.type() == pb::MT_PING)
    {
        refreshStatusHeartbeat();

        return;
    }

#ifdef QT_DEBUG
    gpb::TextFormat::PrintToString(m_rx, &s);
    DEBUG_TAG(1, "status", "update: unknown message type: " << QString::fromStdString(s))
#endif
}

void QApplicationStatus::pollError(int errorNum, const QString &errorMsg)
{
    QString errorString;
    errorString = QString("Error %1: ").arg(errorNum) + errorMsg;
    updateError(SocketError, errorString);
    updateState(Error);
}

void QApplicationStatus::statusHeartbeatTimerTick()
{
    unsubscribe();
    updateError(TimeoutError, "Status service timed out");
    updateState(Error);

#ifdef QT_DEBUG
    DEBUG_TAG(1, "status", "timeout")
#endif

    subscribe();    // trigger a fresh subscribe

    m_statusPingOutstanding = true;
}

/** Connects the 0MQ sockets */
bool QApplicationStatus::connectSockets()
{
    m_context = new PollingZMQContext(this, 1);
    connect(m_context, SIGNAL(pollError(int,QString)),
            this, SLOT(pollError(int,QString)));
    m_context->start();

    m_statusSocket = m_context->createSocket(ZMQSocket::TYP_SUB, this);
    m_statusSocket->setLinger(0);

    try {
        m_statusSocket->connectTo(m_statusUri);
    }
    catch (zmq::error_t e) {
        QString errorString;
        errorString = QString("Error %1: ").arg(e.num()) + QString(e.what());
        updateError(SocketError, errorString);
        updateState(Error);
        return false;
    }

    connect(m_statusSocket, SIGNAL(messageReceived(QList<QByteArray>)),
            this, SLOT(statusMessageReceived(QList<QByteArray>)));

#ifdef QT_DEBUG
    DEBUG_TAG(1, "status", "socket connected" << m_statusUri)
#endif

    return true;
}

/** Disconnects the 0MQ sockets */
void QApplicationStatus::disconnectSockets()
{
    if (m_statusSocket != NULL)
    {
        m_statusSocket->close();
        m_statusSocket->deleteLater();
        m_statusSocket = NULL;
    }

    if (m_context != NULL)
    {
        m_context->stop();
        m_context->deleteLater();
        m_context = NULL;
    }
}

void QApplicationStatus::subscribe()
{
    m_sState = Trying;

    if (m_channels | MotionChannel) {
        m_statusSocket->subscribeTo("motion");
        m_subscriptions.append("motion");
    }
    if (m_channels | ConfigChannel) {
        m_statusSocket->subscribeTo("config");
        m_subscriptions.append("config");
    }
    if (m_channels | TaskChannel) {
        m_statusSocket->subscribeTo("task");
        m_subscriptions.append("task");
    }
    if (m_channels | IoChannel) {
        m_statusSocket->subscribeTo("io");
        m_subscriptions.append("io");
    }
    if (m_channels | InterpChannel) {
        m_statusSocket->subscribeTo("interp");
        m_subscriptions.append("interp");
    }
}

void QApplicationStatus::unsubscribe()
{
    m_sState = Down;
    foreach (QString subscription, m_subscriptions)
    {
        m_statusSocket->unsubscribeFrom(subscription);

        if (subscription == "motion") {
            initializeObject(MotionChannel);
        }
        else if (subscription == "config") {
            initializeObject(ConfigChannel);
        }
        else if (subscription == "io") {
            initializeObject(IoChannel);
        }
        else if (subscription == "task") {
            initializeObject(TaskChannel);
        }
        else if (subscription == "interp") {
            initializeObject(InterpChannel);
        }
    }
    m_subscriptions.clear();
}

void QApplicationStatus::updateRunning(const QJsonObject &object)
{
    Q_UNUSED(object)

    bool running;

    running = m_task["taskMode"].isDouble()
            && (((TaskMode)m_task["taskMode"].toDouble() == TaskModeAuto)
            || ((TaskMode)m_task["taskMode"].toDouble() == TaskModeMdi))
            && m_interp["interpState"].isDouble()
            && ((InterpreterState)m_interp["interpState"].toDouble() != InterpreterIdle);

    if (running != m_running)
    {
        m_running = running;
        emit runningChanged(running);
    }
}

/** If the ready property has a rising edge we try to connect
 *  if it is has a falling edge we disconnect and cleanup
 */
void QApplicationStatus::setReady(bool arg)
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

void QApplicationStatus::initializeObject(QApplicationStatus::StatusChannel channel)
{
    switch (channel)
    {
    case MotionChannel:
        m_motion = QJsonObject();
        recurseDescriptor(statusMotion.GetDescriptor(), &m_motion);
        emit motionChanged(m_motion);
        return;
    case ConfigChannel:
        m_config = QJsonObject();
        recurseDescriptor(statusConfig.GetDescriptor(), &m_config);
        emit configChanged(m_config);
        return;
    case IoChannel:
        m_io = QJsonObject();
        recurseDescriptor(statusIo.GetDescriptor(), &m_io);
        emit ioChanged(m_io);
        return;
    case TaskChannel:
        m_task = QJsonObject();
        recurseDescriptor(statusTask.GetDescriptor(), &m_task);
        emit taskChanged(m_task);
        return;
    case InterpChannel:
        m_interp = QJsonObject();
        recurseDescriptor(statusInterp.GetDescriptor(), &m_interp);
        emit interpChanged(m_interp);
        return;
    }
}
