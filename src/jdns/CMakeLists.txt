set(jdns_SRCS
    jdns.c
    jdns_mdnsd.c
    jdns_packet.c
    jdns_sys.c
    jdns_util.c
)


set(jdns_PUBLIC_HEADERS
    "${JDNS_INCLUDEDIR}/jdns.h"
    "${JDNS_INCLUDEDIR}/jdns_export.h"
)

set(jdns_HEADERS
    jdns_packet.h
    jdns_mdnsd.h
    jdns_p.h
)

add_library(jdns ${jdns_SRCS} ${jdns_HEADERS} ${jdns_PUBLIC_HEADERS})

if(WIN32)
  target_link_libraries(jdns Ws2_32 Advapi32)
endif(WIN32)

if(NOT android)
  set_target_properties(jdns PROPERTIES
                        VERSION ${JDNS_LIB_MAJOR_VERSION}.${JDNS_LIB_MINOR_VERSION}.${JDNS_LIB_PATCH_VERSION}
                        SOVERSION ${JDNS_LIB_MAJOR_VERSION}
  )
endif()
set_target_properties(jdns PROPERTIES
                      DEFINE_SYMBOL JDNS_MAKEDLL
                      PUBLIC_HEADER "${jdns_PUBLIC_HEADERS}"
#                     FRAMEWORK ${OSX_FRAMEWORK}
)

install(TARGETS jdns EXPORT jdns-export
        LIBRARY DESTINATION ${LIB_INSTALL_DIR}
        RUNTIME DESTINATION ${BIN_INSTALL_DIR}
        ARCHIVE DESTINATION ${LIB_INSTALL_DIR}
#       FRAMEWORK DESTINATION ${FRAMEWORK_INSTALL_DIR}
        PUBLIC_HEADER DESTINATION "${INCLUDE_INSTALL_DIR}/jdns"
)

if(MSVC)
  get_target_property(LOCATION jdns LOCATION_DEBUG)
  string(REGEX REPLACE "\\.[^.]*$" ".pdb" LOCATION "${LOCATION}")
  install(FILES ${LOCATION} DESTINATION ${LIB_INSTALL_DIR} CONFIGURATIONS Debug)

  get_target_property(LOCATION jdns LOCATION_RELWITHDEBINFO)
  string(REGEX REPLACE "\\.[^.]*$" ".pdb" LOCATION "${LOCATION}")
  install(FILES ${LOCATION} DESTINATION ${LIB_INSTALL_DIR} CONFIGURATIONS RelWithDebInfo)
endif(MSVC)
