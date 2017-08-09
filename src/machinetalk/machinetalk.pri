MACHINETALK_PATH = $$OUT_PWD/../machinetalk
INCLUDEPATH += $$PWD

!win32 {
    LIBS += -L$$MACHINETALK_PATH
    LIBS += -Bstatic
} else {
    CONFIG(release, debug|release): LIBS += -L$$MACHINETALK_PATH/release
    CONFIG(debug, debug|release): LIBS += -L$$MACHINETALK_PATH/debug
}
LIBS += -lmachinetalk
