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

if(QT_MAJ)
  set_target_properties(qjdns PROPERTIES OUTPUT_NAME "qjdns${QT_MAJ}")
endif()

if(Qt5Core_FOUND)
  target_link_libraries(qjdns ${Qt5Core_LIBRARIES} ${Qt5Network_LIBRARIES})
else(Qt5Core_FOUND)
  target_link_libraries(qjdns ${QT_LIBRARIES})
endif(Qt5Core_FOUND)
target_link_libraries(qjdns jdns)

if(NOT ANDROID)
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

set(INCLUDES_DESTINATION "")
if("${CMAKE_VERSION}" VERSION_LESS "2.8.12")
  if(USE_RELATIVE_PATHS)
    target_include_directories(qjdns INTERFACE "${CMAKE_INSTALL_PREFIX}/${INCLUDE_INSTALL_DIR}/jdns")
  else()
    target_include_directories(qjdns INTERFACE "${INCLUDE_INSTALL_DIR}/jdns")
  endif()
else()
  set(INCLUDES_DESTINATION INCLUDES DESTINATION "${INCLUDE_INSTALL_DIR}/jdns")
endif()

install(TARGETS qjdns EXPORT qjdns-export
        LIBRARY DESTINATION ${LIB_INSTALL_DIR}
        RUNTIME DESTINATION ${BIN_INSTALL_DIR}
        ARCHIVE DESTINATION ${LIB_INSTALL_DIR}
#       FRAMEWORK DESTINATION ${FRAMEWORK_INSTALL_DIR}
        PUBLIC_HEADER DESTINATION "${INCLUDE_INSTALL_DIR}/jdns" ${INCLUDES_DESTINATION}
)

if(MSVC)
  get_target_property(LOCATION qjdns LOCATION_DEBUG)
  string(REGEX REPLACE "\\.[^.]*$" ".pdb" LOCATION "${LOCATION}")
  install(FILES ${LOCATION} DESTINATION ${CMAKE_INSTALL_PREFIX}/bin CONFIGURATIONS Debug)

  get_target_property(LOCATION qjdns LOCATION_RELWITHDEBINFO)
  string(REGEX REPLACE "\\.[^.]*$" ".pdb" LOCATION "${LOCATION}")
  install(FILES ${LOCATION} DESTINATION ${CMAKE_INSTALL_PREFIX}/bin CONFIGURATIONS RelWithDebInfo)
endif(MSVC)
