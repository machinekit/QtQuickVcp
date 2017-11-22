TEMPLATE = app
TARGET = qmltests
CONFIG += warn_on qmltestcase
CONFIG -= app_bundle
SOURCES += tst_qml.cpp

QML_IMPORT_PATH += "$$PWD/../../app/PMCS_UI.PMCS UI"
IMPORTPATH += "$$PWD/../../app/PMCS_UI.PMCS UI"

DISTFILES += \
    tst_ApplicationObject.qml \
    tst_JogDistanceHandler.qml \
    tst_LauncherPage.qml
