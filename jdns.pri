# qmake project include file

QT *= network

DEFINES += JDNS_STATIC

windows:{
	LIBS += -lWs2_32 -lAdvapi32
}
unix:{
	#QMAKE_CFLAGS += -pedantic
}

HEADERS += \
	$$PWD/src/jdns_packet.h \
	$$PWD/src/jdns_mdnsd.h \
	$$PWD/src/jdns_p.h \
	$$PWD/src/jdns.h \
	$$PWD/src/qjdns_sock.h \
	$$PWD/src/qjdns_p.h \
	$$PWD/src/qjdns.h

SOURCES += \
	$$PWD/src/jdns_util.c \
	$$PWD/src/jdns_packet.c \
	$$PWD/src/jdns_mdnsd.c \
	$$PWD/src/jdns_sys.c \
	$$PWD/src/jdns.c \
	$$PWD/src/qjdns_sock.cpp \
	$$PWD/src/qjdns.cpp
