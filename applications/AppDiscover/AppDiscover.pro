TEMPLATE = app

QT += qml quick widgets

SOURCES += main.cpp

RESOURCES += qml.qrc

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH = $$relative_path($$OUT_PWD/../../imports, $$PWD)

# Add more folders to ship with the application, here
#folder_01.source = $$QML_IMPORT_PATH
#folder_01.target =
#folder_02.source = qml
#folder_02.target =
#DEPLOYMENTFOLDERS += folder_01

# Please do not modify the following two lines. Required for deployment.
#include(deployment.pri)
#qtcAddDeployment()

ANDROID_EXTRA_LIBS += \
    $$OUT_PWD/../../imports/Machinekit/Controls/libmachinekitcontrolsplugin.so \
    $$OUT_PWD/../../imports/Machinekit/HalRemote/libmachinekithalremoteplugin.so \
    $$OUT_PWD/../../imports/Machinekit/HalRemote/Controls/libmachinekithalremotecontrols.so \
    $$OUT_PWD/../../imports/Machinekit/PathView/libmachinekitpathview.so \
    $$OUT_PWD/../../imports/Machinekit/VideoView/libmachinekitvideoview.so

# deactivate installs for applications
INSTALLS =
