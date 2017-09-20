#ifndef LOGSERVICE_H
#define LOGSERVICE_H

#include <QObject>
#include <application/logservicebase.h>

class LogService : public machinetalk::application::LogServiceBase
{
    Q_OBJECT

public:
    explicit LogService(QObject *parent  = nullptr);
};

#endif // LOGSERVICE_H
