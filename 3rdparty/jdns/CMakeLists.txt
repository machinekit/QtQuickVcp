project(jdns)

if(NOT APPLE)
  cmake_minimum_required(VERSION 2.8.11)
else()
  cmake_minimum_required(VERSION 3.0)
endif()

cmake_policy(SET CMP0003 NEW)

# Do not link against qtmain on Windows
cmake_policy(SET CMP0020 OLD)

# On Windows debug library should have 'd' postfix.
if(WIN32)
  set(CMAKE_DEBUG_POSTFIX "d")
elseif(APPLE)
 set(CMAKE_DEBUG_POSTFIX "_debug")
endif(WIN32)

# jdns can be used as bundled in this case need to explicity disable automoc
# to avoid duplicates of mocked files
set(CMAKE_AUTOMOC OFF)


# OPTION(OSX_FRAMEWORK "Build a Mac OS X Framework")
# SET(FRAMEWORK_INSTALL_DIR "${CMAKE_INSTALL_PREFIX}/Library/Frameworks"
#     CACHE PATH "Where to place jdns.framework if OSX_FRAMEWORK is selected")

option(BUILD_SHARED_LIBS "Build shared library" ON)
option(BUILD_QJDNS "Buid JDNS Qt-wrapper" ON)
option(BUILD_JDNS_TOOL "Build jdns test tool" ON)

# jdns tool requires qjdns
if(NOT BUILD_QJDNS)
  set(BUILD_JDNS_TOOL OFF)
endif(NOT BUILD_QJDNS)

if(NOT BUILD_SHARED_LIBS)
  add_definitions(-DJDNS_STATIC)
endif(NOT BUILD_SHARED_LIBS)

if(BUILD_QJDNS)
  option(QT4_BUILD "Force building with Qt4 even if Qt5 is found")
  # On Linux in most cases (or always) can be cointstalled both Qt4 and Qt5.
  # Need to provide two versions of qjdns are Qt4 and Qt5-based.
  if(${CMAKE_SYSTEM_NAME} STREQUAL "Linux")
    option(MULTI_QT "Consider multiple Qt version on the system (add suffixes)" ON)
  else()
    option(MULTI_QT "Consider multiple Qt version on the system (add suffixes)" OFF)
  endif()

  if(NOT QT4_BUILD)
    find_package(Qt5Core QUIET)
    find_package(Qt5Network QUIET)
  endif(NOT QT4_BUILD)

  if(Qt5Core_FOUND)
    message("Qt5 found")

    include_directories(${Qt5Core_INCLUDE_DIRS})
    include_directories(${Qt5Network_INCLUDE_DIRS})
    add_definitions(${Qt5Core_DEFINITIONS})
    add_definitions(${Qt5Network_DEFINITIONS})

    # Tell CMake to run moc when necessary:
    set(CMAKE_AUTOMOC ON)
    # As moc files are generated in the binary dir, tell CMake
    # to always look for includes there:
    set(CMAKE_INCLUDE_CURRENT_DIR ON)
    set(QJDns_QT_PC_VERSION "Qt5Core Qt5Network")
    set(QT_MAJ "-qt5")
  else(Qt5Core_FOUND)
    message("Qt5 not found, searching for Qt4")
    # Find Qt4
    find_package(Qt4 REQUIRED QtCore QtNetwork)

    # Include the cmake file needed to use qt4
    include(${QT_USE_FILE})
    set(QJDns_QT_PC_VERSION "QtCore QtNetwork")
    set(QT_MAJ "-qt4")
  endif(Qt5Core_FOUND)

  if(NOT WIN32)
    set(QT_DONT_USE_QTGUI TRUE)
  endif(NOT WIN32)

  if(NOT MULTI_QT)
    set(QT_MAJ "")
  endif(NOT MULTI_QT)
endif(BUILD_QJDNS)


# put the include dirs which are in the source or build tree
# before all other include dirs, so the headers in the sources
# are prefered over the already installed ones
set(CMAKE_INCLUDE_DIRECTORIES_PROJECT_BEFORE ON)

set(JDNS_INCLUDEDIR "${CMAKE_CURRENT_SOURCE_DIR}/include/jdns" )


