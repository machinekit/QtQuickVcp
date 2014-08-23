#include "qemcstatus.h"
#include "debughelper.h"

QEmcStatus::QEmcStatus(QQuickItem *parent) :
    QQuickItem(parent),
    m_statusUri(""),
    m_ready(false),
    m_sState(Down),
    m_connectionState(Disconnected),
    m_error(NoError),
    m_errorString(""),
    m_channels(MotionChannel | ConfigChannel | IoChannel | TaskChannel | InterpChannel),
    m_componentCompleted(false),
    m_context(NULL),
    m_statusSocket(NULL),
    m_statusHeartbeatTimer(new QTimer(this)),
    m_statusPingOutstanding(false)
{
    connect(m_statusHeartbeatTimer, SIGNAL(timeout()),
            this, SLOT(statusHeartbeatTimerTick()));
}

QEmcStatus::~QEmcStatus()
{
    disconnectSockets();
}

/** componentComplete is executed when the QML component is fully loaded */
void QEmcStatus::componentComplete()
{
    m_componentCompleted = true;

    if (m_ready == true)    // the component was set to ready before it was completed
    {
        start();
    }

    QQuickItem::componentComplete();
}

void QEmcStatus::start()
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

void QEmcStatus::stop()
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

void QEmcStatus::startStatusHeartbeat(int interval)
{
    m_statusHeartbeatTimer->stop();
    m_statusPingOutstanding = false;

    if (interval > 0)
    {
        m_statusHeartbeatTimer->setInterval(interval);
        m_statusHeartbeatTimer->start();
    }
}

void QEmcStatus::stopStatusHeartbeat()
{
    m_statusHeartbeatTimer->stop();
}

void QEmcStatus::refreshStatusHeartbeat()
{
    if (m_statusHeartbeatTimer->isActive())
    {
        m_statusHeartbeatTimer->stop();
        m_statusHeartbeatTimer->start();
    }
}

void QEmcStatus::updateState(QEmcStatus::State state)
{
    if (state != m_connectionState)
    {
        if (m_connectionState == Connected) // we are not connected anymore
        {
            stopStatusHeartbeat();
            clearObject(MotionChannel);
            clearObject(ConfigChannel);
            clearObject(IoChannel);
            clearObject(TaskChannel);
            clearObject(InterpChannel);
        }

        m_connectionState = state;
        emit connectionStateChanged(m_connectionState);
    }
}

void QEmcStatus::updateError(QEmcStatus::ConnectionError error, const QString &errorString)
{
    m_error = error;
    m_errorString = errorString;

    emit errorStringChanged(m_errorString);
    emit errorChanged(m_error);
}

void QEmcStatus::updatePosition(QJsonObject *object, const QString &baseName, const pb::Position &position)
{
    QJsonObject jsonObject;

    if (!object->value(baseName).isUndefined())
    {
        jsonObject = object->value(baseName).toObject();
    }

    if (position.has_x())
    {
        jsonObject["x"] = (double)position.x();
    }

    if (position.has_y())
    {
        jsonObject["y"] = (double)position.y();
    }

    if (position.has_z())
    {
        jsonObject["z"] = (double)position.z();
    }

    if (position.has_a())
    {
        jsonObject["a"] = (double)position.a();
    }

    if (position.has_b())
    {
        jsonObject["b"] = (double)position.b();
    }

    if (position.has_c())
    {
        jsonObject["c"] = (double)position.c();
    }

    if (position.has_u())
    {
        jsonObject["u"] = (double)position.u();
    }

    if (position.has_v())
    {
        jsonObject["v"] = (double)position.v();
    }

    if (position.has_w())
    {
        jsonObject["w"] = (double)position.w();
    }

    (*object)[baseName] = jsonObject;
}

