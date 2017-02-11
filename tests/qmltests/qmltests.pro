TEMPLATE = app
TARGET = tst_qml
CONFIG += warn_on qmltestcase
SOURCES += tst_qml.cpp

QML_IMPORT_PATH += "$$PWD/../../app/PMCS_UI.PMCS UI"
IMPORTPATH += "$$PWD/../../app/PMCS_UI.PMCS UI"

DISTFILES += \
    tst_ApplicationObject.qml \
    tst_JogDistanceHandler.qml
