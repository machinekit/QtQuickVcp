#include <qjdns.h>

QJDns::NameServer::NameServer()
{
}

QJDns::QJDns(QObject *parent)
    :QObject(parent)
{
}

QJDns::~QJDns()
{
}

bool QJDns::init(Mode mode, const QHostAddress &address)
{
    return true;
}

QStringList QJDns::debugLines()
{
    return QStringList();
}

QJDns::SystemInfo QJDns::systemInfo()
{
    return SystemInfo();
}

void QJDns::setNameServers(const QList<NameServer> &list)
{

}

int QJDns::queryStart(const QByteArray &name, int type)
{
    return 0;
}

void QJDns::queryCancel(int id)
{
}
