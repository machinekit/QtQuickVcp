#include "qapplicationcommand.h"
#include "debughelper.h"

QApplicationCommand::QApplicationCommand(QQuickItem *parent) :
    QQuickItem(parent),
    m_commandUri(""),
    m_heartbeatPeriod(3000),
    m_ready(false),
    m_cState(Down),
    m_connectionState(Disconnected),
    m_error(NoError),
    m_errorString(""),
    m_componentCompleted(false),
    m_context(NULL),
    m_commandSocket(NULL),
    m_commandHeartbeatTimer(new QTimer(this)),
    m_commandPingOutstanding(false)
{
    connect(m_commandHeartbeatTimer, SIGNAL(timeout()),
            this, SLOT(commandHeartbeatTimerTick()));
}

QApplicationCommand::~QApplicationCommand()
{
    disconnectSockets();
}

/** componentComplete is executed when the QML component is fully loaded */
void QApplicationCommand::componentComplete()
{
    m_componentCompleted = true;

    if (m_ready == true)    // the component was set to ready before it was completed
    {
        start();
    }

    QQuickItem::componentComplete();
}

/** If the ready property has a rising edge we try to connect
 *  if it is has a falling edge we disconnect and cleanup
 */
void QApplicationCommand::setReady(bool arg)
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

void QApplicationCommand::abort()
{
    if (m_connectionState != Connected) {
        return;
    }

    sendCommandMessage(pb::MT_EMC_TASK_ABORT);
}

void QApplicationCommand::runProgram(int lineNumber = 0)
{
    if (m_connectionState != Connected) {
        return;
    }

    pb::EmcCommandParameters *commandParams = m_tx.mutable_emc_command_params();
    commandParams->set_line_number(lineNumber);

    sendCommandMessage(pb::MT_EMC_TASK_PLAN_RUN);
}

void QApplicationCommand::pauseProgram()
{
    if (m_connectionState != Connected) {
        return;
    }

    sendCommandMessage(pb::MT_EMC_TASK_PLAN_PAUSE);
}

void QApplicationCommand::stepProgram()
{
    if (m_connectionState != Connected) {
        return;
    }

    sendCommandMessage(pb::MT_EMC_TASK_PLAN_STEP);
}

void QApplicationCommand::resumeProgram()
{
    if (m_connectionState != Connected) {
        return;
    }

    sendCommandMessage(pb::MT_EMC_TASK_PLAN_RESUME);
}

void QApplicationCommand::setSpindleBrake(QApplicationCommand::SpindleBrake brake)
{
    if (m_connectionState != Connected) {
        return;
    }

    if (brake == EngageBrake)
    {
        sendCommandMessage(pb::MT_EMC_SPINDLE_BRAKE_ENGAGE);
    }
    else if (brake == ReleaseBrake)
    {
        sendCommandMessage(pb::MT_EMC_SPINDLE_BRAKE_RELEASE);
    }
}

void QApplicationCommand::setDebugLevel(int debugLevel)
{
    if (m_connectionState != Connected) {
        return;
    }

    pb::EmcCommandParameters *commandParams = m_tx.mutable_emc_command_params();
    commandParams->set_debug_level(debugLevel);

    sendCommandMessage(pb::MT_EMC_SET_DEBUG);
}

void QApplicationCommand::setFeedOverride(double scale)
{
    if (m_connectionState != Connected) {
        return;
    }

    pb::EmcCommandParameters *commandParams = m_tx.mutable_emc_command_params();
    commandParams->set_scale(scale);

    sendCommandMessage(pb::MT_EMC_TRAJ_SET_SCALE);
}

void QApplicationCommand::setFloodEnabled(bool enable)
{
    if (m_connectionState != Connected) {
        return;
    }

    if (enable)
    {
        sendCommandMessage(pb::MT_EMC_COOLANT_FLOOD_ON);
    }
    else
    {
        sendCommandMessage(pb::MT_EMC_COOLANT_FLOOD_OFF);
    }
}

void QApplicationCommand::homeAxis(int index)
{
    if (m_connectionState != Connected) {
        return;
    }

    pb::EmcCommandParameters *commandParams = m_tx.mutable_emc_command_params();
    commandParams->set_index(index);

    sendCommandMessage(pb::MT_EMC_AXIS_HOME);
}

