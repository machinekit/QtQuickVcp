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
#ifndef APPLICATIONSTATUS_H
#define APPLICATIONSTATUS_H

#include <QStringList>
#include <QHash>
#include <google/protobuf/text_format.h>
#include <google/protobuf/message.h>
#include <google/protobuf/descriptor.h>
#include <machinetalk/protobuf/message.pb.h>
#include <machinetalk/protobuf/status.pb.h>
#include <machinetalkservice.h>
#include <application/statusbase.h>

namespace qtquickvcp {

class ApplicationStatus : public application::StatusBase
{
    Q_OBJECT
    Q_PROPERTY(QJsonObject config READ config NOTIFY configChanged)
    Q_PROPERTY(QJsonObject motion READ motion NOTIFY motionChanged)
    Q_PROPERTY(QJsonObject io READ io NOTIFY ioChanged)
    Q_PROPERTY(QJsonObject task READ task NOTIFY taskChanged)
    Q_PROPERTY(QJsonObject interp READ interp NOTIFY interpChanged)
    Q_PROPERTY(bool running READ isRunning NOTIFY runningChanged)
    Q_PROPERTY(bool synced READ isSynced NOTIFY syncedChanged)
    Q_PROPERTY(StatusChannels channels READ channels WRITE setChannels NOTIFY channelsChanged)
    Q_ENUMS(OriginIndex TrajectoryMode MotionStatus
            AxisType KinematicsType CanonUnits TaskExecState TaskState
            TaskMode InterpreterState InterpreterExitCode PositionOffset
            PositionFeedback TimeUnits MotionType)
    Q_FLAGS(StatusChannels)

public:
    explicit ApplicationStatus(QObject *parent = 0);

    enum OriginIndex {
        OriginG54 = pb::ORIGIN_G54,
        OriginG55 = pb::ORIGIN_G55,
        OriginG56 = pb::ORIGIN_G56,
        OriginG57 = pb::ORIGIN_G57,
        OriginG58 = pb::ORIGIN_G58,
        OriginG59 = pb::ORIGIN_G59,
        OriginG59_1 = pb::ORIGIN_G59_1,
        OriginG59_2 = pb::ORIGIN_G59_2,
        OriginG59_3 = pb::ORIGIN_G59_3
    };

    enum TrajectoryMode {
        FreeMode = pb::EMC_TRAJ_MODE_FREE,
        CoordinatedMode = pb::EMC_TRAJ_MODE_COORD,
        TeleopMode = pb::EMC_TRAJ_MODE_TELEOP
    };

    enum MotionStatus {
        MotionUninitialized = pb::UNINITIALIZED_STATUS,
        MotionDone = pb::RCS_DONE,
        MotionExec = pb::RCS_EXEC,
        MotionError = pb::RCS_ERROR,
        MotionReceived = pb::RCS_RECEIVED
    };

    enum MotionType {
        NoneType = pb::_EMC_MOTION_TYPE_NONE,
        TraverseType = pb::_EMC_MOTION_TYPE_TRAVERSE,
        FeedType = pb::_EMC_MOTION_TYPE_FEED,
        ArcType = pb::_EMC_MOTION_TYPE_ARC,
        ToolchangeType = pb::_EMC_MOTION_TYPE_TOOLCHANGE,
        ProbingType = pb::_EMC_MOTION_TYPE_PROBING,
        IndexRotaryType = pb::_EMC_MOTION_TYPE_INDEXROTARY
    };

    enum AxisType {
        LinearAxis = pb::EMC_AXIS_LINEAR,
        AngularAxis = pb::EMC_AXIS_ANGULAR
    };

    enum KinematicsType {
        IdentityKinematics = pb::KINEMATICS_IDENTITY,
        ForwardOnlyKinematics = pb::KINEMATICS_FORWARD_ONLY,
        InverseOnlyKinematics = pb::KINEMATICS_INVERSE_ONLY,
        BothKinematics = pb::KINEMATICS_BOTH
    };

    enum CanonUnits {
        CanonUnitsInch = pb::CANON_UNITS_INCH,
        CanonUnitsMm = pb::CANON_UNITS_MM,
        CanonUnitsCm = pb::CANON_UNITS_CM
    };

    enum TimeUnits {
        TimeUnitsMinute = pb::TIME_UNITS_MINUTE,
        TimeUnitsSecond = pb::TIME_UNITS_SECOND
    };

    enum TaskExecState {
        TaskError = pb::EMC_TASK_EXEC_ERROR,
        TaskDone = pb::EMC_TASK_EXEC_DONE,
        TaskWaitingForMotion = pb::EMC_TASK_EXEC_WAITING_FOR_MOTION,
        TaskWaitingForMotionQueue = pb::EMC_TASK_EXEC_WAITING_FOR_MOTION_QUEUE,
        TaskWaitingForIo = pb::EMC_TASK_EXEC_WAITING_FOR_IO,
        TaskWaitingForMotionAndIo = pb::EMC_TASK_EXEC_WAITING_FOR_MOTION_AND_IO,
        TaskWaitingForDelay = pb::EMC_TASK_EXEC_WAITING_FOR_DELAY,
        TaskWaitingForSystemCmd = pb::EMC_TASK_EXEC_WAITING_FOR_SYSTEM_CMD,
        TaskWaitingForSpindleOriented = pb::EMC_TASK_EXEC_WAITING_FOR_SPINDLE_ORIENTED
    };

