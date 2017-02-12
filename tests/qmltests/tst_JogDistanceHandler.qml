import QtQuick 2.3
import QtTest 1.0
import Machinekit.Application.Controls 1.0

TestCase {
    name: "JogDistanceHandlerTest"

    JogDistanceHandler {
        id: handler
    }

    function createStatus(increments) {
        return {
            "config" : {
                "increments": increments,
                "axis": [
                    {
                        "increments": increments
                    },
                    {
                        "increments": increments
                    },
                ]
            }
        };
    }

    function test_parseIncrementItem() {
        var testData = [];
        testData.push([10.0, "10 mm"]);
        testData.push([25.4, "1 inch"]);
        testData.push([0.0223, "22.3 um"]);
        testData.push([32.0, "32mm"]);
        testData.push([573.4, "57.34  cm"]);
        testData.push([25.4 * 0.001 * 340.0, "340mil"]);
        testData.push([1.0, "1.0"])
        testData.push([0.25, "1/4"])

        for (var i = 0; i < testData.length; ++i) {
            var result = handler._parseIncrementItem(testData[i][1]);
            compare(result, testData[i][0]);
        }
    }

    function test_parseIncrements() {
        var axis;
        var status;
        var result;

        axis = -1;
        status = createStatus("1 2 3 5 6");
        result = handler._parseIncrements(axis, status);
        compare(result.increments.length, 5);
        compare(result.distances.length, 5);
        compare(result.distances[2], 3.0);
        compare(result.increments[2], "3");

        axis = 1;
        status = createStatus("12in, 3.2 mm, 4um, 1mil");
        result = handler._parseIncrements(axis, status);
        compare(result.increments.length, 4);
        compare(result.distances.length, 4);
        compare(result.distances[1], 3.2);
        compare(result.increments[1], "3.2 mm");
        compare(result.distances[3], 0.0254);
        compare(result.increments[3], "1mil");
    }
}
