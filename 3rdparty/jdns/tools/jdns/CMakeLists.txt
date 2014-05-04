set(jdns_tool_MOC_HDRS
    main.h
)

if(NOT Qt5Core_FOUND)
  qt4_wrap_cpp(jdns_tool_MOC_SRCS ${jdns_tool_MOC_HDRS})
endif()

set(jdns_tool_SRCS
    main.cpp
)

add_executable(jdns-tool ${jdns_tool_SRCS} ${jdns_tool_MOC_SRCS} ${jdns_tool_MOC_SRCS})

target_link_libraries(jdns-tool jdns qjdns)

set_target_properties(jdns-tool PROPERTIES
                      OUTPUT_NAME jdns
)

install(TARGETS jdns-tool
        LIBRARY DESTINATION ${LIB_INSTALL_DIR}
        RUNTIME DESTINATION ${BIN_INSTALL_DIR}
        ARCHIVE DESTINATION ${LIB_INSTALL_DIR}
#       FRAMEWORK DESTINATION ${FRAMEWORK_INSTALL_DIR}
)

if(MSVC)
  get_target_property(LOCATION jdns-tool LOCATION_DEBUG)
  string(REGEX REPLACE "\\.[^.]*$" ".pdb" LOCATION "${LOCATION}")
  install(FILES ${LOCATION} DESTINATION ${BIN_INSTALL_DIR} CONFIGURATIONS Debug)

  get_target_property(LOCATION jdns-tool LOCATION_RELWITHDEBINFO)
  string(REGEX REPLACE "\\.[^.]*$" ".pdb" LOCATION "${LOCATION}")
  install(FILES ${LOCATION} DESTINATION ${BIN_INSTALL_DIR} CONFIGURATIONS RelWithDebInfo)
endif(MSVC)
