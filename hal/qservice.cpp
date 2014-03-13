#include "qservice.h"

QService::QService(QQuickItem *parent) :
    QQuickItem(parent)
{
    m_uri = "";
    m_version = 0;
    m_minVersion = 0;
    m_api = SA_ZMQ_PROTOBUF;
    m_description = "";
    m_found = false;
}

void QService::setData(QString uri, int version, QService::ServiceApi api, QString description, bool found)
{
    m_uri = uri;
    m_version = version;
    m_api = api;
    m_description = description;
    m_found = found;

    emit uriChanged(m_uri);
    emit versionChanged(m_version);
    emit apiChanged(m_api);
    emit descriptionChanged(m_description);
    emit foundChanged(m_found);
}

void QService::clearFound()
{
    m_found = false;
    emit foundChanged(m_found);
}
