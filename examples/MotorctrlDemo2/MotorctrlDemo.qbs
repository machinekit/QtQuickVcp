import qbs 1.0
import "~/projects/qbstest/machinekit/imports/MachinekitUI.qbs" as MachinekitUI
import "~/projects/qbstest/machinekit/imports/MachineKitApplication.qbs" as MachineKitApplication

Project {
    name: "MotorctrlDemo"

    MachineKitApplication {
            name: "MotorctrlHAL"
            configFiles: ["MotorctrlHAL/haltalk.ini"]
            halFiles: ["MotorctrlHAL/motorctrl.hal"]
            //bbioFiles: ["MotorctrlHAL/paralell_cape2.bbio"]

            targetDir: "/home/linuxcnc/motorctrl"
            applicationType: "hal"
        }

    MachinekitUI {
            name: "MotorctrlUI"
            qmlFiles: ["MotorctrlUI/MotorctrlDemo.qml"]
            targetDir: ""///home/alexander/motorctrl"
            importsDir: "/home/alexander/projects/build-QtQuickVcp-Qt_5_2_1_x86_64-Release/qmlhalplugin/imports/"
        }
}
 
