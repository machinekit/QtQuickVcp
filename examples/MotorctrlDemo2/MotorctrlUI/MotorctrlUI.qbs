import qbs
import "~/projects/qbstest/machinekit/imports/MachinekitUI.qbs" as MachinekitUI

MachinekitUI {
        name: "MotorctrlUI"
        qmlFiles: ["MotorctrlDemo.qml"]
        targetDir: ""///home/alexander/motorctrl"
        importsDir: "/home/alexander/projects/build-QtQuickVcp-Qt_5_2_1_x86_64-Release/qmlhalplugin/imports/"
    } 
