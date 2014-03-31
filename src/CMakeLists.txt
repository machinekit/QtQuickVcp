set(qjdns_MOC_HDRS
    qjdns.h
    qjdns_p.h
)

set(jdns_app_MOC_HDRS
    main.h
)

set(qjdns_HDRS
    jdns_packet.h
    jdns_mdnsd.h
    jdns.h
    qjdns_sock.h
)

if(NOT Qt5Core_FOUND)
  qt4_wrap_cpp(qjdns_MOC_SRCS ${qjdns_MOC_HDRS})
  qt4_wrap_cpp(jdns_app_MOC_SRCS ${jdns_app_MOC_HDRS})
endif()

set(qjdns_SRCS
    qjdns.cpp
    qjdns_sock.cpp
    jdns.c
    jdns_mdnsd.c
    jdns_packet.c
    jdns_sys.c
    jdns_util.c
)

set(qjdns_HEADERS
    jdns_packet.h
    jdns_mdnsd.h
    jdns_p.h
    jdns.h
    qjdns_sock.h
    qjdns_export.h
)

add_library (qjdns SHARED ${qjdns_SRCS} ${qjdns_MOC_SRCS} ${qjdns_HEADERS})
if(Qt5Core_FOUND)
  target_link_libraries(qjdns ${Qt5Core_LIBRARIES} ${Qt5Network_LIBRARIES})
else(Qt5Core_FOUND)
  target_link_libraries(qjdns ${QT_LIBRARIES})
endif(Qt5Core_FOUND)

if(WIN32)
  target_link_libraries(qjdns Ws2_32 Advapi32)
endif(WIN32)

add_executable(jdns main.cpp ${jdns_app_MOC_SRCS})
target_link_libraries(jdns qjdns)

if(NOT android)
  set_target_properties(qjdns PROPERTIES
                        VERSION ${QJDNS_LIB_MAJOR_VERSION}.${QJDNS_LIB_MINOR_VERSION}.${QJDNS_LIB_PATCH_VERSION}
                        SOVERSION ${QJDNS_LIB_MAJOR_VERSION}
  )
endif()
set_target_properties(qjdns PROPERTIES
                      DEFINE_SYMBOL QJDNS_MAKEDLL
                      PUBLIC_HEADER qjdns.h
#                     FRAMEWORK ${OSX_FRAMEWORK}
)

install(TARGETS qjdns jdns EXPORT qjdns-export
        LIBRARY DESTINATION ${LIB_INSTALL_DIR}
        RUNTIME DESTINATION ${CMAKE_INSTALL_PREFIX}/bin
        ARCHIVE DESTINATION ${LIB_INSTALL_DIR}
#       FRAMEWORK DESTINATION ${FRAMEWORK_INSTALL_DIR}
        PUBLIC_HEADER DESTINATION ${INCLUDE_INSTALL_DIR}
)

if(MSVC)
  get_target_property(LOCATION qjdns LOCATION_DEBUG)
  string(REGEX REPLACE "\\.[^.]*$" ".pdb" LOCATION "${LOCATION}")
  install(FILES ${LOCATION} DESTINATION ${CMAKE_INSTALL_PREFIX}/bin CONFIGURATIONS Debug)

  get_target_property(LOCATION qjdns LOCATION_RELWITHDEBINFO)
  string(REGEX REPLACE "\\.[^.]*$" ".pdb" LOCATION "${LOCATION}")
  install(FILES ${LOCATION} DESTINATION ${CMAKE_INSTALL_PREFIX}/bin CONFIGURATIONS RelWithDebInfo)
endif(MSVC)