void QApplicationCommand::jog(QApplicationCommand::JogType type, int axisIndex, double velocity = 0.0, double distance = 0.0)
{
    if (m_connectionState != Connected) {
        return;
    }

    pb::ContainerType containerType;
    pb::EmcCommandParameters *commandParams = m_tx.mutable_emc_command_params();
    commandParams->set_index(axisIndex);

    if (type == StopJog)
    {
        containerType = pb::MT_EMC_AXIS_ABORT;
    }
    else if (type == ContinousJog) {
        containerType = pb::MT_EMC_AXIS_JOG;
        commandParams->set_velocity(velocity);
    }
    else if (type == IncrementJog)
    {
        containerType = pb::MT_EMC_AXIS_INCR_JOG;
        commandParams->set_velocity(velocity);
        commandParams->set_distance(distance);
    }
    else
    {
        m_tx.Clear();
        return;
    }

    sendCommandMessage(containerType);
}

void QApplicationCommand::loadToolTable()
{
    if (m_connectionState != Connected) {
        return;
    }

    sendCommandMessage(pb::MT_EMC_TOOL_LOAD_TOOL_TABLE);
}

void QApplicationCommand::setMaximumVelocity(double velocity)
{
    if (m_connectionState != Connected) {
        return;
    }

    pb::EmcCommandParameters *commandParams = m_tx.mutable_emc_command_params();
    commandParams->set_velocity(velocity);

    sendCommandMessage(pb::MT_EMC_TRAJ_SET_MAX_VELOCITY);
}

void QApplicationCommand::executeMdi(const QString &command)
{
    if (m_connectionState != Connected) {
        return;
    }

    pb::EmcCommandParameters *commandParams = m_tx.mutable_emc_command_params();
    commandParams->set_command(command.toStdString());

    sendCommandMessage(pb::MT_EMC_TASK_PLAN_EXECUTE);
}

void QApplicationCommand::setMistEnabled(bool enable)
{
    if (m_connectionState != Connected) {
        return;
    }

    if (enable)
    {
        sendCommandMessage(pb::MT_EMC_COOLANT_MIST_ON);
    }
    else
    {
        sendCommandMessage(pb::MT_EMC_COOLANT_MIST_OFF);
    }
}

void QApplicationCommand::setTaskMode(QApplicationStatus::TaskMode mode)
{
    if (m_connectionState != Connected) {
        return;
    }

    pb::EmcCommandParameters *commandParams = m_tx.mutable_emc_command_params();
    commandParams->set_task_mode((pb::EmcTaskModeType)mode);

    sendCommandMessage(pb::MT_EMC_TASK_SET_MODE);
}

void QApplicationCommand::overrideLimits()
{
    if (m_connectionState != Connected) {
        return;
    }

    sendCommandMessage(pb::MT_EMC_AXIS_OVERRIDE_LIMITS);
}

void QApplicationCommand::openProgram(const QString &filePath)
{
    if (m_connectionState != Connected) {
        return;
    }

    pb::EmcCommandParameters *commandParams = m_tx.mutable_emc_command_params();
    commandParams->set_path(filePath.toStdString());

    sendCommandMessage(pb::MT_EMC_TASK_PLAN_OPEN);
}

void QApplicationCommand::resetProgram()
{
    if (m_connectionState != Connected) {
        return;
    }

    sendCommandMessage(pb::MT_EMC_TASK_PLAN_INIT);
}

void QApplicationCommand::setAdaptiveFeedEnabled(bool enable)
{
    if (m_connectionState != Connected) {
        return;
    }

    pb::EmcCommandParameters *commandParams = m_tx.mutable_emc_command_params();
    commandParams->set_enable(enable);

    sendCommandMessage(pb::MT_EMC_MOTION_ADAPTIVE);
}

void QApplicationCommand::setAnalogOutput(int index, double value)
{
    if (m_connectionState != Connected) {
        return;
    }

    pb::EmcCommandParameters *commandParams = m_tx.mutable_emc_command_params();
    commandParams->set_index(index);
    commandParams->set_value(value);

    sendCommandMessage(pb::MT_EMC_MOTION_SET_AOUT);
}

void QApplicationCommand::setBlockDeleteEnabled(bool enable)
{
    if (m_connectionState != Connected) {
        return;
    }

    pb::EmcCommandParameters *commandParams = m_tx.mutable_emc_command_params();
    commandParams->set_enable(enable);

    sendCommandMessage(pb::MT_EMC_TASK_PLAN_SET_BLOCK_DELETE);
}

