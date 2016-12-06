MACHINETALK_PATH = $$OUT_PWD/../machinetalk
INCLUDEPATH += $$PWD

!win32 {
    LIBS += -L$$MACHINETALK_PATH
} else {
    LIBS += -L$$MACHINETALK_PATH/release
    LIBS += -L$$MACHINETALK_PATH/debug
}
#macx: {
#    LIBS += -Bstatic
#}
LIBS += -Bstatic
LIBS += -lmachinetalk
