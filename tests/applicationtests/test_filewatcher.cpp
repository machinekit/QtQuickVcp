#include <catch.hpp>
#include <trompeloeil.hpp>

#include <QString>
#include <QtTest>
#include <QTemporaryDir>
#include <filewatcher.h>

using namespace qtquickvcp;

void writeTestFile(const QString filePath)
{
    QFile file(filePath);
    file.open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Append);
    file.write("test");
    file.close();
}

TEST_CASE("File watcher tests", "[application]")
{
    QTemporaryDir tempDir;
    FileWatcher watcher;
    QSignalSpy changedSpy(&watcher, &FileWatcher::fileChanged);

    GIVEN ("we watch an existing file") {
        const auto &filePath = tempDir.filePath("testfile.txt");
        writeTestFile(filePath);
        watcher.setFileUrl(QUrl::fromLocalFile(filePath));

        WHEN("the file watcher is enabled") {
            watcher.setEnabled(true);

            AND_WHEN("the file is modified") {
                writeTestFile(filePath);

                THEN("we receive a file changed signal") {
                    changedSpy.wait(100);

                    REQUIRE(changedSpy.count() == 1);
                }
            }
        }

        WHEN("the watch is not disabled") {
            watcher.setEnabled(false);

            AND_WHEN("the file is modified") {
                writeTestFile(filePath);

                THEN("we don't receive a file changed signal") {
                    changedSpy.wait(100);

                    REQUIRE(changedSpy.count() == 0);
                }
            }
        }
    }
}
