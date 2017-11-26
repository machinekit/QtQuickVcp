#include <catch.hpp>
#include <trompeloeil.hpp>

#include <QtTest>
#include <QTemporaryDir>

#include <applicationfilesynchandler.h>

using namespace qtquickvcp;

TEST_CASE("Application File Sync Handler", "[application]")
{
    ApplicationFileSyncHandler handler;
    QSignalSpy startFileDownloadSpy(&handler, &ApplicationFileSyncHandler::startFileDownload);
    QSignalSpy programUnloadedSpy(&handler, &ApplicationFileSyncHandler::programUnloaded);

    GIVEN("The remote file path and the remote path are empty and the handler is ready") {
        handler.setRemoteFilePath("file://");
        handler.setRemotePath("file://");
        handler.setReady(true);

        WHEN("A valid remote file path is set") {
            handler.setRemoteFilePath("file:///home/machinekit/nc_files/test.ngc");

            THEN("No signal is triggered") {
                REQUIRE(startFileDownloadSpy.count() == 0);
                REQUIRE(programUnloadedSpy.count() == 0);
            }

            AND_WHEN("A valid remote path is set") {
                handler.setRemotePath("file:///home/machinekit/nc_files/");

                THEN("The file download is triggered") {
                    REQUIRE(startFileDownloadSpy.count() == 1);
                }

                AND_WHEN("The remote path is updated and valid") {
                    handler.setRemotePath("file:///home/machinekit/");
                    startFileDownloadSpy.clear();

                    THEN("No new file download is triggered") {
                        REQUIRE(startFileDownloadSpy.count() == 0);
                    }
                }
            }
        }
    }

    GIVEN("The remote file path and remote path are valid") {
        handler.setRemoteFilePath("file:///home/machinekit/nc_files/arc.ngc");
        handler.setRemotePath("file:///home/machinekit/nc_files/");

        WHEN("The handler gets ready") {
            startFileDownloadSpy.clear();
            handler.setReady(true);

            THEN("A file download is triggered") {
                REQUIRE(startFileDownloadSpy.count() == 1);
            }

        AND_WHEN("The remote file path is changed to something valid") {
            startFileDownloadSpy.clear();
            handler.setRemoteFilePath("file:///home/machinekit/nc_files/circle.ngc");

            THEN("A file download is triggered") {
                REQUIRE(startFileDownloadSpy.count() == 1);
            }
        }

        AND_WHEN("Ignore next change is active") {
            handler.setIgnoreNextChange(true);

            AND_WHEN("The remote file path is changed") {
                startFileDownloadSpy.clear();
                handler.setRemoteFilePath("file:///home/machinekit/nc_files/tux.ngc");

                THEN("No file download is triggered") {
                    REQUIRE(startFileDownloadSpy.count() == 0);
                }
            }
        }

        AND_WHEN("The remote file path is changed to an empty path") {
            programUnloadedSpy.clear();
            handler.setRemoteFilePath("file://");

            THEN("Unload program is signaled") {
                REQUIRE(programUnloadedSpy.count() == 1);
            }
        }

        AND_WHEN("The remote file path is changed to something not in the remote path") {
            startFileDownloadSpy.clear();
            handler.setRemoteFilePath("file://something.ngc");

            THEN("No file download is triggered") {
                REQUIRE(startFileDownloadSpy.count() == 0);
            }

            AND_WHEN("The file path is changed back to the original file path") {
                startFileDownloadSpy.clear();
                handler.setRemoteFilePath("file:///home/machinekit/nc_files/arc.ngc");

                THEN("No file download is triggered") {
                    REQUIRE(startFileDownloadSpy.count() == 0);
                }
            }
        }
        }
    }
}
