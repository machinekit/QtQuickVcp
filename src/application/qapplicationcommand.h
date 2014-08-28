#ifndef QEMCCOMMAND_H
#define QEMCCOMMAND_H

#include <QQuickItem>
#include <QTimer>
#include <nzmqt/nzmqt.hpp>
#include <google/protobuf/text_format.h>
#include "message.pb.h"
#include "status.pb.h"
#include "emcclass.pb.h"
#include "qapplicationstatus.h"

#if defined(Q_OS_IOS)
namespace gpb = google_public::protobuf;
#else
namespace gpb = google::protobuf;
#endif

using namespace nzmqt;

class QApplicationCommand : public QQuickItem
{
    Q_OBJECT
    Q_INTERFACES(QQmlParserStatus)
    Q_PROPERTY(QString commandUri READ commandUri WRITE setCommandUri NOTIFY commandUriChanged)
    Q_PROPERTY(int heartbeatPeriod READ heartbeatPeriod WRITE heartbeatPeriod NOTIFY heartbeatPeriodChanged)
    Q_PROPERTY(bool ready READ ready WRITE setReady NOTIFY readyChanged)
    Q_PROPERTY(State connectionState READ connectionState NOTIFY connectionStateChanged)
    Q_PROPERTY(ConnectionError error READ error NOTIFY errorChanged)
    Q_PROPERTY(QString errorString READ errorString NOTIFY errorStringChanged)
    Q_ENUMS(State ConnectionError SpindleBrake JogType TaskState TaskMode)

public:
    explicit QApplicationCommand(QQuickItem *parent = 0);
    ~QApplicationCommand();

    enum SocketState {
        Down = 1,
        Trying = 2,
        Up = 3
    };

    enum State {
        Disconnected = 0,
        Connecting = 1,
        Connected = 2,
        Error = 3
    };

    enum ConnectionError {
        NoError = 0,
        CommandError = 1,
        TimeoutError = 2,
        SocketError = 3
    };

    enum SpindleBrake {
        ReleaseBrake,
        EngageBrake
    };

    enum JogType {
        StopJog,
        ContinousJog,
        IncrementJog
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

    virtual void componentComplete();

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

    bool ready() const
    {
        return m_ready;
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

    void setReady(bool arg);

    void abort();
    void runProgram(int lineNumber);
    void pauseProgram();
    void stepProgram();
    void resumeProgram();
    void setSpindleBrake(SpindleBrake brake);
    void setDebugLevel(int debugLevel);
    void setFeedOverride(double scale);
    void setFloodEnabled(bool enable);
    void homeAxis(int index);
    void jog(JogType type, int axisIndex, double velocity, double distance);
    void loadToolTable();
    void setMaximumVelocity(double velocity);
    void executeMdi(const QString &command);
    void setMistEnabled(bool enable);
    void setTaskMode(TaskMode mode);
    void overrideLimits();
    void openProgram(const QString &fileName);
    void resetProgram();
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
    void setSpindle(SpindleMode mode, double speed);
    void setSpindleOverride(double scale);
    void setTaskState(TaskState state);
    void setTeleopEnabled(bool enable);
    void setTeleopVector(double a, double b, double c, double u, double v, double w);
    void setToolOffset(int index, double zOffset, double xOffset, double diameter, double frontangle, double backangle, int orientation);
    void setTrajectoryMode(QApplicationStatus::TrajectoryMode mode);
    void unhomeAxis(int index);
private:

    QString         m_commandUri;
    int             m_heartbeatPeriod;
    bool            m_ready;
    SocketState     m_cState;
    State           m_connectionState;
    ConnectionError m_error;
    QString         m_errorString;
    bool            m_componentCompleted;

    PollingZMQContext *m_context;
    ZMQSocket   *m_commandSocket;
    QTimer      *m_commandHeartbeatTimer;
    bool         m_commandPingOutstanding;
    // more efficient to reuse a protobuf Message
    pb::Container   m_rx;
    pb::Container   m_tx;

    void start();
    void stop();
    void startCommandHeartbeat();
    void stopCommandHeartbeat();
    void updateState(State state);
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
    void readyChanged(bool arg);
};

#endif // QEMCCOMMAND_H
