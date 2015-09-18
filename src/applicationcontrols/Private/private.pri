# private qml files
PRIVATE_QML_FILES += \
    $$PWD/NetworkPage.qml \
    $$PWD/InstancePage.qml \
    $$PWD/LauncherPage.qml \
    $$PWD/SelectedPage.qml \
    $$PWD/ConfigPage.qml \
    $$PWD/AppPage.qml \
    $$PWD/LoadingPage.qml \
    $$PWD/ErrorPage.qml

QML_FILES += $$PRIVATE_QML_FILES

QML_PRIVATE_INFRA_FILES += \
    $$PRIVATE_QML_FILES \
    $$PWD/qmldir

RESOURCES += \
    $$PWD/applicationcontrolsprivate.qrc

HEADERS += \
    $$PWD/pluginprivate.h

INCLUDEPATH += \
    $$PWD
