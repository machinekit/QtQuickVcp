import QtQuick 2.3
import QtTest 1.0
import Machinekit.Application.Controls.Private 1.0

TestCase {
    name: "LauncherPageTests"

    LauncherPage {
        id: launcherPage
    }

    property var launcherData: [
        {"id": 0, "running": false, "importance": 1},
        {"id": 1, "running": false, "importance": 0},
        {"id": 2, "running": false, "importance": 0},
        {"id": 3, "running": true, "importance": 0},
        {"id": 4, "running": false, "importance": 1},
    ]

    function test_filterLaunchersCreatesCopy()
    {
        var filtered = launcherPage.__filterLaunchers(launcherData);

        verify(launcherData !== filtered);
    }

    function test_filterLaunchersPrioritizesRunningAndImportantInCorrectOrder()
    {
        var filtered = launcherPage.__filterLaunchers(launcherData);

        compare(filtered[0].id, 3);
        compare(filtered[1].id, 0);
        compare(filtered[2].id, 4);
    }
}
