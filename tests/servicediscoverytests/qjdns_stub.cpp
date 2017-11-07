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

bool QJDns::init(Mode, const QHostAddress &)
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

void QJDns::setNameServers(const QList<NameServer> &)
{

}

int QJDns::queryStart(const QByteArray &, int)
{
    return 0;
}

void QJDns::queryCancel(int)
{
}
