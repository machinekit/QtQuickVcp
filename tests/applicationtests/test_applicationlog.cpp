#include <catch.hpp>
#include <trompeloeil.hpp>

#include <QtTest>
#include <QTemporaryDir>
#include <applicationlog.h>
#include <applicationlogmessage.h>
#include "logservice.h"

using namespace qtquickvcp;

//Q_DECLARE_METATYPE(ApplicationLogMessage);

void registerMetaTypes() {
    qRegisterMetaType<ApplicationLogMessage*>("ApplicationLogMessage");
}

TEST_CASE("Application Log Tests", "[application]")
{
    registerMetaTypes();

    auto logClient = std::make_shared<ApplicationLog>();
    logClient->setLogUri("tcp://127.0.0.1:12346");
    logClient->componentComplete();

    auto logService = std::make_shared<LogService>();
    logService->setLogUri(logClient->logUri());
    logService->componentComplete();

    GIVEN("The log client is running and connected") {
        logService->setReady(true);
        logClient->setReady(true);
        // would wait here if we would have a reliable protocol

        WHEN("we send a log message via the log service") {
            QSignalSpy spy(logClient.get(), &ApplicationLog::messageReceived);

            machinetalk::Container tx;
            auto message = tx.mutable_log_message();
            message->set_level(machinetalk::RTAPI_MSG_ERR);
            message->set_origin(machinetalk::MSG_RTUSER);
            message->set_pid(10);
            message->set_tag("tagname");
            message->set_text("error");
            logService->sendLogMessage("log", tx);

            THEN("we receive a log message at the log client") {
                spy.wait(100);
                REQUIRE(spy.count() == 1);
                auto log = qvariant_cast<QJsonObject>(spy.at(0).at(0));
                REQUIRE(log["level"] == ApplicationLog::Error);
            }
        }
    }
}
