include(../paths.pri)
include(../3rdparty/nzmqt/nzmqt.pri)

!isEmpty(ZEROMQ_INCLUDE_PATH): INCLUDEPATH += $$ZEROMQ_INCLUDE_PATH
!isEmpty(ZEROMQ_LIB_PATH): LIBS += -L$$ZEROMQ_LIB_PATH
!isEmpty(ZEROMQ_LIB_FLAGS): LIBS += $$ZEROMQ_LIB_FLAGS
LIBS += -lzmq

ios: {
    LIBS += -L$$LIBSODIUM_LIB_PATH
    LIBS += -Bstatic -lsodium
}
