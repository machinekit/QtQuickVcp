#ifndef QSERVICEDISCOVERY_H
#define QSERVICEDISCOVERY_H

#include <QQuickItem>
#include <QUdpSocket>
#include <QTimer>
#include <QTime>
#include <nzmqt/nzmqt.hpp>
#include "message.pb.h"

using namespace nzmqt;

class QServiceDiscovery : public QQuickItem
{
    Q_OBJECT
    Q_PROPERTY(int port READ port WRITE setPort NOTIFY portChanged)
    Q_PROPERTY(int instance READ instance WRITE setInstance NOTIFY instanceChanged)
    Q_PROPERTY(int retryTime READ retryTime WRITE setRetryTime NOTIFY retryTimeChanged)
    Q_PROPERTY(int maxWait READ maxWait WRITE setMaxWait NOTIFY maxWaitChanged)
    Q_PROPERTY(bool trace READ trace WRITE setTrace NOTIFY traceChanged)
    Q_PROPERTY(bool running READ isRunning NOTIFY runningChanged)
    //Q_PROPERTY(QMap services READ services NOTIFY servicesChanged)
    Q_ENUMS(ServiceApi)
    Q_ENUMS(ServiceType)

public:
    explicit QServiceDiscovery(int port = SERVICE_DISCOVERY_PORT,
                               int instance = 0,
                               int retry = RETRY,
                               int maxWait = MAX_WAIT,
                               bool trace = false,
                               QObject *parent = 0);


    enum ServiceApi {
        SA_ZMQ_PROTOBUF = pb::SA_ZMQ_PROTOBUF,
        SA_WS_JSON = pb::SA_WS_JSON
    };

    enum ServiceType {
        ST_LOGGING = pb::ST_LOGGING,
        ST_CONFIG = pb::ST_CONFIG,
        ST_REDIS = pb::ST_REDIS,
        ST_HTTP = pb::ST_HTTP,
        ST_HTTPS = pb::ST_HTTPS,
        ST_WEBSOCKET = pb::ST_WEBSOCKET,
        ST_WEBSOCKETS = pb::ST_WEBSOCKETS,
        ST_RTAPI_COMMAND = pb::ST_RTAPI_COMMAND,
        ST_STP_HALGROUP = pb::ST_STP_HALGROUP,
        ST_STP_HALRCOMP = pb::ST_STP_HALRCOMP,
        ST_STP_INTERP = pb::ST_STP_INTERP,
        ST_STP_TASK = pb::ST_STP_TASK,
        ST_HAL_RCOMMAND = pb::ST_HAL_RCOMMAND,
        ST_TASK_COMMAND = pb::ST_TASK_COMMAND,
        ST_INTERP_COMMAND = pb::ST_INTERP_COMMAND,
        ST_MESSAGEBUS_COMMAND = pb::ST_MESSAGEBUS_COMMAND,
        ST_MESSAGEBUS_RESPONSE = pb::ST_MESSAGEBUS_RESPONSE
    };

    typedef struct ServiceStruct{
        QString uri = "";
        int version = 0;
        int minVersion = 0;
        ServiceApi api = SA_ZMQ_PROTOBUF;
        QString description = "";
        bool found = false;

        bool operator =(ServiceStruct const &rhs) {
            this->uri = rhs.uri;
            this->version = rhs.version;
            this->minVersion = rhs.minVersion;
            this->api = rhs.api;
            this->description = rhs.description;
            this->found = rhs.found;
            return true;
        }

        bool operator ==(ServiceStruct const &rhs) {
            return (this->uri == rhs.uri)
            && (this->version == rhs.version)
            && (this->minVersion == rhs.minVersion)
            && (this->api == rhs.api)
            && (this->description == rhs.description)
            && (this->found == rhs.found);
        }
    } Service;

    static const int SERVICE_DISCOVERY_PORT = 10042;
    //static const QString BROADCAST_IP;
    static const int RETRY = 500;
    static const double MAX_WAIT = 2.0;

    void addService(ServiceType type, int minVersion = 0);
    void removeService(ServiceType type);
    void startDiscovery();
    void stopDiscovery();
    void sendBroadcast();

    int port() const
    {
        return m_port;
    }

    int instance() const
    {
        return m_instance;
    }

    int retryTime() const
    {
        return m_retryTime;
    }

    int maxWait() const
    {
        return m_maxWait;
    }

    bool trace() const
    {
        return m_trace;
    }

    QMap<ServiceType, Service> services() const
    {
        return m_services;
    }

    QMap<ServiceType, Service> replies() const
    {
        return m_replies;
    }

    bool isRunning() const
    {
        return m_running;
    }

private:
    int m_wanted;
    int m_currentWanted;
    int m_port;
    int m_instance;
    int m_retryTime;
    int m_maxWait;
    bool m_trace;
    QMap<ServiceType, Service> m_services;
    QMap<ServiceType, Service> m_replies;

    QString m_probe;
    QUdpSocket *m_udpSocket;
    QTimer *m_timeoutTimer;
    QTime *m_waitTime;
   // ZMQContext *m_context;
    // ZMQSocket *m_socket;

    bool m_running;

private slots:
void udpReadyRead();
void timeout();

signals:
    void discoveryTimeout();
    void discoverySucceded();

    void uriChanged(QString arg);
    void versionChanged(int arg);
    void apiChanged(ServiceApi arg);
    void descriptionChanged(QString arg);
    void foundChanged(bool arg);
    void portChanged(int arg);
    void instanceChanged(int arg);
    void retryTimeChanged(int arg);
    void maxWaitChanged(int arg);
    void traceChanged(bool arg);
    void servicesChanged(QMap<ServiceType, Service> arg);
    void repliesChanged(QMap<ServiceType, Service> arg);
    void runningChanged(bool arg);

public slots:

void setPort(int arg)
{
    if (m_port != arg) {
        m_port = arg;
        emit portChanged(arg);
    }
}
void setInstance(int arg)
{
    if (m_instance != arg) {
        m_instance = arg;
        emit instanceChanged(arg);
    }
}
void setRetryTime(int arg)
{
    if (m_retryTime != arg) {
        m_retryTime = arg;
        emit retryTimeChanged(arg);
    }
}
void setMaxWait(int arg)
{
    if (m_maxWait != arg) {
        m_maxWait = arg;
        emit maxWaitChanged(arg);
    }
}
void setTrace(bool arg)
{
    if (m_trace != arg) {
        m_trace = arg;
        emit traceChanged(arg);
    }
}
};

#endif // QSERVICEDISCOVERY_H