#add extra search paths for libraries and includes
set(LIB_SUFFIX "" CACHE STRING "Define suffix of directory name (32/64)" )
set(BIN_INSTALL_DIR "${CMAKE_INSTALL_PREFIX}/bin" CACHE STRING "Directory where binary will install")
set(LIB_INSTALL_DIR "${CMAKE_INSTALL_PREFIX}/lib${LIB_SUFFIX}" CACHE STRING "Directory where library will install")
set(INCLUDE_INSTALL_DIR "${CMAKE_INSTALL_PREFIX}/include" CACHE PATH "The directory the headers are installed in")

set(LIB_INSTALL_DIR_FULL "${LIB_INSTALL_DIR}")
set(INCLUDE_INSTALL_DIR_FULL "${INCLUDE_INSTALL_DIR}")

# Normalize paths for comparsion
get_filename_component(CMAKE_INSTALL_PREFIX ${CMAKE_INSTALL_PREFIX} ABSOLUTE)
get_filename_component(BIN_INSTALL_DIR ${BIN_INSTALL_DIR} ABSOLUTE)
get_filename_component(LIB_INSTALL_DIR ${LIB_INSTALL_DIR} ABSOLUTE)
get_filename_component(INCLUDE_INSTALL_DIR ${INCLUDE_INSTALL_DIR} ABSOLUTE)

if(WIN32)
  # if all paths are subdirs of CMAKE_INSTALL_PREFIX it is possible to use relative paths
  set(USE_RELATIVE_PATHS ON)
  foreach(PATH ${BIN_INSTALL_DIR} ${LIB_INSTALL_DIR} ${INCLUDE_INSTALL_DIR})
    string(FIND "${PATH}/" "${CMAKE_INSTALL_PREFIX}/" POS)
    if(NOT "${POS}" STREQUAL "0")
      set(USE_RELATIVE_PATHS OFF)
    endif()
  endforeach()
else()
  # On Mac OS X relative paths lead to relative library id.
  # Relative id can't be used to start application linked against such library.
  set(USE_RELATIVE_PATHS OFF)
endif()

if(USE_RELATIVE_PATHS)
  file(RELATIVE_PATH BIN_INSTALL_DIR ${CMAKE_INSTALL_PREFIX} ${BIN_INSTALL_DIR})
  file(RELATIVE_PATH LIB_INSTALL_DIR ${CMAKE_INSTALL_PREFIX} ${LIB_INSTALL_DIR})
  file(RELATIVE_PATH INCLUDE_INSTALL_DIR ${CMAKE_INSTALL_PREFIX} ${INCLUDE_INSTALL_DIR})
endif()

if(NOT MSVC)
  set(JDNS_CONFIG_INSTALL_DIR "${LIB_INSTALL_DIR}/cmake/jdns")
  set(QJDNS_CONFIG_INSTALL_DIR "${LIB_INSTALL_DIR}/cmake/qjdns${QT_MAJ}")
  set(QJDNS_COMMON_CONFIG_INSTALL_DIR "${LIB_INSTALL_DIR}/cmake/qjdns")
else(NOT MSVC)
  set(JDNS_CONFIG_INSTALL_DIR "${CMAKE_INSTALL_PREFIX}/cmake/")
  set(QJDNS_CONFIG_INSTALL_DIR "${CMAKE_INSTALL_PREFIX}/cmake/")
  set(QJDNS_COMMON_CONFIG_INSTALL_DIR "${CMAKE_INSTALL_PREFIX}/cmake/")
endif(NOT MSVC)

set(JDNS_LIB_MAJOR_VERSION "2")
set(JDNS_LIB_MINOR_VERSION "0")
set(JDNS_LIB_PATCH_VERSION "4")

set(JDNS_LIB_VERSION_STRING "${JDNS_LIB_MAJOR_VERSION}.${JDNS_LIB_MINOR_VERSION}.${JDNS_LIB_PATCH_VERSION}")

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin" )
# Use the same path for shared and static library
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib" )
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib" )


