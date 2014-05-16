include(../paths.pri)
include(../3rdparty/nzmqt/nzmqt.pri)

!isEmpty(ZEROMQ_LIB_PATH): LIBS += -L$$ZEROMQ_LIB_PATH
!isEmpty(ZEROMQ_INCLUDE_PATH): INCLUDEPATH += $$ZEROMQ_INCLUDE_PATH

LIBS += -Bstatic -lzmq