void QEmcStatus::updateMessage(QJsonObject *jsonObject, const pb::EmcStatusMotionAxis &axis)
{
    if (axis.has_enabled()) {
        (*jsonObject)["enabled"] = (bool)axis.enabled();
    }

    if (axis.has_fault()) {
        (*jsonObject)["fault"] = (bool)axis.fault();
    }

    if (axis.has_ferror_current()) {
        (*jsonObject)["ferrorCurrent"] = (double)axis.ferror_current();
    }

    if (axis.has_ferror_highmark()) {
        (*jsonObject)["ferrorHighmark"] = (double)axis.ferror_highmark();
    }

    if (axis.has_homed()) {
        (*jsonObject)["homed"] = (bool)axis.homed();
    }

    if (axis.has_homing()) {
        (*jsonObject)["homing"] = (bool)axis.homing();
    }

    if (axis.has_inpos()) {
        (*jsonObject)["inpos"] = (bool)axis.inpos();
    }

    if (axis.has_input()) {
        (*jsonObject)["input"] = (double)axis.input();
    }

    if (axis.has_max_hard_limit()) {
        (*jsonObject)["maxHardLimit"] = (bool)axis.max_hard_limit();
    }

    if (axis.has_max_soft_limit()) {
        (*jsonObject)["maxSoftLimit"] = (bool)axis.max_soft_limit();
    }

    if (axis.has_min_hard_limit()) {
        (*jsonObject)["minHardLimit"] = (bool)axis.min_hard_limit();
    }

    if (axis.has_min_soft_limit()) {
        (*jsonObject)["minSoftLimit"] = (bool)axis.min_soft_limit();
    }

    if (axis.has_output()) {
        (*jsonObject)["output"] = (double)axis.output();
    }

    if (axis.has_override_limits()) {
        (*jsonObject)["overrideLimits"] = (bool)axis.override_limits();
    }

    if (axis.has_velocity()) {
        (*jsonObject)["velocity"] = (double)axis.velocity();
    }
}

void QEmcStatus::updateMessage(QJsonObject *jsonObject, const pb::EmcStatusConfigAxis &axis)
{
    if (axis.has_axistype()) {
        (*jsonObject)["axisType"] = (int)axis.axistype();
    }

    if (axis.has_backlash()) {
        (*jsonObject)["backlash"] = (double)axis.backlash();
    }

    if (axis.has_max_ferror()) {
        (*jsonObject)["maxFerror"] = (double)axis.max_ferror();
    }

    if (axis.has_max_position_limit()) {
        (*jsonObject)["maxPositionLimit"] = (double)axis.max_position_limit();
    }

    if (axis.has_min_ferror()) {
        (*jsonObject)["minFerror"] = (double)axis.min_ferror();
    }

    if (axis.has_min_position_limit()) {
        (*jsonObject)["minPositionLimit"] = (double)axis.min_position_limit();
    }

    if (axis.has_units()) {
        (*jsonObject)["units"] = (double)axis.units();
    }
}

void QEmcStatus::updateMessage(QJsonObject *jsonObject, const pb::EmcToolData &toolData)
{
    if (toolData.has_id()) {
        (*jsonObject)["id"] = (int)toolData.id();
    }

    if (toolData.has_xoffset()) {
        (*jsonObject)["xOffset"] = (double)toolData.xoffset();
    }

    if (toolData.has_yoffset()) {
        (*jsonObject)["yOffset"] = (double)toolData.yoffset();
    }

    if (toolData.has_zoffset()) {
        (*jsonObject)["zOffset"] = (double)toolData.zoffset();
    }

    if (toolData.has_aoffset()) {
        (*jsonObject)["aOffset"] = (double)toolData.aoffset();
    }

    if (toolData.has_boffset()) {
        (*jsonObject)["bOffset"] = (double)toolData.boffset();
    }

    if (toolData.has_coffset()) {
        (*jsonObject)["cOffset"] = (double)toolData.coffset();
    }

    if (toolData.has_uoffset()) {
        (*jsonObject)["uOffset"] = (double)toolData.uoffset();
    }

    if (toolData.has_voffset()) {
        (*jsonObject)["vOffset"] = (double)toolData.voffset();
    }

    if (toolData.has_woffset()) {
        (*jsonObject)["wOffset"] = (double)toolData.woffset();
    }

    if (toolData.has_diameter()) {
        (*jsonObject)["diameter"] = (double)toolData.diameter();
    }

    if (toolData.has_frontangle()) {
        (*jsonObject)["frontangle"] = (double)toolData.frontangle();
    }

    if (toolData.has_backangle()) {
        (*jsonObject)["backangle"] = (double)toolData.backangle();
    }

    if (toolData.has_orientation()) {
        (*jsonObject)["orientation"] = (double)toolData.orientation();
    }
}

