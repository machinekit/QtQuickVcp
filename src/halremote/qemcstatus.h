#ifndef QEMCSTATUS_H
#define QEMCSTATUS_H

#include <QQuickItem>
#include <QTimer>
#include <QJsonObject>
#include <QJsonArray>
#include <nzmqt/nzmqt.hpp>
#include <google/protobuf/text_format.h>
#include "message.pb.h"
#include "status.pb.h"

#if defined(Q_OS_IOS)
namespace gpb = google_public::protobuf;
#else
namespace gpb = google::protobuf;
#endif

using namespace nzmqt;

class QEmcStatus : public QQuickItem
{
    Q_OBJECT
    Q_INTERFACES(QQmlParserStatus)
    Q_PROPERTY(QString statusUri READ statusUri WRITE setStatusUri NOTIFY statusUriChanged)
    Q_PROPERTY(bool ready READ ready WRITE setReady NOTIFY readyChanged)
    Q_PROPERTY(State connectionState READ connectionState NOTIFY connectionStateChanged)
    Q_PROPERTY(ConnectionError error READ error NOTIFY errorChanged)
    Q_PROPERTY(QString errorString READ errorString NOTIFY errorStringChanged)
    Q_PROPERTY(QJsonObject config READ config NOTIFY configChanged)
    Q_PROPERTY(QJsonObject motion READ motion NOTIFY motionChanged)
    Q_PROPERTY(QJsonObject io READ io NOTIFY ioChanged)
    Q_PROPERTY(QJsonObject task READ task NOTIFY taskChanged)
    Q_PROPERTY(QJsonObject interp READ interp NOTIFY interpChanged)
    Q_PROPERTY(StatusChannels channels READ channels WRITE setChannels NOTIFY channelsChanged)
    Q_ENUMS(State ConnectionError OriginIndex TrajectoryMode MotionStatus AxisType KinematicsType CanonUnits TaskExecState TaskState TaskMode InterpreterState InterpreterExitCode)
    Q_FLAGS(StatusChannels)

public:
    explicit QEmcStatus(QQuickItem *parent = 0);
    ~QEmcStatus();

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
        StatusError = 1,
        TimeoutError = 2,
        SocketError = 3
    };

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
        InchesUnits = pb::CANON_UNITS_INCHES,
        mmUnits = pb::CANON_UNITS_MM,
        cmUnits = pb::CANON_UNITS_CM
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
        TaskSateEstopReset = pb::EMC_TASK_STATE_ESTOP_RESET,
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

    enum StatusChannel {
        MotionChannel = 0x1,
        ConfigChannel = 0x2,
        IoChannel     = 0x4,
        TaskChannel   = 0x8,
        InterpChannel = 0x16
    };
    Q_DECLARE_FLAGS(StatusChannels, StatusChannel)

    virtual void componentComplete();

    QString statusUri() const
    {
        return m_statusUri;
    }

    bool ready() const
    {
        return m_ready;
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

public slots:

    void setStatusUri(QString arg)
    {
        if (m_statusUri == arg)
            return;

        m_statusUri = arg;
        emit statusUriChanged(arg);
    }

    void setReady(bool arg);

    void setChannels(StatusChannels arg)
    {
        if (m_channels == arg)
            return;

        m_channels = arg;
        emit channelsChanged(arg);
    }

    void clearObject(StatusChannel channel);

private:
    QString         m_statusUri;
    bool            m_ready;
    SocketState     m_sState;
    State           m_connectionState;
    ConnectionError m_error;
    QString         m_errorString;
    QJsonObject     m_config;
    QJsonObject     m_motion;
    QJsonObject     m_io;
    QJsonObject     m_task;
    QJsonObject     m_interp;
    StatusChannels  m_channels;
    bool            m_componentCompleted;

    PollingZMQContext *m_context;
    ZMQSocket   *m_statusSocket;
    QStringList  m_subscriptions;
    QTimer      *m_statusHeartbeatTimer;
    bool         m_statusPingOutstanding;
    // more efficient to reuse a protobuf Message
    pb::Container   m_rx;

    void start();
    void stop();
    void startStatusHeartbeat(int interval);
    void stopStatusHeartbeat();
    void refreshStatusHeartbeat();
    void updateState(State state);
    void updateError(ConnectionError error, const QString &errorString);
    void updatePosition(QJsonObject *object, const QString &baseName, const pb::Position &position);
    template<typename ValueType, class Type>
    void updateIndexValue(QJsonObject *object, const QString &baseName, const gpb::RepeatedPtrField<Type> &fields);
    template<typename ValueType, class Type>
    void updateIndexMessage(QJsonObject *object, const QString &baseName, const gpb::RepeatedPtrField<Type> &fields);
    void updateMessage(QJsonObject *jsonObject, const pb::EmcStatusMotionAxis &axis);
    void updateMessage(QJsonObject *jsonObject, const pb::EmcStatusConfigAxis &axis);
    void updateMessage(QJsonObject *jsonObject, const pb::EmcToolData &toolData);
    void updateMotion(const pb::EmcStatusMotion &motion);
    void updateConfig(const pb::EmcStatusConfig &config);
    void updateIo(const pb::EmcStatusIo &io);
    void updateTask(const pb::EmcStatusTask &task);
    void updateInterp(const pb::EmcStatusInterp &interp);

private slots:
    void statusMessageReceived(const QList<QByteArray> &messageList);
    void pollError(int errorNum, const QString &errorMsg);
    void statusHeartbeatTimerTick();

    bool connectSockets();
    void disconnectSockets();
    void subscribe();
    void unsubscribe();

signals:
    void statusUriChanged(QString arg);
    void readyChanged(bool arg);
    void connectionStateChanged(State arg);
    void errorChanged(ConnectionError arg);
    void errorStringChanged(QString arg);
    void configChanged(QJsonObject arg);
    void motionChanged(QJsonObject arg);
    void ioChanged(QJsonObject arg);
    void taskChanged(QJsonObject arg);
    void interpChanged(QJsonObject arg);
    void channelsChanged(StatusChannels arg);
};

#endif // QEMCSTATUS_H
