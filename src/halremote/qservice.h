#ifndef QSERVICE_H
#define QSERVICE_H

#include <QObject>
#include "message.pb.h"

class QService : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString uri READ uri NOTIFY uriChanged)
    Q_PROPERTY(int version READ version NOTIFY versionChanged)
    Q_PROPERTY(ServiceApi api READ api NOTIFY apiChanged)
    Q_PROPERTY(QString description READ description NOTIFY descriptionChanged)
    Q_PROPERTY(bool found READ isFound NOTIFY foundChanged)
    Q_PROPERTY(int minVersion READ minVersion WRITE setMinVersion NOTIFY minVersionChanged)
    Q_PROPERTY(ServiceType type READ type WRITE setType NOTIFY typeChanged)
    Q_ENUMS(ServiceApi)
    Q_ENUMS(ServiceType)

public:
    explicit QService(QObject *parent = 0);

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

    QString uri() const
    {
        return m_uri;
    }

    int version() const
    {
        return m_version;
    }

    ServiceApi api() const
    {
        return m_api;
    }

    QString description() const
    {
        return m_description;
    }

    bool isFound() const
    {
        return m_found;
    }

    int minVersion() const
    {
        return m_minVersion;
    }

    ServiceType type() const
    {
        return m_type;
    }

public slots:

    void setData(QString uri, int version, ServiceApi api, QString description, bool found);
    void clearFound();

    void setMinVersion(int arg)
    {
        if (m_minVersion != arg) {
            m_minVersion = arg;
            emit minVersionChanged(arg);
        }
    }

    void setType(ServiceType arg)
    {
        if (m_type != arg) {
            m_type = arg;
            emit typeChanged(arg);
        }
    }

private:
    QString m_uri;
    int m_version;
    ServiceApi m_api;
    QString m_description;
    bool m_found;
    int m_minVersion;
    ServiceType m_type;

signals:

    void uriChanged(QString arg);
    void versionChanged(int arg);
    void apiChanged(ServiceApi arg);
    void descriptionChanged(QString arg);
    void foundChanged(bool arg);
    void minVersionChanged(int arg);
    void typeChanged(ServiceType arg);
};

#endif // QSERVICE_H
