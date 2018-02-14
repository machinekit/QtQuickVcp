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

QString createTestDir(const QString &rootPath, const QString &name)
{
    QDir dir(rootPath);
    dir.mkdir(name);

    return dir.filePath(name);
}

TEST_CASE("FileWatcher Tests", "[application][!mayfail]")
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

        WHEN("the watch is not enabled") {
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

    GIVEN ("we watch an existing directory with recursive option") {
        watcher.setFileUrl(QUrl::fromLocalFile(tempDir.path()));
        watcher.setRecursive(true);

        WHEN("the file watch is enabled") {
            watcher.setEnabled(true);

            AND_WHEN("a file is created") {
                const auto &filePath = tempDir.filePath("testfile.txt");
                writeTestFile(filePath);

                THEN ("we receive a file changed signal") {
                    changedSpy.wait(100);

                    REQUIRE(changedSpy.count() == 1);

                    AND_WHEN("the file is changed") {
                        changedSpy.clear();
                        writeTestFile(filePath);

                        THEN("we receive a file changed signal") {
                            changedSpy.wait(100);

                            REQUIRE(changedSpy.count() == 1);
                        }
                    }
                }
            }

            AND_WHEN("a directory is created") {
                const auto &dirPath = createTestDir(tempDir.path(), "testdir");

                THEN ("we receive a file changed signal") {
                    changedSpy.wait(100);

                    REQUIRE(changedSpy.count() == 1);

                    AND_WHEN("a file is created") {
                        changedSpy.clear();
                        const auto &filePath = QDir(dirPath).filePath("testfile.txt");
                        writeTestFile(filePath);

                        THEN("we receive a file changed signal") {
                            changedSpy.wait(100);

                            REQUIRE(changedSpy.count() == 1);
                        }
                    }
                }
            }

            AND_WHEN("a file in with extension from the nameFilters is created") {
                QStringList nameFilters;
                nameFilters.append("qmlc");
                watcher.setNameFilters(nameFilters);
                const auto &filePath = tempDir.filePath("testfile.qmlc");
                writeTestFile(filePath);

                THEN ("we receive a file changed signal") { // FIXME: find a way to ignore this too
                    changedSpy.wait(100);

                    REQUIRE(changedSpy.count() == 1);

                    AND_WHEN("the file is changed") {
                        changedSpy.clear();
                        writeTestFile(filePath);

                        THEN("we still don't receive a file changed signal") {
                            changedSpy.wait(100);

                            REQUIRE(changedSpy.count() == 0);
                        }
                    }
                }
            }
        }
    }
}