void QApplicationCommand::setDigitalOutput(int index, bool enable)
{
    if (m_connectionState != Connected) {
        return;
    }

    pb::EmcCommandParameters *commandParams = m_tx.mutable_emc_command_params();
    commandParams->set_index(index);
    commandParams->set_enable(enable);

    sendCommandMessage(pb::MT_EMC_MOTION_SET_DOUT);
}

void QApplicationCommand::setFeedHoldEnabled(bool enable)
{
    if (m_connectionState != Connected) {
        return;
    }

    pb::EmcCommandParameters *commandParams = m_tx.mutable_emc_command_params();
    commandParams->set_enable(enable);

    sendCommandMessage(pb::MT_EMC_TRAJ_SET_FH_ENABLE);
}

void QApplicationCommand::setFeedOverrideEnabled(bool enable)
{
    if (m_connectionState != Connected) {
        return;
    }

    pb::EmcCommandParameters *commandParams = m_tx.mutable_emc_command_params();
    commandParams->set_enable(enable);

    sendCommandMessage(pb::MT_EMC_TRAJ_SET_FO_ENABLE);
}

void QApplicationCommand::setAxisMaxPositionLimit(int axisIndex, double value)
{
    if (m_connectionState != Connected) {
        return;
    }

    pb::EmcCommandParameters *commandParams = m_tx.mutable_emc_command_params();
    commandParams->set_index(axisIndex);
    commandParams->set_value(value);

    sendCommandMessage(pb::MT_EMC_AXIS_SET_MAX_POSITION_LIMIT);
}

void QApplicationCommand::setAxisMinPositionLimit(int axisIndex, double value)
{
    if (m_connectionState != Connected) {
        return;
    }

    pb::EmcCommandParameters *commandParams = m_tx.mutable_emc_command_params();
    commandParams->set_index(axisIndex);
    commandParams->set_value(value);

    sendCommandMessage(pb::MT_EMC_AXIS_SET_MIN_POSITION_LIMIT);
}

void QApplicationCommand::setOptionalStopEnabled(bool enable)
{
    if (m_connectionState != Connected) {
        return;
    }

    pb::EmcCommandParameters *commandParams = m_tx.mutable_emc_command_params();
    commandParams->set_enable(enable);

    sendCommandMessage(pb::MT_EMC_TASK_PLAN_OPTIONAL_STOP);
}

void QApplicationCommand::setSpindleOverrideEnabled(bool enable)
{
    if (m_connectionState != Connected) {
        return;
    }

    pb::EmcCommandParameters *commandParams = m_tx.mutable_emc_command_params();
    commandParams->set_enable(enable);

    sendCommandMessage(pb::MT_EMC_TRAJ_SET_SO_ENABLE);
}

void QApplicationCommand::setSpindle(QApplicationCommand::SpindleMode mode, double velocity = 0.0)
{
    if (m_connectionState != Connected) {
        return;
    }

    pb::ContainerType containerType;
    pb::EmcCommandParameters *commandParams = m_tx.mutable_emc_command_params();

    switch (mode)
    {
    case SpindleForward:
        containerType = pb::MT_EMC_SPINDLE_ON;
        commandParams->set_velocity(velocity);
        break;
    case SpindleReverse:
        containerType = pb::MT_EMC_SPINDLE_ON;
        commandParams->set_velocity(velocity * -1.0);
        break;
    case SpindleOff:
        containerType = pb::MT_EMC_SPINDLE_OFF;
        break;
    case SpindleIncrease:
        containerType = pb::MT_EMC_SPINDLE_INCREASE;
        break;
    case SpindleDecrease:
        containerType = pb::MT_EMC_SPINDLE_DECREASE;
        break;
    case SpindleConstant:
        containerType = pb::MT_EMC_SPINDLE_CONSTANT;
    default:
        m_tx.Clear();
        return;
    }

    sendCommandMessage(containerType);
}

void QApplicationCommand::setSpindleOverride(double scale)
{
    if (m_connectionState != Connected) {
        return;
    }

    pb::EmcCommandParameters *commandParams = m_tx.mutable_emc_command_params();
    commandParams->set_scale(scale);

    sendCommandMessage(pb::MT_EMC_TRAJ_SET_SPINDLE_SCALE);
}

