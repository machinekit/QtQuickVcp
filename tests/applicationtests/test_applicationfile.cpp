#include <catch.hpp>
#include <trompeloeil.hpp>

#include <QtTest>
#include <QTemporaryDir>

#define private public
#include <applicationfile.h>

using namespace qtquickvcp;

TEST_CASE("Application File Tests", "[application]")
{
    ApplicationFile file;

    SECTION("applicationFilePath is determined corretly") {
        file.setLocalPath("file:///tmp/Machinekit-1234");

        const auto &path = file.applicationFilePath("example.ngc", "examples");

        REQUIRE(path.toStdString() == "/tmp/Machinekit-1234/examples/example.ngc");
    }
}
