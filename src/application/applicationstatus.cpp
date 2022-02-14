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
#include <QtQml>
#include <google/protobuf/text_format.h>
#include <machinetalkservice.h>
#include "debughelper.h"

using namespace machinetalk;

namespace qtquickvcp {

ApplicationStatus::ApplicationStatus(QObject *parent) :
    application::StatusBase(parent),
    m_config(new QObject(this)),
    m_motion(new QObject(this)),
    m_io(new QObject(this)),
    m_task(new QObject(this)),
    m_interp(new QObject(this)),
    m_ui(new QObject(this)),
    m_running(false),
    m_synced(false),
    m_syncedChannels(NoChannel),
    m_channels(MotionChannel | ConfigChannel | IoChannel | TaskChannel | InterpChannel),
    m_optionalChannels(UiChannel)
{
    connect(this, &ApplicationStatus::taskChanged,
            this, &ApplicationStatus::updateRunning);
    connect(this, &ApplicationStatus::interpChanged,
            this, &ApplicationStatus::updateRunning);

    m_channelMap.insert("motion", MotionChannel);
    m_channelMap.insert("config", ConfigChannel);
    m_channelMap.insert("io", IoChannel);
    m_channelMap.insert("task", TaskChannel);
    m_channelMap.insert("interp", InterpChannel);
    m_channelMap.insert("ui", UiChannel);
}

void ApplicationStatus::componentComplete()
{
    initializeObject(MotionChannel);
    initializeObject(ConfigChannel);
    initializeObject(IoChannel);
    initializeObject(TaskChannel);
    initializeObject(InterpChannel);
    initializeObject(UiChannel);

    StatusBase::componentComplete();
}

void ApplicationStatus::updateSync(ApplicationStatus::StatusChannel channel)
{
    m_syncedChannels |= channel;
    bool flagsMatch = (
                (!(m_channels & MotionChannel) || (m_syncedChannels & MotionChannel)) &&
                (!(m_channels & ConfigChannel) || (m_syncedChannels & ConfigChannel)) &&
                (!(m_channels & IoChannel) || (m_syncedChannels & IoChannel)) &&
                (!(m_channels & TaskChannel) || (m_syncedChannels & TaskChannel)) &&
                (!(m_channels & InterpChannel) || (m_syncedChannels & InterpChannel)) &&
                (!(m_channels & UiChannel) || (m_syncedChannels & UiChannel))
                );

    if (flagsMatch && !m_synced) {
        channelsSynced();
    }
}

void ApplicationStatus::updateMotionObject(const EmcStatusMotion &motion)
{
    MachinetalkService::recurseMessage(motion, m_motion);
    emit motionChanged();
}

void ApplicationStatus::updateConfigObject(const EmcStatusConfig &config)
{
    MachinetalkService::recurseMessage(config, m_config);
    emit configChanged();
}

void ApplicationStatus::updateIoObject(const EmcStatusIo &io)
{
    MachinetalkService::recurseMessage(io, m_io);
    emit ioChanged();
}

void ApplicationStatus::updateTaskObject(const EmcStatusTask &task)
{
    MachinetalkService::recurseMessage(task, m_task);
    emit taskChanged();
}

void ApplicationStatus::updateInterpObject(const EmcStatusInterp &interp)
{
    MachinetalkService::recurseMessage(interp, m_interp);
    emit interpChanged();
}

void ApplicationStatus::updateUiObject(const EmcStatusUI &ui)
{
    MachinetalkService::recurseMessage(ui, m_ui);
    emit uiChanged();
}

void ApplicationStatus::handleEmcstatFullUpdateMessage(const QByteArray &topic, const Container &rx)
{
    StatusChannel channel = m_channelMap.value(topic, NoChannel);
    emcstatUpdateReceived(channel, rx);
    updateSync(channel);
}

void ApplicationStatus::handleEmcstatIncrementalUpdateMessage(const QByteArray &topic, const Container &rx)
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
    m_syncedChannels = StatusChannels();
    emit syncedChanged(m_synced);
}

void ApplicationStatus::updateTopics()
{
    clearStatusTopics();
    if ((m_channels | m_optionalChannels) & MotionChannel) {
        addStatusTopic("motion");
        initializeObject(MotionChannel);
    }
    if ((m_channels | m_optionalChannels) & ConfigChannel) {
        addStatusTopic("config");
        initializeObject(ConfigChannel);
    }
    if ((m_channels | m_optionalChannels) & TaskChannel) {
        addStatusTopic("task");
        initializeObject(TaskChannel);
    }
    if ((m_channels | m_optionalChannels) & IoChannel) {
        addStatusTopic("io");
        initializeObject(IoChannel);
    }
    if ((m_channels | m_optionalChannels) & InterpChannel) {
        addStatusTopic("interp");
        initializeObject(InterpChannel);
    }
    if ((m_channels | m_optionalChannels) & UiChannel) {
        addStatusTopic("ui");
        initializeObject(UiChannel);
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
    case UiChannel:
        updateUiObject(rx.emc_status_ui());
        break;
    case NoChannel:
        break;
    }
}

void ApplicationStatus::updateRunning()
{
    bool running;

    running = m_task->property("taskMode").isValid() && m_interp->property("interpState").isValid()
            && ((m_task->property("taskMode").toInt() == TaskModeAuto)
                || (m_task->property("taskMode").toInt() == TaskModeMdi))
            && (m_interp->property("interpState").toInt() != InterpreterIdle);

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
        //m_motion->deleteLater();
        m_motion = MachinetalkService::recurseDescriptor(EmcStatusMotion::descriptor(), m_motion);
        emit motionChanged();
        break;
    case ConfigChannel:
        //m_config->deleteLater();
        m_config = MachinetalkService::recurseDescriptor(EmcStatusConfig::descriptor(), m_config);
        emit configChanged();
        break;
    case IoChannel:
        //m_io->deleteLater();
        m_io = MachinetalkService::recurseDescriptor(EmcStatusIo::descriptor(), m_io);
        emit ioChanged();
        break;
    case TaskChannel:
        //m_task->deleteLater();
        m_task = MachinetalkService::recurseDescriptor(EmcStatusTask::descriptor(), m_task);
        emit taskChanged();
        break;
    case InterpChannel:
        //m_interp->deleteLater();
        m_interp = MachinetalkService::recurseDescriptor(EmcStatusInterp::descriptor(), m_interp);
        emit interpChanged();
        break;
    case UiChannel:
        //m_ui->deleteLater();
        m_ui = MachinetalkService::recurseDescriptor(EmcStatusUI::descriptor(), m_ui);
        emit uiChanged();
        break;
    case NoChannel:
        break;
    }
}
} // namespace qtquickvcp