    enum TaskMode {
        TaskModeManual = pb::EMC_TASK_MODE_MANUAL,
        TaskModeAuto = pb::EMC_TASK_MODE_AUTO,
        TaskModeMdi = pb::EMC_TASK_MODE_MDI
    };

    enum TaskState {
        TaskStateEstop = pb::EMC_TASK_STATE_ESTOP,
        TaskStateEstopReset = pb::EMC_TASK_STATE_ESTOP_RESET,
        TaskStateOff = pb::EMC_TASK_STATE_OFF,
        TaskStateOn = pb::EMC_TASK_STATE_ON
    };

    enum InterpreterState {
        InterpreterIdle = pb::EMC_TASK_INTERP_IDLE,
        InterpreterReading = pb::EMC_TASK_INTERP_READING,
        InterpreterPaused = pb::EMC_TASK_INTERP_PAUSED,
        InterpreterWaiting = pb::EMC_TASK_INTERP_WAITING
    };

    enum InterpreterExitCode {
        InterpreterExitOk = pb::EMC_INTERP_EXIT_OK,
        InterpreterExitExit = pb::EMC_INTERP_EXIT_EXIT,
        InterpreterExitExecuteFinish = pb::EMC_INTERP_EXIT_EXECUTE_FINISH,
        InterpreterExitEndfile = pb::EMC_INTERP_EXIT_ENDFILE,
        InterpreterExitFileNotOpen = pb::EMC_INTERP_EXIT_FILE_NOT_OPEN,
        InterpreterExitError = pb::EMC_INTERP_EXIT_ERROR
    };

    enum PositionOffset {
        RelativePositionOffset = pb::EMC_CONFIG_RELATIVE_OFFSET,
        MachinePositionOffset = pb::EMC_CONFIG_MACHINE_OFFSET
    };

    enum PositionFeedback {
        ActualPositionFeedback = pb::EMC_CONFIG_ACTUAL_FEEDBACK,
        CommandedPositionFeedback = pb::EMC_CONFIG_COMMANDED_FEEDBACK
    };

    enum StatusChannel {
        NoChannel = 0x0,
        MotionChannel = 0x1,
        ConfigChannel = 0x2,
        IoChannel     = 0x4,
        TaskChannel   = 0x8,
        InterpChannel = 0x10
    };
    Q_DECLARE_FLAGS(StatusChannels, StatusChannel)

    QJsonObject config() const
    {
        return m_config;
    }

    QJsonObject motion() const
    {
        return m_motion;
    }

    QJsonObject io() const
    {
        return m_io;
    }

    QJsonObject task() const
    {
        return m_task;
    }

    QJsonObject interp() const
    {
        return m_interp;
    }

    StatusChannels channels() const
    {
        return m_channels;
    }

    bool isRunning() const
    {
        return m_running;
    }

    bool isSynced() const
    {
        return m_synced;
    }

public slots:
    void setChannels(StatusChannels arg)
    {
        if (m_channels == arg)
            return;

        m_channels = arg;
        emit channelsChanged(arg);
    }

private:
    QJsonObject     m_config;
    QJsonObject     m_motion;
    QJsonObject     m_io;
    QJsonObject     m_task;
    QJsonObject     m_interp;
    bool            m_running;
    bool            m_synced;
    StatusChannels  m_syncedChannels;
    StatusChannels  m_channels;
    QHash<QByteArray, StatusChannel> m_channelMap;

    void emcstatUpdateReceived(StatusChannel channel, const pb::Container &rx);
    void updateSync(StatusChannel channel);
    void updateMotionObject(const pb::EmcStatusMotion &motion);
    void updateConfigObject(const pb::EmcStatusConfig &config);
    void updateIoObject(const pb::EmcStatusIo &io);
    void updateTaskObject(const pb::EmcStatusTask &task);
    void updateInterpObject(const pb::EmcStatusInterp &interp);
    void initializeObject(StatusChannel channel);


private slots:
    void emcstatFullUpdateReceived(const QByteArray &topic, const pb::Container &rx);
    void emcstatIncrementalUpdateReceived(const QByteArray &topic, const pb::Container &rx);
    void syncStatus();
    void unsyncStatus();
    void updateTopics();

    void updateRunning(const QJsonObject &object);

signals:
    void configChanged(const QJsonObject &arg);
    void motionChanged(const QJsonObject &arg);
    void ioChanged(const QJsonObject &arg);
    void taskChanged(const QJsonObject &arg);
    void interpChanged(const QJsonObject &arg);
    void channelsChanged(StatusChannels arg);
    void runningChanged(bool arg);
    void syncedChanged(bool arg);
}; // class ApplicationStatus
} // namespace qtquickvcp

#endif // APPLICATIONSTATUS_H
