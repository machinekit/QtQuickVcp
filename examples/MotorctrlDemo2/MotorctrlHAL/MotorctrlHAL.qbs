import qbs
import "~/projects/qbstest/machinekit/imports/MachineKitApplication.qbs" as MachineKitApplication

MachineKitApplication {
        name: "MotorctrlDemo"
        configFiles: ["haltalk.ini"]
        halFiles: ["motorctrl.hal"]
        //bbioFiles: ["paralell_cape2.bbio"]

        targetDir: "/home/linuxcnc/motorctrl"
        applicationType: "hal"
    } 
