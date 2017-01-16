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

#include "applicationstatus.h"
#include <google/protobuf/text_format.h>
#include <machinetalkservice.h>
#include "debughelper.h"

using namespace machinetalk;

namespace qtquickvcp {

ApplicationStatus::ApplicationStatus(QObject *parent) :
    application::StatusBase(parent),
    m_running(false),
    m_synced(false),
    m_syncedChannels(NoChannel),
    m_channels(MotionChannel | ConfigChannel | IoChannel | TaskChannel | InterpChannel)
{
    connect(this, &ApplicationStatus::taskChanged,
            this, &ApplicationStatus::updateRunning);
    connect(this, &ApplicationStatus::interpChanged,
            this, &ApplicationStatus::updateRunning);

    initializeObject(MotionChannel);
    initializeObject(ConfigChannel);
    initializeObject(IoChannel);
    initializeObject(TaskChannel);
    initializeObject(InterpChannel);

    m_channelMap.insert("motion", MotionChannel);
    m_channelMap.insert("config", ConfigChannel);
    m_channelMap.insert("io", IoChannel);
    m_channelMap.insert("task", TaskChannel);
    m_channelMap.insert("interp", InterpChannel);
}

void ApplicationStatus::updateSync(ApplicationStatus::StatusChannel channel)
{
    m_syncedChannels |= channel;

    if (m_syncedChannels == m_channels && !m_synced) {
        channelsSynced();
    }
}

void ApplicationStatus::updateMotionObject(const EmcStatusMotion &motion)
{
    MachinetalkService::recurseMessage(motion, m_motion);
    emit motionChanged(m_motion);
}

void ApplicationStatus::updateConfigObject(const EmcStatusConfig &config)
{
    MachinetalkService::recurseMessage(config, m_config);
    emit configChanged(m_config);
}

void ApplicationStatus::updateIoObject(const EmcStatusIo &io)
{
    MachinetalkService::recurseMessage(io, m_io);
    emit ioChanged(m_io);
}

void ApplicationStatus::updateTaskObject(const EmcStatusTask &task)
{
    MachinetalkService::recurseMessage(task, m_task);
    emit taskChanged(m_task);
}

void ApplicationStatus::updateInterpObject(const EmcStatusInterp &interp)
{
    MachinetalkService::recurseMessage(interp, m_interp);
    emit interpChanged(m_interp);
}

void ApplicationStatus::emcstatFullUpdateReceived(const QByteArray &topic, const Container &rx)
{
    StatusChannel channel = m_channelMap.value(topic, NoChannel);
    emcstatUpdateReceived(channel, rx);
    updateSync(channel);
}

void ApplicationStatus::emcstatIncrementalUpdateReceived(const QByteArray &topic, const Container &rx)
{
    StatusChannel channel = m_channelMap.value(topic, NoChannel);
    emcstatUpdateReceived(channel, rx);
}

void ApplicationStatus::syncStatus()
{
    m_synced = true;
    emit syncedChanged(m_synced);
}

void ApplicationStatus::unsyncStatus()
{
    m_synced = false;
    m_syncedChannels = 0;
    emit syncedChanged(m_synced);
}

void ApplicationStatus::updateTopics()
{
    clearStatusTopics();
    if (m_channels & MotionChannel) {
        addStatusTopic("motion");
        initializeObject(MotionChannel);
    }
    if (m_channels & ConfigChannel) {
        addStatusTopic("config");
        initializeObject(ConfigChannel);
    }
    if (m_channels & TaskChannel) {
        addStatusTopic("task");
        initializeObject(TaskChannel);
    }
    if (m_channels & IoChannel) {
        addStatusTopic("io");
        initializeObject(IoChannel);
    }
    if (m_channels & InterpChannel) {
        addStatusTopic("interp");
        initializeObject(InterpChannel);
    }
}

void ApplicationStatus::emcstatUpdateReceived(StatusChannel channel, const Container &rx)
{
    switch (channel) {
    case MotionChannel:
        updateMotionObject(rx.emc_status_motion());
        break;
    case ConfigChannel:
        updateConfigObject(rx.emc_status_config());
        break;
    case IoChannel:
        updateIoObject(rx.emc_status_io());
        break;
    case TaskChannel:
        updateTaskObject(rx.emc_status_task());
        break;
    case InterpChannel:
        updateInterpObject(rx.emc_status_interp());
        break;
    case NoChannel:
        break;
    }
}

void ApplicationStatus::updateRunning(const QJsonObject &object)
{
    Q_UNUSED(object)
    bool running;

    running = m_task["taskMode"].isDouble() && m_interp["interpState"].isDouble()
            && ((static_cast<int>(m_task["taskMode"].toDouble()) == TaskModeAuto)
                || (static_cast<int>(m_task["taskMode"].toDouble()) == TaskModeMdi))
            && (static_cast<int>(m_interp["interpState"].toDouble()) != InterpreterIdle);

    if (running != m_running)
    {
        m_running = running;
        emit runningChanged(running);
    }
}

void ApplicationStatus::initializeObject(ApplicationStatus::StatusChannel channel)
{
    switch (channel)
    {
    case MotionChannel:
        m_motion = QJsonObject();
        MachinetalkService::recurseDescriptor(EmcStatusMotion::descriptor(), m_motion);
        emit motionChanged(m_motion);
        break;
    case ConfigChannel:
        m_config = QJsonObject();
        MachinetalkService::recurseDescriptor(EmcStatusConfig::descriptor(), m_config);
        emit configChanged(m_config);
        break;
    case IoChannel:
        m_io = QJsonObject();
        MachinetalkService::recurseDescriptor(EmcStatusIo::descriptor(), m_io);
        emit ioChanged(m_io);
        break;
    case TaskChannel:
        m_task = QJsonObject();
        MachinetalkService::recurseDescriptor(EmcStatusTask::descriptor(), m_task);
        emit taskChanged(m_task);
        break;
    case InterpChannel:
        m_interp = QJsonObject();
        MachinetalkService::recurseDescriptor(EmcStatusInterp::descriptor(), m_interp);
        emit interpChanged(m_interp);
        break;
    case NoChannel:
        break;
    }
}
}; // namespace qtquickvcp
