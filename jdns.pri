# qmake project include file

QT *= network

DEFINES += JDNS_STATIC
INCLUDEPATH += $$PWD/include/jdns

windows:{
	LIBS += -lWs2_32 -lAdvapi32
}
unix:{
    #QMAKE_CFLAGS += -pedantic
}
android: {
DEFINES += JDNS_MODERN_RES_API
DEFINES += USE_EXTEXT
DEFINES += JDNS_OS_ANDROID

SOURCES +=  $$PWD/android/ev_streams.c \
            $$PWD/android/ev_timers.c \
            $$PWD/android/getaddrinfo.c \
            $$PWD/android/ns_name.c \
            $$PWD/android/ns_netint.c \
            $$PWD/android/ns_parse.c \
            $$PWD/android/ns_samedomain.c \
            $$PWD/android/ns_ttl.c \
            $$PWD/android/res_comp.c \
            $$PWD/android/res_data.c \
            $$PWD/android/res_init.c \
            $$PWD/android/res_send.c \
            $$PWD/android/ns_print.c \
            $$PWD/android/res_debug.c \
            $$PWD/android/res_mkquery.c \
            $$PWD/android/res_state.c

HEADERS +=  $$PWD/android/arpa_nameser.h \
            $$PWD/android/arpa_nameser_compat.h \
            $$PWD/android/assertions.h \
            $$PWD/android/dst.h \
            $$PWD/android/eventlib.h \
            $$PWD/android/eventlib_p.h \
            $$PWD/android/heap.h \
            $$PWD/android/linker_format.h \
            $$PWD/android/list.h \
            $$PWD/android/memcluster.h \
            $$PWD/android/res_debug.h \
            $$PWD/android/res_private.h \
            $$PWD/android/resolv_cache.h \
            $$PWD/android/resolv_private.h \
            $$PWD/android/resolv_static.h

INCLUDEPATH += $$PWD/android/
}

HEADERS += \
	$$PWD/src/jdns/jdns_packet.h \
	$$PWD/src/jdns/jdns_mdnsd.h \
	$$PWD/src/jdns/jdns_p.h \
	$$PWD/include/jdns/jdns.h \
	$$PWD/src/qjdns/qjdns_sock.h \
	$$PWD/src/qjdns/qjdns_p.h \
	$$PWD/src/qjdns/qjdnsshared_p.h \
	$$PWD/include/jdns/qjdns.h \
	$$PWD/include/jdns/qjdnsshared.h \
	$$PWD/include/jdns/jdns_export.h

SOURCES += \
	$$PWD/src/jdns/jdns_util.c \
	$$PWD/src/jdns/jdns_packet.c \
	$$PWD/src/jdns/jdns_mdnsd.c \
	$$PWD/src/jdns/jdns_sys.c \
	$$PWD/src/jdns/jdns.c \
	$$PWD/src/qjdns/qjdns_sock.cpp \
	$$PWD/src/qjdns/qjdns.cpp \
	$$PWD/src/qjdns/qjdnsshared.cpp
