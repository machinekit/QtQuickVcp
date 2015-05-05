# private qml files
PRIVATE_QML_FILES += \
    $$PWD/KnobPart.qml \
    $$PWD/KnobPie.qml

QML_FILES += $$PRIVATE_QML_FILES

QML_PRIVATE_INFRA_FILES += \
    $$PRIVATE_QML_FILES \
    $$PWD/qmldir
