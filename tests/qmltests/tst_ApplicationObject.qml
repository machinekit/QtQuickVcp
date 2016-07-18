import QtQuick 2.3
import QtTest 1.0
import Machinekit.Application 1.0

TestCase {
    name: "ApplicationTest"

    ApplicationObject {
        id: appObject
    }

    ApplicationCore {
        id: applicationCore
    }

    function test_applicationStatus() {
        compare(appObject.status, applicationCore.status, "test application status")
    }
}
