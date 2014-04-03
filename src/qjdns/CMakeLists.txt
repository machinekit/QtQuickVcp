set(qjdns_MOC_HDRS
    "${JDNS_INCLUDEDIR}/qjdns.h"
    "${JDNS_INCLUDEDIR}/qjdnsshared.h"
    qjdns_p.h
    qjdnsshared_p.h
)

if(NOT Qt5Core_FOUND)
  qt4_wrap_cpp(qjdns_MOC_SRCS ${qjdns_MOC_HDRS})
endif()

set(qjdns_SRCS
    qjdns.cpp
    qjdns_sock.cpp
    qjdnsshared.cpp
)

set(qjdns_PUBLIC_HEADERS
    "${JDNS_INCLUDEDIR}/qjdns.h"
    "${JDNS_INCLUDEDIR}/qjdnsshared.h"
)

set(qjdns_HEADERS
    qjdns_sock.h
)

add_library(qjdns ${qjdns_SRCS} ${qjdns_MOC_SRCS} ${qjdns_MOC_HDRS} ${qjdns_PUBLIC_HEADERS})
if(Qt5Core_FOUND)
  target_link_libraries(qjdns ${Qt5Core_LIBRARIES} ${Qt5Network_LIBRARIES})
else(Qt5Core_FOUND)
  target_link_libraries(qjdns ${QT_LIBRARIES})
endif(Qt5Core_FOUND)
target_link_libraries(qjdns jdns)

if(NOT android)
  set_target_properties(qjdns PROPERTIES
                        VERSION ${JDNS_LIB_MAJOR_VERSION}.${JDNS_LIB_MINOR_VERSION}.${JDNS_LIB_PATCH_VERSION}
                        SOVERSION ${JDNS_LIB_MAJOR_VERSION}
  )
endif()
set_target_properties(qjdns PROPERTIES
                      DEFINE_SYMBOL JDNS_MAKEDLL
                      PUBLIC_HEADER "${qjdns_PUBLIC_HEADERS}"
#                     FRAMEWORK ${OSX_FRAMEWORK}
)

install(TARGETS qjdns EXPORT qjdns-export
        LIBRARY DESTINATION ${LIB_INSTALL_DIR}
        RUNTIME DESTINATION ${BIN_INSTALL_DIR}
        ARCHIVE DESTINATION ${LIB_INSTALL_DIR}
#       FRAMEWORK DESTINATION ${FRAMEWORK_INSTALL_DIR}
        PUBLIC_HEADER DESTINATION "${INCLUDE_INSTALL_DIR}/jdns"
)

if(MSVC)
  get_target_property(LOCATION qjdns LOCATION_DEBUG)
  string(REGEX REPLACE "\\.[^.]*$" ".pdb" LOCATION "${LOCATION}")
  install(FILES ${LOCATION} DESTINATION ${CMAKE_INSTALL_PREFIX}/bin CONFIGURATIONS Debug)

  get_target_property(LOCATION qjdns LOCATION_RELWITHDEBINFO)
  string(REGEX REPLACE "\\.[^.]*$" ".pdb" LOCATION "${LOCATION}")
  install(FILES ${LOCATION} DESTINATION ${CMAKE_INSTALL_PREFIX}/bin CONFIGURATIONS RelWithDebInfo)
endif(MSVC)
