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

#ifndef QEMCCOMMAND_H
#define QEMCCOMMAND_H

#include <abstractserviceimplementation.h>
#include <QTimer>
#include <QUrl>
#include <QCoreApplication>
#include <QHostInfo>
#include <nzmqt/nzmqt.hpp>
#include <google/protobuf/text_format.h>
#include <machinetalk/protobuf/message.pb.h>
#include <machinetalk/protobuf/status.pb.h>
#include <machinetalk/protobuf/emcclass.pb.h>
#include "qapplicationstatus.h"

class QApplicationCommand : public AbstractServiceImplementation
{
    Q_OBJECT
    Q_PROPERTY(QString commandUri READ commandUri WRITE setCommandUri NOTIFY commandUriChanged)
    Q_PROPERTY(int heartbeatPeriod READ heartbeatPeriod WRITE heartbeatPeriod NOTIFY heartbeatPeriodChanged)
    Q_PROPERTY(bool connected READ isConnected NOTIFY connectedChanged)
    Q_PROPERTY(State connectionState READ connectionState NOTIFY connectionStateChanged)
    Q_PROPERTY(ConnectionError error READ error NOTIFY errorChanged)
    Q_PROPERTY(QString errorString READ errorString NOTIFY errorStringChanged)
    Q_ENUMS(State ConnectionError SpindleBrake JogType TaskState TaskMode SpindleMode TrajectoryMode)

public:
    explicit QApplicationCommand(QObject *parent = 0);

    enum SocketState {
        Down = 1,
        Trying = 2,
        Up = 3
    };

    enum State {
        Disconnected = 0,
        Connecting = 1,
        Connected = 2,
        Timeout = 3,
        Error = 4
    };

    enum ConnectionError {
        NoError = 0,
        ServiceError = 1,
        SocketError = 2
    };

    enum SpindleBrake {
        ReleaseBrake,
        EngageBrake
    };

    enum JogType {
        StopJog = 0,
        ContinuousJog = 1,
        IncrementJog = 2
    };

    enum SpindleMode {
        SpindleForward,
        SpindleReverse,
        SpindleOff,
        SpindleDecrease,
        SpindleIncrease,
        SpindleConstant
    };

    enum TaskState {
        TaskStateEstop = pb::EMC_TASK_STATE_ESTOP,
        TaskStateEstopReset = pb::EMC_TASK_STATE_ESTOP_RESET,
        TaskStateOff = pb::EMC_TASK_STATE_OFF,
        TaskStateOn = pb::EMC_TASK_STATE_ON
    };

    enum TaskMode {
        TaskModeManual = pb::EMC_TASK_MODE_MANUAL,
        TaskModeAuto = pb::EMC_TASK_MODE_AUTO,
        TaskModeMdi = pb::EMC_TASK_MODE_MDI
    };

    enum TrajectoryMode {
        FreeMode = pb::EMC_TRAJ_MODE_FREE,
        CoordinatedMode = pb::EMC_TRAJ_MODE_COORD,
        TeleopMode = pb::EMC_TRAJ_MODE_TELEOP
    };

    QString commandUri() const
    {
        return m_commandUri;
    }

    State connectionState() const
    {
        return m_connectionState;
    }

    ConnectionError error() const
    {
        return m_error;
    }

    QString errorString() const
    {
        return m_errorString;
    }

    int heartbeatPeriod() const
    {
        return m_heartbeatPeriod;
    }

    bool isConnected() const
    {
        return m_connected;
    }

public slots:

    void setCommandUri(QString arg)
    {
        if (m_commandUri == arg)
            return;

        m_commandUri = arg;
        emit commandUriChanged(arg);
    }

    void heartbeatPeriod(int arg)
    {
        if (m_heartbeatPeriod == arg)
            return;

        m_heartbeatPeriod = arg;
        emit heartbeatPeriodChanged(arg);
    }

    void abort(const QString &interpreter);
    void runProgram(const QString &interpreter, int lineNumber);
    void pauseProgram(const QString &interpreter);
    void stepProgram(const QString &interpreter);
    void resumeProgram(const QString &interpreter);
    void resetProgram(const QString &interpreter);
    void setTaskMode(const QString &interpreter, TaskMode mode);
    void setTaskState(const QString &interpreter, TaskState state);
    void openProgram(const QString &interpreter, const QString &fileName);
    void executeMdi(const QString &interpreter, const QString &command);
    void setSpindleBrake(SpindleBrake brake);
    void setDebugLevel(int debugLevel);
    void setFeedOverride(double scale);
    void setFloodEnabled(bool enable);
    void homeAxis(int index);
    void jog(JogType type, int axisIndex);
    void jog(JogType type, int axisIndex, double velocity);
    void jog(JogType type, int axisIndex, double velocity, double distance);
    void loadToolTable();
    void updateToolTable(const QJsonArray &toolTable);
    void setMaximumVelocity(double velocity);
    void setMistEnabled(bool enable);
    void overrideLimits();
    void setAdaptiveFeedEnabled(bool enable);
    void setAnalogOutput(int index, double value);
    void setBlockDeleteEnabled(bool enable);
    void setDigitalOutput(int index, bool enable);
    void setFeedHoldEnabled(bool enable);
    void setFeedOverrideEnabled(bool enable);
    void setAxisMaxPositionLimit(int axisIndex, double value);
    void setAxisMinPositionLimit(int axisIndex, double value);
    void setOptionalStopEnabled(bool enable);
    void setSpindleOverrideEnabled(bool enable);
    void setSpindle(SpindleMode mode);
    void setSpindle(SpindleMode mode, double velocity);
    void setSpindleOverride(double scale);
    void setTeleopEnabled(bool enable);
    void setTeleopVector(double a, double b, double c, double u, double v, double w);
    void setToolOffset(int index, double zOffset, double xOffset, double diameter, double frontangle, double backangle, int orientation);
    void setTrajectoryMode(TrajectoryMode mode);
    void unhomeAxis(int index);
    void shutdown();
private:

    QString         m_commandUri;
    int             m_heartbeatPeriod;
    bool            m_connected;
    SocketState     m_commandSocketState;
    State           m_connectionState;
    ConnectionError m_error;
    QString         m_errorString;

    nzmqt::PollingZMQContext *m_context;
    nzmqt::ZMQSocket *m_commandSocket;
    QTimer      *m_commandHeartbeatTimer;
    int         m_commandPingErrorCount;
    int         m_commandPingErrorThreshold;
    QUuid       m_uuid;
    // more efficient to reuse a protobuf Message
    pb::Container   m_rx;
    pb::Container   m_tx;

    void start();
    void stop();
    void cleanup();
    void startCommandHeartbeat();
    void stopCommandHeartbeat();
    void updateState(State state);
    void updateState(State state, ConnectionError error, const QString &errorString);
    void updateError(ConnectionError error, const QString &errorString);
    void sendCommandMessage(pb::ContainerType type);

private slots:
    void commandMessageReceived(const QList<QByteArray> &messageList);
    void pollError(int errorNum, const QString &errorMsg);
    void commandHeartbeatTimerTick();

    bool connectSockets();
    void disconnectSockets();

signals:
    void commandUriChanged(QString arg);
    void connectionStateChanged(State arg);
    void errorChanged(ConnectionError arg);
    void errorStringChanged(QString arg);
    void heartbeatPeriodChanged(int arg);
    void connectedChanged(bool arg);
};

#endif // QEMCCOMMAND_H