template<typename ValueType, class Type>
void QEmcStatus::updateIndexValue(QJsonObject *object, const QString &baseName, const gpb::RepeatedPtrField<Type> &fields)
{
    QJsonArray jsonArray;

    if (!(*object).value(baseName).isUndefined())
    {
        jsonArray = (*object).value(baseName).toArray();
    }

    for (int i = 0; i < fields.size(); ++i)
    {
        Type field = fields.Get(i);

        if (field.has_value())
        {
            if (field.index() == jsonArray.size())
            {
                jsonArray.append((ValueType)field.value());
            }
            else
            {
                jsonArray[field.index()] = (ValueType)field.value();
            }
        }
    }

    (*object)[baseName] = jsonArray;
}
template<typename ValueType, class Type>
void QEmcStatus::updateIndexMessage(QJsonObject *object, const QString &baseName, const gpb::RepeatedPtrField<Type> &fields)
{
    QJsonArray jsonArray;

    if (!(*object).value(baseName).isUndefined())
    {
        jsonArray = (*object).value(baseName).toArray();
    }

    for (int i = 0; i < fields.size(); ++i)
    {
        QJsonObject jsonObject;
        Type field = fields.Get(i);

        if (field.index() < jsonArray.size()) {
            jsonObject = jsonArray.at(field.index()).toObject();
        }

        updateMessage(&jsonObject, field);

        if (field.index() == jsonArray.size()) {
            jsonArray.append(jsonObject);
        }
        else
        {
            jsonArray[field.index()] = jsonObject;
        }
    }

    (*object)[baseName] = jsonArray;
}


void QEmcStatus::updateMotion(const pb::EmcStatusMotion &motion)
{
    if (motion.has_active_queue())
    {
        m_motion["activeQueue"] = (int)motion.active_queue();
    }

    if (motion.has_actual_position()) {
        updatePosition(&m_motion, "actualPosition", motion.actual_position());
    }

    if (motion.has_adaptive_feed_enabled()) {
        m_motion["adaptiveFeedEnabled"] = (bool)motion.adaptive_feed_enabled();
    }

    if (motion.ain_size() > 0) {
        updateIndexValue<double>(&m_motion, "ain", motion.ain());
    }

    if (motion.aout_size() > 0) {
        updateIndexValue<double>(&m_motion, "aout", motion.aout());
    }

    if (motion.axis_size() > 0) {
        updateIndexMessage<pb::EmcStatusMotionAxis>(&m_motion, "axis", motion.axis());
    }

    if (motion.has_block_delete()) {
        m_motion["blockDelete"] = (bool)motion.block_delete();
    }

    if (motion.has_current_line()) {
        m_motion["currentLine"] = (int)motion.current_line();
    }

    if (motion.has_current_vel()) {
        m_motion["currentVel"] = (double)motion.current_vel();
    }

    if (motion.has_delay_left()) {
        m_motion["delayLeft"] = (double)motion.delay_left();
    }

    if (motion.din_size() > 0) {
        updateIndexValue<bool>(&m_motion, "din", motion.din());
    }

    if (motion.has_distance_to_go()) {
        m_motion["distanceToGo"] = (double)motion.distance_to_go();
    }

    if (motion.dout_size() > 0) {
        updateIndexValue<bool>(&m_motion, "dout", motion.dout());
    }

    if (motion.has_dtg()) {
        updatePosition(&m_motion, "dtg", motion.dtg());
    }

    if (motion.has_enabled()) {
        m_motion["enabled"] = (double)motion.enabled();
    }

    if (motion.has_feed_hold_enabled()) {
        m_motion["feedHoldEnabled"] = (bool)motion.feed_hold_enabled();
    }

    if (motion.has_feed_override_enabled()) {
        m_motion["feedOverrideEnabled"] = (bool)motion.feed_override_enabled();
    }

    if (motion.has_feedrate()) {
        m_motion["feedrate"] = (double)motion.feedrate();
    }

    if (motion.has_g5x_index()) {
        m_motion["g5xIndex"] = (int)motion.g5x_index();
    }

    if (motion.has_g5x_offset()) {
        updatePosition(&m_motion, "g5xOffset", motion.g5x_offset());
    }

    if (motion.has_g92_offset()) {
        updatePosition(&m_motion, "g92Offset", motion.g92_offset());
    }

    if (motion.has_id()) {
        m_motion["id"] = (int)motion.id();
    }

    if (motion.has_inpos()) {
        m_motion["inpos"] = (bool)motion.inpos();
    }

    if (motion.has_joint_actual_position()) {
        updatePosition(&m_motion, "jointActualPosition", motion.joint_actual_position());
    }

    if (motion.has_joint_position()) {
        updatePosition(&m_motion, "jointPosition", motion.joint_position());
    }

    if (motion.limit_size() > 0) {
        updateIndexValue<int>(&m_motion, "limit", motion.limit());
    }

    if (motion.has_motion_line()) {
        m_motion["motionLine"] = (int)motion.motion_line();
    }

    if (motion.has_motion_type()) {
        m_motion["motionType"] = (int)motion.motion_type();
    }

    if (motion.has_motion_mode()) {
        m_motion["motionMode"] = (int)motion.motion_mode();
    }

    if (motion.has_paused()) {
        m_motion["paused"] = (bool)motion.paused();
    }

    if (motion.has_position()) {
        updatePosition(&m_motion, "position", motion.position());
    }

    if (motion.has_probe_tripped()) {
        m_motion["probeTripped"] = (bool)motion.probe_tripped();
    }

    if (motion.has_probe_val()) {
        m_motion["probeVal"] = (int)motion.probe_val();
    }

    if (motion.has_probed_position()) {
        updatePosition(&m_motion, "probedPosition", motion.probed_position());
    }

    if (motion.has_probing()) {
        m_motion["probing"] = (bool)motion.probing();
    }

    if (motion.has_queue()) {
        m_motion["queue"] = (int)motion.queue();
    }
    if (motion.has_queue_full()) {
        m_motion["queueFull"] = (bool)motion.queue_full();
    }

    if (motion.has_rotation_xy()) {
        m_motion["rotationXY"] = (double)motion.rotation_xy();
    }

    if (motion.has_spindle_brake()) {
        m_motion["spindleBrake"] = (bool)motion.spindle_brake();
    }

    if (motion.has_spindle_direction()) {
        m_motion["spindleDirection"] = (int)motion.spindle_direction();
    }

    if (motion.has_spindle_enabled()) {
        m_motion["spindleEnabled"] = (bool)motion.spindle_enabled();
    }

    if (motion.has_spindle_increasing()) {
        m_motion["spindleIncreasing"] = (int)motion.spindle_increasing();
    }

    if (motion.has_spindle_override_enabled()) {
        m_motion["spindleOverrideEnabled"] = (bool)motion.spindle_override_enabled();
    }

    if (motion.has_spindle_speed()) {
        m_motion["spindleSpeed"] = (double)motion.spindle_speed();
    }

    if (motion.has_spindlerate()) {
        m_motion["spindlerate"] = (double)motion.spindlerate();
    }

    if (motion.has_state()) {
        m_motion["state"] = (int)motion.state();
    }

    emit motionChanged(m_motion);
}

