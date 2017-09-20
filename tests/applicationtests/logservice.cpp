#include "logservice.h"
#include <machinetalk/protobuf/message.pb.h>
#include <google/protobuf/text_format.h>

using namespace machinetalk;

LogService::LogService(QObject *parent) :
    application::LogServiceBase(parent)
{

}
