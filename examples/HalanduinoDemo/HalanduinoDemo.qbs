import qbs

CppApplication {
    name: "HalanduinoDemo"
    Depends { name: "Qt"; submodules: ["core", "gui", "qml", "widgets", "quick"] }
    files: ["main.cpp",
            "qml.qrc",
            "main.qml",
            "HalanduinoDemo.Control/*"]
} 