void QEmcStatus::updateConfig(const pb::EmcStatusConfig &config)
{
    if (config.has_acceleration()) {
        m_config["acceleration"] = (double)config.acceleration();
    }

    if (config.has_angular_units()) {
        m_config["angularUnits"] = (double)config.angular_units();
    }

    if (config.has_axes()) {
        m_config["axes"] = (int)config.axes();
    }

    if (config.axis_size() > 0) {
        updateIndexMessage<pb::EmcStatusConfigAxis>(&m_config, "axis", config.axis());
    }

    if (config.has_axis_mask()) {
        m_config["axisMask"] = (int)config.axis_mask();
    }

    if (config.has_cycle_time()) {
        m_config["cycleTime"] = (double)config.cycle_time();
    }

    if (config.has_debug()) {
        m_config["debug"] = (int)config.debug();
    }

    if (config.has_kinematics_type()) {
        m_config["kinematicsType"] = (int)config.kinematics_type();
    }

    if (config.has_linear_units()) {
        m_config["linearUnits"] = (int)config.linear_units();
    }

    if (config.has_max_acceleration()) {
        m_config["maxAcceleration"] = (double)config.max_acceleration();
    }

    if (config.has_max_velocity()) {
        m_config["maxVelocity"] = (double)config.max_velocity();
    }

    if (config.has_program_units()) {
        m_config["programUnits"] = (int)config.program_units();
    }

    if (config.has_velocity()) {
        m_config["velocity"] = (int)config.velocity();
    }

    emit configChanged(m_config);
}

void QEmcStatus::updateIo(const pb::EmcStatusIo &io)
{
    if (io.has_estop()) {
        m_io["estop"] = (bool)io.estop();
    }

    if (io.has_flood()) {
        m_io["flood"] = (bool)io.flood();
    }

    if (io.has_lube()) {
        m_io["lube"] = (bool)io.lube();
    }

    if (io.has_lube_level()) {
        m_io["lubeLevel"] = (bool)io.lube_level();
    }

    if (io.has_mist()) {
        m_io["mist"] = (bool)io.mist();
    }

    if (io.has_pocket_prepped()) {
        m_io["pocketPrepped"] = (bool)io.pocket_prepped();
    }

    if (io.has_tool_in_spindle()) {
        m_io["toolInSpindle"] = (bool)io.tool_in_spindle();
    }

    if (io.has_tool_offset()) {
        updatePosition(&m_io, "toolOffset", io.tool_offset());
    }

    if (io.tool_table_size() > 0) {
        updateIndexMessage<pb::EmcToolData>(&m_io, "toolTable", io.tool_table());
    }

    emit ioChanged(m_io);
}

