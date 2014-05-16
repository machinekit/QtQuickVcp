#Directories

#TOP = /home/alexander/projects/
#LINUXCNCDIR = $$TOP/linuxcnc

# eventually import directly from linuxcnc build dir
#PROTOGEN = $$LINUXCNCDIR/src/middleware/generated

include(../paths.pri)
include(../3rdparty/nzmqt/nzmqt.pri)

LIBS += -L$$ZEROMQ_LIB_PATH
INCLUDEPATH += $$ZEROMQ_INCLUDE_PATH

LIBS += -lzmq
