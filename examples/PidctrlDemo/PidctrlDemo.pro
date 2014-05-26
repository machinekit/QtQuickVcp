TEMPLATE = app

QT += qml quick widgets

SOURCES += main.cpp

RESOURCES += qml.qrc

# Additional import path used to resolve QML modules in Qt Creator's code model
#QML_IMPORT_PATH = $$relative_path($$OUT_PWD/../qmlhalplugin/imports, $$PWD)

# Add more folders to ship with the application, here
#folder_01.source = $$QML_IMPORT_PATH
#folder_01.target =
#DEPLOYMENTFOLDERS += folder_01

# Please do not modify the following two lines. Required for deployment.
#include(deployment.pri)
#qtcAddDeployment()
