include(../paths.pri)
include(../3rdparty/nzmqt/nzmqt.pri)

!isEmpty(ZEROMQ_INCLUDE_PATH): INCLUDEPATH += $$ZEROMQ_INCLUDE_PATH
!isEmpty(ZEROMQ_LIB_PATH):win32:CONFIG(release, debug|release): LIBS += -L$$ZEROMQ_LIB_PATH/release
else:!isEmpty(ZEROMQ_LIB_PATH):win32:CONFIG(debug, debug|release): LIBS += -L$$ZEROMQ_LIB_PATH/debug
!isEmpty(ZEROMQ_LIB_FLAGS): LIBS += $$ZEROMQ_LIB_FLAGS

!win32: LIBS += -lzmq
win32:  LIBS += -llibzmq

ios: {
    LIBS += -L$$LIBSODIUM_LIB_PATH
    LIBS += -Bstatic -lsodium
}