void QApplicationCommand::setTaskState(QApplicationStatus::TaskState state)
{
    if (m_connectionState != Connected) {
        return;
    }

    pb::EmcCommandParameters *commandParams = m_tx.mutable_emc_command_params();
    commandParams->set_task_state((pb::EmcTaskStateType)state);

    sendCommandMessage(pb::MT_EMC_TASK_SET_STATE);
}

void QApplicationCommand::setTeleopEnabled(bool enable)
{
    if (m_connectionState != Connected) {
        return;
    }

    pb::EmcCommandParameters *commandParams = m_tx.mutable_emc_command_params();
    commandParams->set_enable(enable);

    sendCommandMessage(pb::MT_EMC_TRAJ_SET_TELEOP_ENABLE);
}

void QApplicationCommand::setTeleopVector(double a, double b, double c, double u = 0.0, double v = 0.0, double w = 0.0)
{
    if (m_connectionState != Connected) {
        return;
    }

    pb::EmcCommandParameters *commandParams = m_tx.mutable_emc_command_params();
    pb::EmcPose *pose = commandParams->mutable_pose();
    pose->set_a(a);
    pose->set_b(b);
    pose->set_c(c);
    pose->set_u(u);
    pose->set_v(v);
    pose->set_w(w);

    sendCommandMessage(pb::MT_EMC_TRAJ_SET_TELEOP_VECTOR);
}

void QApplicationCommand::setToolOffset(int index, double zOffset, double xOffset, double diameter, double frontangle, double backangle, int orientation)
{
    if (m_connectionState != Connected) {
        return;
    }

    pb::EmcCommandParameters *commandParams = m_tx.mutable_emc_command_params();
    pb::EmcToolData *tooldata = commandParams->mutable_tool_data();
    tooldata->set_index(index);
    tooldata->set_zoffset(zOffset);
    tooldata->set_xoffset(xOffset);
    tooldata->set_diameter(diameter);
    tooldata->set_frontangle(frontangle);
    tooldata->set_backangle(backangle);
    tooldata->set_orientation(orientation);

    sendCommandMessage(pb::MT_EMC_TOOL_SET_OFFSET);
}

void QApplicationCommand::setTrajectoryMode(QApplicationStatus::TrajectoryMode mode)
{
    if (m_connectionState != Connected) {
        return;
    }

    pb::EmcCommandParameters *commandParams = m_tx.mutable_emc_command_params();
    commandParams->set_traj_mode((pb::EmcTrajectoryModeType)mode);

    sendCommandMessage(pb::MT_EMC_TRAJ_SET_MODE);
}

void QApplicationCommand::unhomeAxis(int index)
{
    if (m_connectionState != Connected) {
        return;
    }

    pb::EmcCommandParameters *commandParams = m_tx.mutable_emc_command_params();
    commandParams->set_index(index);

    sendCommandMessage(pb::MT_EMC_AXIS_UNHOME);
}

void QApplicationCommand::start()
{
#ifdef QT_DEBUG
   DEBUG_TAG(1, "command", "start")
#endif
    m_cState = Trying;
    updateState(Connecting);


    if (connectSockets())
    {
        updateState(Connected);
    }
}

void QApplicationCommand::stop()
{
#ifdef QT_DEBUG
    DEBUG_TAG(1, "command", "stop")
#endif

    // cleanup here
    stopCommandHeartbeat();
    disconnectSockets();

    updateState(Disconnected);
    updateError(NoError, "");   // clear the error here
}

void QApplicationCommand::startCommandHeartbeat()
{
    m_commandPingOutstanding = false;

    if (m_heartbeatPeriod > 0)
    {
        m_commandHeartbeatTimer->setInterval(m_heartbeatPeriod);
        m_commandHeartbeatTimer->start();
    }
}

void QApplicationCommand::stopCommandHeartbeat()
{
    m_commandHeartbeatTimer->stop();
}

void QApplicationCommand::updateState(QApplicationCommand::State state)
{
    if (state != m_connectionState)
    {
        m_connectionState = state;
        emit connectionStateChanged(m_connectionState);

        if (m_connectionState == Connected)
        {
            startCommandHeartbeat();
        }
        else
        {
            stopCommandHeartbeat();
        }
    }
}

void QApplicationCommand::updateError(QApplicationCommand::ConnectionError error, const QString &errorString)
{
    m_error = error;
    m_errorString = errorString;

    emit errorStringChanged(m_errorString);
    emit errorChanged(m_error);
}

