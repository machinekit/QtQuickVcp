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

#include <QObject>
#include <QJsonObject>
#include <machinetalk/protobuf/message.pb.h>
#include <machinetalk/protobuf/status.pb.h>
#include <application/statusbase.h>

namespace qtquickvcp {

class ApplicationStatus : public machinetalk::application::StatusBase
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
        OriginG54 = machinetalk::ORIGIN_G54,
        OriginG55 = machinetalk::ORIGIN_G55,
        OriginG56 = machinetalk::ORIGIN_G56,
        OriginG57 = machinetalk::ORIGIN_G57,
        OriginG58 = machinetalk::ORIGIN_G58,
        OriginG59 = machinetalk::ORIGIN_G59,
        OriginG59_1 = machinetalk::ORIGIN_G59_1,
        OriginG59_2 = machinetalk::ORIGIN_G59_2,
        OriginG59_3 = machinetalk::ORIGIN_G59_3
    };

    enum TrajectoryMode {
        FreeMode = machinetalk::EMC_TRAJ_MODE_FREE,
        CoordinatedMode = machinetalk::EMC_TRAJ_MODE_COORD,
        TeleopMode = machinetalk::EMC_TRAJ_MODE_TELEOP
    };

    enum MotionStatus {
        MotionUninitialized = machinetalk::UNINITIALIZED_STATUS,
        MotionDone = machinetalk::RCS_DONE,
        MotionExec = machinetalk::RCS_EXEC,
        MotionError = machinetalk::RCS_ERROR,
        MotionReceived = machinetalk::RCS_RECEIVED
    };

    enum MotionType {
        NoneType = machinetalk::_EMC_MOTION_TYPE_NONE,
        TraverseType = machinetalk::_EMC_MOTION_TYPE_TRAVERSE,
        FeedType = machinetalk::_EMC_MOTION_TYPE_FEED,
        ArcType = machinetalk::_EMC_MOTION_TYPE_ARC,
        ToolchangeType = machinetalk::_EMC_MOTION_TYPE_TOOLCHANGE,
        ProbingType = machinetalk::_EMC_MOTION_TYPE_PROBING,
        IndexRotaryType = machinetalk::_EMC_MOTION_TYPE_INDEXROTARY
    };

    enum AxisType {
        LinearAxis = machinetalk::EMC_AXIS_LINEAR,
        AngularAxis = machinetalk::EMC_AXIS_ANGULAR
    };

    enum KinematicsType {
        IdentityKinematics = machinetalk::KINEMATICS_IDENTITY,
        ForwardOnlyKinematics = machinetalk::KINEMATICS_FORWARD_ONLY,
        InverseOnlyKinematics = machinetalk::KINEMATICS_INVERSE_ONLY,
        BothKinematics = machinetalk::KINEMATICS_BOTH
    };

    enum CanonUnits {
        CanonUnitsInch = machinetalk::CANON_UNITS_INCH,
        CanonUnitsMm = machinetalk::CANON_UNITS_MM,
        CanonUnitsCm = machinetalk::CANON_UNITS_CM
    };

    enum TimeUnits {
        TimeUnitsMinute = machinetalk::TIME_UNITS_MINUTE,
        TimeUnitsSecond = machinetalk::TIME_UNITS_SECOND
    };

    enum TaskExecState {
        TaskError = machinetalk::EMC_TASK_EXEC_ERROR,
        TaskDone = machinetalk::EMC_TASK_EXEC_DONE,
        TaskWaitingForMotion = machinetalk::EMC_TASK_EXEC_WAITING_FOR_MOTION,
        TaskWaitingForMotionQueue = machinetalk::EMC_TASK_EXEC_WAITING_FOR_MOTION_QUEUE,
        TaskWaitingForIo = machinetalk::EMC_TASK_EXEC_WAITING_FOR_IO,
        TaskWaitingForMotionAndIo = machinetalk::EMC_TASK_EXEC_WAITING_FOR_MOTION_AND_IO,
        TaskWaitingForDelay = machinetalk::EMC_TASK_EXEC_WAITING_FOR_DELAY,
        TaskWaitingForSystemCmd = machinetalk::EMC_TASK_EXEC_WAITING_FOR_SYSTEM_CMD,
        TaskWaitingForSpindleOriented = machinetalk::EMC_TASK_EXEC_WAITING_FOR_SPINDLE_ORIENTED
    };

    enum TaskMode {
        TaskModeManual = machinetalk::EMC_TASK_MODE_MANUAL,
        TaskModeAuto = machinetalk::EMC_TASK_MODE_AUTO,
        TaskModeMdi = machinetalk::EMC_TASK_MODE_MDI
    };

    enum TaskState {
        TaskStateEstop = machinetalk::EMC_TASK_STATE_ESTOP,
        TaskStateEstopReset = machinetalk::EMC_TASK_STATE_ESTOP_RESET,
        TaskStateOff = machinetalk::EMC_TASK_STATE_OFF,
        TaskStateOn = machinetalk::EMC_TASK_STATE_ON
    };

    enum InterpreterState {
        InterpreterIdle = machinetalk::EMC_TASK_INTERP_IDLE,
        InterpreterReading = machinetalk::EMC_TASK_INTERP_READING,
        InterpreterPaused = machinetalk::EMC_TASK_INTERP_PAUSED,
        InterpreterWaiting = machinetalk::EMC_TASK_INTERP_WAITING
    };

    enum InterpreterExitCode {
        InterpreterExitOk = machinetalk::EMC_INTERP_EXIT_OK,
        InterpreterExitExit = machinetalk::EMC_INTERP_EXIT_EXIT,
        InterpreterExitExecuteFinish = machinetalk::EMC_INTERP_EXIT_EXECUTE_FINISH,
        InterpreterExitEndfile = machinetalk::EMC_INTERP_EXIT_ENDFILE,
        InterpreterExitFileNotOpen = machinetalk::EMC_INTERP_EXIT_FILE_NOT_OPEN,
        InterpreterExitError = machinetalk::EMC_INTERP_EXIT_ERROR
    };

    enum PositionOffset {
        RelativePositionOffset = machinetalk::EMC_CONFIG_RELATIVE_OFFSET,
        MachinePositionOffset = machinetalk::EMC_CONFIG_MACHINE_OFFSET
    };

    enum PositionFeedback {
        ActualPositionFeedback = machinetalk::EMC_CONFIG_ACTUAL_FEEDBACK,
        CommandedPositionFeedback = machinetalk::EMC_CONFIG_COMMANDED_FEEDBACK
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

    void emcstatUpdateReceived(StatusChannel channel, const machinetalk::Container &rx);
    void updateSync(StatusChannel channel);
    void updateMotionObject(const machinetalk::EmcStatusMotion &motion);
    void updateConfigObject(const machinetalk::EmcStatusConfig &config);
    void updateIoObject(const machinetalk::EmcStatusIo &io);
    void updateTaskObject(const machinetalk::EmcStatusTask &task);
    void updateInterpObject(const machinetalk::EmcStatusInterp &interp);
    void initializeObject(StatusChannel channel);


private slots:
    void handleEmcstatFullUpdateMessage(const QByteArray &topic, const machinetalk::Container &rx);
    void handleEmcstatIncrementalUpdateMessage(const QByteArray &topic, const machinetalk::Container &rx);
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