# pkg-config
if(NOT WIN32)
  configure_file(${CMAKE_CURRENT_SOURCE_DIR}/jdns.pc.in
                 ${CMAKE_CURRENT_BINARY_DIR}/jdns.pc
                 @ONLY)
  install(FILES ${CMAKE_CURRENT_BINARY_DIR}/jdns.pc
          DESTINATION ${LIB_INSTALL_DIR}/pkgconfig)

  if(BUILD_QJDNS)
    configure_file(${CMAKE_CURRENT_SOURCE_DIR}/qjdns.pc.in
                   ${CMAKE_CURRENT_BINARY_DIR}/qjdns${QT_MAJ}.pc
                   @ONLY)
    install(FILES ${CMAKE_CURRENT_BINARY_DIR}/qjdns${QT_MAJ}.pc
            DESTINATION ${LIB_INSTALL_DIR}/pkgconfig)
  endif(BUILD_QJDNS)
endif(NOT WIN32)

include_directories("include/jdns/")

# Subdirs
add_subdirectory(src)

if(BUILD_JDNS_TOOL)
  add_subdirectory(tools/jdns)
endif(BUILD_JDNS_TOOL)

configure_file(
  "${CMAKE_CURRENT_SOURCE_DIR}/cmake_uninstall.cmake.in"
  "${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake"
  IMMEDIATE @ONLY)

# cmake-modules
include(CMakePackageConfigHelpers)
configure_package_config_file(
  JDnsConfig.cmake.in
  "${CMAKE_CURRENT_BINARY_DIR}/JDnsConfig.cmake"
  INSTALL_DESTINATION ${JDNS_CONFIG_INSTALL_DIR}
  PATH_VARS BIN_INSTALL_DIR
            LIB_INSTALL_DIR
            INCLUDE_INSTALL_DIR)

write_basic_config_version_file(JDnsConfigVersion.cmake VERSION ${JDNS_LIB_VERSION_STRING} COMPATIBILITY AnyNewerVersion)

install(FILES ${CMAKE_CURRENT_BINARY_DIR}/JDnsConfig.cmake
              ${CMAKE_CURRENT_BINARY_DIR}/JDnsConfigVersion.cmake
        DESTINATION ${JDNS_CONFIG_INSTALL_DIR})

install(EXPORT jdns-export DESTINATION ${JDNS_CONFIG_INSTALL_DIR} FILE JDnsTargets.cmake)
if(BUILD_QJDNS)
  install(EXPORT qjdns-export DESTINATION ${QJDNS_CONFIG_INSTALL_DIR} FILE QJDns${QT_MAJ}Targets.cmake)

  # cmake-modules
  configure_package_config_file(
    QJDns-qtConfig.cmake.in
    "${CMAKE_CURRENT_BINARY_DIR}/QJDns${QT_MAJ}Config.cmake"
    INSTALL_DESTINATION ${QJDNS_CONFIG_INSTALL_DIR})

  write_basic_config_version_file(QJDns${QT_MAJ}ConfigVersion.cmake VERSION ${JDNS_LIB_VERSION_STRING} COMPATIBILITY AnyNewerVersion)

  install(FILES ${CMAKE_CURRENT_BINARY_DIR}/QJDns${QT_MAJ}Config.cmake
                ${CMAKE_CURRENT_BINARY_DIR}/QJDns${QT_MAJ}ConfigVersion.cmake
          DESTINATION ${QJDNS_CONFIG_INSTALL_DIR})

  # Install QJDns config module which will automagically choose correct building of QJDns
  if(MULTI_QT)
    configure_file(${CMAKE_CURRENT_SOURCE_DIR}/QJDnsConfig.cmake.in
                   ${CMAKE_CURRENT_BINARY_DIR}/QJDnsConfig.cmake
                   COPYONLY)

    write_basic_config_version_file(QJDnsConfigVersion.cmake VERSION ${JDNS_LIB_VERSION_STRING} COMPATIBILITY AnyNewerVersion)
    install(FILES ${CMAKE_CURRENT_BINARY_DIR}/QJDnsConfig.cmake
                  ${CMAKE_CURRENT_BINARY_DIR}/QJDnsConfigVersion.cmake
            DESTINATION ${QJDNS_COMMON_CONFIG_INSTALL_DIR})
  endif()

endif(BUILD_QJDNS)

add_custom_target(uninstall
  "${CMAKE_COMMAND}" -P "${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake")