void QApplicationCommand::sendCommandMessage(pb::ContainerType type)
{
    try {
        m_tx.set_type(type);
        m_commandSocket->sendMessage(QByteArray(m_tx.SerializeAsString().c_str(), m_tx.ByteSize()));
        m_tx.Clear();
    }
    catch (zmq::error_t e) {
        QString errorString;
        errorString = QString("Error %1: ").arg(e.num()) + QString(e.what());
        updateError(SocketError, errorString);
        updateState(Error);
    }
}

/** Processes all message received on the command 0MQ socket */
void QApplicationCommand::commandMessageReceived(const QList<QByteArray> &messageList)
{
    m_rx.ParseFromArray(messageList.at(0).data(), messageList.at(0).size());

#ifdef QT_DEBUG
    std::string s;
    gpb::TextFormat::PrintToString(m_rx, &s);
    DEBUG_TAG(3, "command", "server message" << QString::fromStdString(s))
#endif

    if (m_rx.type() == pb::MT_PING_ACKNOWLEDGE)
    {
        m_cState = Up;
        m_commandPingOutstanding = false;

        if ((m_connectionState == Error) && (m_error == TimeoutError))   // recover from a timeout
        {
            updateError(NoError, "");
            updateState(Connected);
        }

#ifdef QT_DEBUG
        DEBUG_TAG(2, "command", "ping ack")
#endif
        return;
    }
    else if (m_rx.type() == pb::MT_ERROR)
    {
        QString errorString;

        for (int i = 0; i < m_rx.note_size(); ++i)
        {
            errorString.append(QString::fromStdString(m_rx.note(i)) + "\n");
        }

        m_cState = Down;
        updateError(CommandError, errorString);
        updateState(Error);

#ifdef QT_DEBUG
        DEBUG_TAG(1, "command", "error" << errorString)
#endif

        return;
    }
    else
    {
#ifdef QT_DEBUG
        DEBUG_TAG(1, "command", "UNKNOWN server message type")
#endif
    }
}

void QApplicationCommand::pollError(int errorNum, const QString &errorMsg)
{
    QString errorString;
    errorString = QString("Error %1: ").arg(errorNum) + errorMsg;
    updateError(SocketError, errorString);
    updateState(Error);
}

void QApplicationCommand::commandHeartbeatTimerTick()
{
    if (m_commandPingOutstanding)
    {
        m_cState = Trying;
        updateError(TimeoutError, "Command service timed out");
        updateState(Error);

#ifdef QT_DEBUG
        DEBUG_TAG(1, "command", "timeout")
#endif
    }

    sendCommandMessage(pb::MT_PING);

    m_commandPingOutstanding = true;

#ifdef QT_DEBUG
    DEBUG_TAG(2, "command", "ping")
        #endif
}

/** Connects the 0MQ sockets */
bool QApplicationCommand::connectSockets()
{
    m_context = new PollingZMQContext(this, 1);
    connect(m_context, SIGNAL(pollError(int,QString)),
            this, SLOT(pollError(int,QString)));
    m_context->start();

    m_commandSocket = m_context->createSocket(ZMQSocket::TYP_DEALER, this);
    m_commandSocket->setLinger(0);
    m_commandSocket->setIdentity(QString("%1-%2").arg("command").arg(QCoreApplication::applicationPid()).toLocal8Bit());

    try {
        m_commandSocket->connectTo(m_commandUri);
    }
    catch (zmq::error_t e) {
        QString errorString;
        errorString = QString("Error %1: ").arg(e.num()) + QString(e.what());
        updateError(SocketError, errorString);
        updateState(Error);
        return false;
    }

    connect(m_commandSocket, SIGNAL(messageReceived(QList<QByteArray>)),
            this, SLOT(commandMessageReceived(QList<QByteArray>)));

#ifdef QT_DEBUG
    DEBUG_TAG(1, "command", "sockets connected" << m_commandUri)
#endif

            return true;
}

/** Disconnects the 0MQ sockets */
void QApplicationCommand::disconnectSockets()
{
    if (m_commandSocket != NULL)
    {
        m_commandSocket->close();
        m_commandSocket->deleteLater();
        m_commandSocket = NULL;
    }

    if (m_context != NULL)
    {
        m_context->stop();
        m_context->deleteLater();
        m_context = NULL;
    }
}
