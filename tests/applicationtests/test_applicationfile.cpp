#include <catch.hpp>
#include <trompeloeil.hpp>

#include <QtTest>
#include <QTemporaryDir>

#include <applicationfile.h>

using namespace qtquickvcp;

class ApplicationFileTest: public ApplicationFile {
public:
    QString applicationFilePath(const QString &fileName, const QString &serverDirectory) const
    {
        return ApplicationFile::applicationFilePath(fileName, serverDirectory);
    }
};

TEST_CASE("Application File Tests", "[application]")
{
    ApplicationFileTest file;

    SECTION("applicationFilePath is determined corretly") {
        file.setLocalPath("file:///tmp/Machinekit-1234");

        const auto &path = file.applicationFilePath("example.ngc", "examples");

        REQUIRE(path.toStdString() == "/tmp/Machinekit-1234/examples/example.ngc");
    }
}
