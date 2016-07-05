import qbs

MachinekitApplication {
    name: "Halanduino"
    halFiles: ["Halanduino.hal"]
    configFiles: ["machinekit.ini"]
    //bbioFiles: ["ServoControl.bbio"]
    //machinekitIni: "machinekit.ini"
    type: "machinekit"
    uis: ["../HalanduinoDemo.Control"]
}