void QEmcStatus::updateTask(const pb::EmcStatusTask &task)
{
    if (task.has_echo_serial_number()) {
        m_task["echoSerialNumber"] = (int)task.echo_serial_number();
    }

    if (task.has_exec_state()) {
        m_task["execState"] = (int)task.exec_state();
    }

    if (task.has_file()) {
        m_task["file"] = QString::fromStdString(task.file());
    }

    if (task.has_input_timeout()) {
        m_task["inputTimeout"] = (bool)task.input_timeout();
    }

    if (task.has_optional_stop()) {
        m_task["optionalStop"] = (bool)task.optional_stop();
    }

    if (task.has_read_line()) {
        m_task["readLine"] = (int)task.read_line();
    }

    if (task.has_task_mode()) {
        m_task["taskMode"] = (int)task.task_mode();
    }

    if (task.has_task_paused()) {
        m_task["taskPaused"] = (int)task.task_paused();
    }

    if (task.has_task_state()) {
        m_task["taskState"] = (int)task.task_state();
    }

    emit taskChanged(m_task);
}

void QEmcStatus::updateInterp(const pb::EmcStatusInterp &interp)
{
    if (interp.has_command()) {
        m_interp["command"] = QString::fromStdString(interp.command());
    }

    if (interp.gcodes_size() > 0) {
        updateIndexValue<int>(&m_interp, "gcodes", interp.gcodes());
    }

    if (interp.has_interp_state()) {
        m_interp["interpState"] = (int)interp.interp_state();
    }

    if (interp.has_interpreter_errcode()) {
        m_interp["interpreterErrcode"] = (int)interp.interpreter_errcode();
    }

    if (interp.mcodes_size() > 0) {
        updateIndexValue<int>(&m_interp, "mcodes", interp.mcodes());
    }

    if (interp.settings_size() > 0) {
        updateIndexValue<double>(&m_interp, "settings", interp.settings());
    }

    emit interpChanged(m_interp);
}

void QEmcStatus::statusMessageReceived(const QList<QByteArray> &messageList)
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
        }

        if ((topic == "config") && m_rx.has_emc_status_config()) {
            updateConfig(m_rx.emc_status_config());
        }

        if ((topic == "io") && m_rx.has_emc_status_io()) {
            updateIo(m_rx.emc_status_io());
        }

        if ((topic == "task") && m_rx.has_emc_status_task()) {
            updateTask(m_rx.emc_status_task());
        }

        if ((topic == "interp") && m_rx.has_emc_status_interp()) {
            updateInterp(m_rx.emc_status_interp());
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
                startStatusHeartbeat(pparams.keepalive_timer());
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

void QEmcStatus::pollError(int errorNum, const QString &errorMsg)
{
    QString errorString;
    errorString = QString("Error %1: ").arg(errorNum) + errorMsg;
    updateError(SocketError, errorString);
    updateState(Error);
}

void QEmcStatus::statusHeartbeatTimerTick()
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
bool QEmcStatus::connectSockets()
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
void QEmcStatus::disconnectSockets()
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

void QEmcStatus::subscribe()
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

void QEmcStatus::unsubscribe()
{
    m_sState = Down;
    foreach (QString subscription, m_subscriptions)
    {
        m_statusSocket->unsubscribeFrom(subscription);

        if (subscription == "motion") {
            clearObject(MotionChannel);
        }
        else if (subscription == "config") {
            clearObject(ConfigChannel);
        }
        else if (subscription == "io") {
            clearObject(IoChannel);
        }
        else if (subscription == "task") {
            clearObject(TaskChannel);
        }
        else if (subscription == "interp") {
            clearObject(InterpChannel);
        }
    }
    m_subscriptions.clear();
}

/** If the ready property has a rising edge we try to connect
 *  if it is has a falling edge we disconnect and cleanup
 */
void QEmcStatus::setReady(bool arg)
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

void QEmcStatus::clearObject(QEmcStatus::StatusChannel channel)
{
    switch (channel)
    {
    case MotionChannel:
        m_motion = QJsonObject();
        emit motionChanged(m_motion);
        return;
    case ConfigChannel:
        m_config = QJsonObject();
        emit configChanged(m_config);
        return;
    case IoChannel:
        m_io = QJsonObject();
        emit ioChanged(m_io);
        return;
    case TaskChannel:
        m_task = QJsonObject();
        emit taskChanged(m_task);
        return;
    case InterpChannel:
        m_interp = QJsonObject();
        emit interpChanged(m_interp);
        return;
    }
}
