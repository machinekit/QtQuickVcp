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

#include "applicationcommand.h"
#include <QUrl>
#include <QJsonObject>
#include <google/protobuf/text_format.h>
#include <machinetalk/protobuf/status.pb.h>
#include <machinetalk/protobuf/emcclass.pb.h>
#include "debughelper.h"

using namespace machinetalk;

namespace qtquickvcp {

ApplicationCommand::ApplicationCommand(QObject *parent) :
    application::CommandBase(parent),
    m_connected(false)
{
}

void ApplicationCommand::abort(const QString &interpreter)
{
    if (!m_connected) {
        return;
    }

    m_tx.set_interp_name(interpreter.toStdString());

    sendEmcTaskAbort(m_tx);
}

void ApplicationCommand::runProgram(const QString &interpreter, int lineNumber = 0)
{
    if (!m_connected) {
        return;
    }

    EmcCommandParameters *commandParams = m_tx.mutable_emc_command_params();
    commandParams->set_line_number(lineNumber);
    m_tx.set_interp_name(interpreter.toStdString());

    sendEmcTaskPlanRun(m_tx);
}

void ApplicationCommand::pauseProgram(const QString &interpreter)
{
    if (!m_connected) {
        return;
    }

    m_tx.set_interp_name(interpreter.toStdString());

    sendEmcTaskPlanPause(m_tx);
}

void ApplicationCommand::stepProgram(const QString &interpreter)
{
    if (!m_connected) {
        return;
    }

    m_tx.set_interp_name(interpreter.toStdString());

    sendEmcTaskPlanStep(m_tx);
}

void ApplicationCommand::resumeProgram(const QString &interpreter)
{
    if (!m_connected) {
        return;
    }

    m_tx.set_interp_name(interpreter.toStdString());

    sendEmcTaskPlanResume(m_tx);
}

void ApplicationCommand::setSpindleBrake(ApplicationCommand::SpindleBrake brake)
{
    if (!m_connected) {
        return;
    }

    if (brake == EngageBrake)
    {
        sendEmcSpindleBrakeEngage(m_tx);
    }
    else if (brake == ReleaseBrake)
    {
        sendEmcSpindleBrakeRelease(m_tx);
    }
}

void ApplicationCommand::setDebugLevel(int debugLevel)
{
    if (!m_connected) {
        return;
    }

    EmcCommandParameters *commandParams = m_tx.mutable_emc_command_params();
    commandParams->set_debug_level(debugLevel);

    sendEmcSetDebug(m_tx);
}

void ApplicationCommand::setFeedOverride(double scale)
{
    if (!m_connected) {
        return;
    }

    EmcCommandParameters *commandParams = m_tx.mutable_emc_command_params();
    commandParams->set_scale(scale);

    sendEmcTrajSetScale(m_tx);
}

void ApplicationCommand::setRapidOverride(double scale)
{
    if (!m_connected) {
        return;
    }

    EmcCommandParameters *commandParams = m_tx.mutable_emc_command_params();
    commandParams->set_scale(scale);

    sendEmcTrajSetRapidScale(m_tx);
}

void ApplicationCommand::setFloodEnabled(bool enable)
{
    if (!m_connected) {
        return;
    }

    if (enable)
    {
        sendEmcCoolantFloodOn(m_tx);
    }
    else
    {
        sendEmcCoolantFloodOff(m_tx);
    }
}

void ApplicationCommand::homeAxis(int index)
{
    if (!m_connected) {
        return;
    }

    EmcCommandParameters *commandParams = m_tx.mutable_emc_command_params();
    commandParams->set_index(index);

    sendEmcAxisHome(m_tx);
}

void ApplicationCommand::jog(ApplicationCommand::JogType type, int axisIndex)
{
    jog(type, axisIndex, 0.0, 0.0);
}

void ApplicationCommand::jog(ApplicationCommand::JogType type, int axisIndex, double velocity)
{
    jog(type, axisIndex, velocity, 0.0);
}

void ApplicationCommand::jog(ApplicationCommand::JogType type, int axisIndex, double velocity, double distance)
{
    if (!m_connected) {
        return;
    }

    EmcCommandParameters *commandParams = m_tx.mutable_emc_command_params();
    commandParams->set_index(axisIndex);

    if (type == StopJog)
    {
        sendEmcAxisAbort(m_tx);
    }
    else if (type == ContinuousJog) {
        commandParams->set_velocity(velocity);
        sendEmcAxisJog(m_tx);
    }
    else if (type == IncrementJog)
    {
        commandParams->set_velocity(velocity);
        commandParams->set_distance(distance);
        sendEmcAxisIncrJog(m_tx);
    }
    else
    {
        m_tx.Clear();
    }
}

void ApplicationCommand::loadToolTable()
{
    if (!m_connected) {
        return;
    }

    sendEmcToolLoadToolTable(m_tx);
}

void ApplicationCommand::updateToolTable(const QJsonArray &toolTable)
{
    if (!m_connected) {
        return;
    }

    EmcCommandParameters *commandParams = m_tx.mutable_emc_command_params();
    for (int i = 0; i < toolTable.size(); ++i)
    {
        QJsonValue value = toolTable.at(i);
        QJsonObject object = value.toObject();
        EmcToolData *toolData = commandParams->add_tool_table();
        toolData->set_index(i);
        toolData->set_id(object.value("id").toInt(0));
        toolData->set_pocket(object.value("pocket").toInt(0));
        toolData->set_diameter(object.value("diameter").toDouble(0.0));
        toolData->set_backangle(object.value("backangle").toDouble(0.0));
        toolData->set_frontangle(object.value("frontangle").toDouble(0.0));
        toolData->set_orientation(object.value("orientation").toInt(0));
        toolData->set_comment(object.value("comment").toString().toStdString());

        Position *offset = toolData->mutable_offset();
        QJsonObject offsetObject = object.value("offset").toObject();
        offset->set_x(offsetObject.value("x").toDouble(0.0));
        offset->set_y(offsetObject.value("y").toDouble(0.0));
        offset->set_z(offsetObject.value("z").toDouble(0.0));
        offset->set_a(offsetObject.value("a").toDouble(0.0));
        offset->set_b(offsetObject.value("b").toDouble(0.0));
        offset->set_c(offsetObject.value("c").toDouble(0.0));
        offset->set_u(offsetObject.value("u").toDouble(0.0));
        offset->set_v(offsetObject.value("v").toDouble(0.0));
        offset->set_w(offsetObject.value("w").toDouble(0.0));
    }

    sendEmcToolUpdateToolTable(m_tx);
}

void ApplicationCommand::setMaximumVelocity(double velocity)
{
    if (!m_connected) {
        return;
    }

    EmcCommandParameters *commandParams = m_tx.mutable_emc_command_params();
    commandParams->set_velocity(velocity);

    sendEmcTrajSetMaxVelocity(m_tx);
}

void ApplicationCommand::executeMdi(const QString &interpreter, const QString &command)
{
    if (!m_connected) {
        return;
    }

    EmcCommandParameters *commandParams = m_tx.mutable_emc_command_params();
    commandParams->set_command(command.toStdString());
    m_tx.set_interp_name(interpreter.toStdString());

    sendEmcTaskPlanExecute(m_tx);
}

void ApplicationCommand::setMistEnabled(bool enable)
{
    if (!m_connected) {
        return;
    }

    if (enable)
    {
        sendEmcCoolantMistOn(m_tx);
    }
    else
    {
        sendEmcCoolantMistOff(m_tx);
    }
}

void ApplicationCommand::setTaskMode(const QString &interpreter, TaskMode mode)
{
    if (!m_connected) {
        return;
    }

    EmcCommandParameters *commandParams = m_tx.mutable_emc_command_params();
    commandParams->set_task_mode((EmcTaskModeType)mode);
    m_tx.set_interp_name(interpreter.toStdString());

    sendEmcTaskSetMode(m_tx);
}

void ApplicationCommand::overrideLimits()
{
    if (!m_connected) {
        return;
    }

    sendEmcAxisOverrideLimits(m_tx);
}

void ApplicationCommand::openProgram(const QString &interpreter, const QString &filePath)
{
    if (!m_connected) {
        return;
    }

    EmcCommandParameters *commandParams = m_tx.mutable_emc_command_params();
    commandParams->set_path(QUrl(filePath).toLocalFile().toStdString());
    m_tx.set_interp_name(interpreter.toStdString());

    sendEmcTaskPlanOpen(m_tx);
}

void ApplicationCommand::resetProgram(const QString &interpreter)
{
    if (!m_connected) {
        return;
    }

    m_tx.set_interp_name(interpreter.toStdString());

    sendEmcTaskPlanInit(m_tx);
}

void ApplicationCommand::setAdaptiveFeedEnabled(bool enable)
{
    if (!m_connected) {
        return;
    }

    EmcCommandParameters *commandParams = m_tx.mutable_emc_command_params();
    commandParams->set_enable(enable);

    sendEmcMotionAdaptive(m_tx);
}

void ApplicationCommand::setAnalogOutput(int index, double value)
{
    if (!m_connected) {
        return;
    }

    EmcCommandParameters *commandParams = m_tx.mutable_emc_command_params();
    commandParams->set_index(index);
    commandParams->set_value(value);

    sendEmcMotionSetAout(m_tx);
}

void ApplicationCommand::setBlockDeleteEnabled(bool enable)
{
    if (!m_connected) {
        return;
    }

    EmcCommandParameters *commandParams = m_tx.mutable_emc_command_params();
    commandParams->set_enable(enable);

    sendEmcTaskPlanSetBlockDelete(m_tx);
}

void ApplicationCommand::setDigitalOutput(int index, bool enable)
{
    if (!m_connected) {
        return;
    }

    EmcCommandParameters *commandParams = m_tx.mutable_emc_command_params();
    commandParams->set_index(index);
    commandParams->set_enable(enable);

    sendEmcMotionSetDout(m_tx);
}

void ApplicationCommand::setFeedHoldEnabled(bool enable)
{
    if (!m_connected) {
        return;
    }

    EmcCommandParameters *commandParams = m_tx.mutable_emc_command_params();
    commandParams->set_enable(enable);

    sendEmcTrajSetFhEnable(m_tx);
}

void ApplicationCommand::setFeedOverrideEnabled(bool enable)
{
    if (!m_connected) {
        return;
    }

    EmcCommandParameters *commandParams = m_tx.mutable_emc_command_params();
    commandParams->set_enable(enable);

    sendEmcTrajSetFoEnable(m_tx);
}

void ApplicationCommand::setAxisMaxPositionLimit(int axisIndex, double value)
{
    if (!m_connected) {
        return;
    }

    EmcCommandParameters *commandParams = m_tx.mutable_emc_command_params();
    commandParams->set_index(axisIndex);
    commandParams->set_value(value);

    sendEmcAxisSetMaxPositionLimit(m_tx);
}

void ApplicationCommand::setAxisMinPositionLimit(int axisIndex, double value)
{
    if (!m_connected) {
        return;
    }

    EmcCommandParameters *commandParams = m_tx.mutable_emc_command_params();
    commandParams->set_index(axisIndex);
    commandParams->set_value(value);

    sendEmcAxisSetMinPositionLimit(m_tx);
}

void ApplicationCommand::setOptionalStopEnabled(bool enable)
{
    if (!m_connected) {
        return;
    }

    EmcCommandParameters *commandParams = m_tx.mutable_emc_command_params();
    commandParams->set_enable(enable);

    sendEmcTaskPlanSetOptionalStop(m_tx);
}

void ApplicationCommand::setSpindleOverrideEnabled(bool enable)
{
    if (!m_connected) {
        return;
    }

    EmcCommandParameters *commandParams = m_tx.mutable_emc_command_params();
    commandParams->set_enable(enable);

    sendEmcTrajSetSoEnable(m_tx);
}

void ApplicationCommand::setSpindle(ApplicationCommand::SpindleMode mode)
{
    setSpindle(mode, 0.0);
}

void ApplicationCommand::setSpindle(ApplicationCommand::SpindleMode mode, double velocity = 0.0)
{
    if (!m_connected) {
        return;
    }

    EmcCommandParameters *commandParams = m_tx.mutable_emc_command_params();

    switch (mode)
    {
    case SpindleForward:
        commandParams->set_velocity(velocity);
        sendEmcSpindleOn(m_tx);
        break;
    case SpindleReverse:
        commandParams->set_velocity(velocity * -1.0);
        sendEmcSpindleOn(m_tx);
        break;
    case SpindleOff:
        sendEmcSpindleOff(m_tx);
        break;
    case SpindleIncrease:
        sendEmcSpindleIncrease(m_tx);
        break;
    case SpindleDecrease:
        sendEmcSpindleDecrease(m_tx);
        break;
    case SpindleConstant:
        sendEmcSpindleConstant(m_tx);
        break;
    }
}

void ApplicationCommand::setSpindleOverride(double scale)
{
    if (!m_connected) {
        return;
    }

    EmcCommandParameters *commandParams = m_tx.mutable_emc_command_params();
    commandParams->set_scale(scale);

    sendEmcTrajSetSpindleScale(m_tx);
}

void ApplicationCommand::setTaskState(const QString &interpreter, TaskState state)
{
    if (!m_connected) {
        return;
    }

    EmcCommandParameters *commandParams = m_tx.mutable_emc_command_params();
    commandParams->set_task_state((EmcTaskStateType)state);
    m_tx.set_interp_name(interpreter.toStdString());

    sendEmcTaskSetState(m_tx);
}

void ApplicationCommand::setTeleopEnabled(bool enable)
{
    if (!m_connected) {
        return;
    }

    EmcCommandParameters *commandParams = m_tx.mutable_emc_command_params();
    commandParams->set_enable(enable);

    sendEmcTrajSetTeleopEnable(m_tx);
}

void ApplicationCommand::setTeleopVector(double a, double b, double c, double u = 0.0, double v = 0.0, double w = 0.0)
{
    if (!m_connected) {
        return;
    }

    EmcCommandParameters *commandParams = m_tx.mutable_emc_command_params();
    EmcPose *pose = commandParams->mutable_pose();
    pose->set_a(a);
    pose->set_b(b);
    pose->set_c(c);
    pose->set_u(u);
    pose->set_v(v);
    pose->set_w(w);

    sendEmcTrajSetTeleopVector(m_tx);
}

void ApplicationCommand::setToolOffset(int index, double zOffset, double xOffset, double diameter, double frontangle, double backangle, int orientation)
{
    if (!m_connected) {
        return;
    }

    EmcCommandParameters *commandParams = m_tx.mutable_emc_command_params();
    EmcToolData *tooldata = commandParams->mutable_tool_data();
    Position *position = tooldata->mutable_offset();
    tooldata->set_index(index);
    position->set_z(zOffset);
    position->set_x(xOffset);
    tooldata->set_diameter(diameter);
    tooldata->set_frontangle(frontangle);
    tooldata->set_backangle(backangle);
    tooldata->set_orientation(orientation);

    sendEmcToolSetOffset(m_tx);
}

void ApplicationCommand::setTrajectoryMode(TrajectoryMode mode)
{
    if (!m_connected) {
        return;
    }

    EmcCommandParameters *commandParams = m_tx.mutable_emc_command_params();
    commandParams->set_traj_mode((EmcTrajectoryModeType)mode);

    sendEmcTrajSetMode(m_tx);
}

void ApplicationCommand::unhomeAxis(int index)
{
    if (!m_connected) {
        return;
    }

    EmcCommandParameters *commandParams = m_tx.mutable_emc_command_params();
    commandParams->set_index(index);

    sendEmcAxisUnhome(m_tx);
}

void ApplicationCommand::shutdown()
{
    if (!m_connected) {
        return;
    }

    sendShutdown(m_tx);

    QProcess process;
    process.setWorkingDirectory("/sbin");
    process.start("sudo -S shutdown -P now");   // -P(power-off)

    bool retval = false;
    while ((retval = process.waitForFinished()));
        qDebug() << process.readAll();
}

void ApplicationCommand::setConnected()
{
    m_connected = true;
    emit connectedChanged(m_connected);
}

void ApplicationCommand::clearConnected()
{
    m_connected = false;
    emit connectedChanged(m_connected);
}

void ApplicationCommand::emccmdExecutedReceived(const Container &rx)
{
    Q_UNUSED(rx); // message currently unused
}

void ApplicationCommand::emccmdCompletedReceived(const Container &rx)
{
    Q_UNUSED(rx); // message currently unused
}

} // namespace qtquickvcp
