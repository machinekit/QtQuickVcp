project(jdns)

# Force cmake 2.8.8 in order to have a decent support of Qt5
cmake_minimum_required(VERSION 2.8.8)
cmake_policy(SET CMP0003 NEW)

# Do not link against qtmain on Windows
cmake_policy(SET CMP0020 OLD)

# On Windows debug library should have 'd' postfix.
if(WIN32)
  set(CMAKE_DEBUG_POSTFIX "d")
elseif(APPLE)
 set(CMAKE_DEBUG_POSTFIX "_debug")
endif(WIN32)

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
  else(Qt5Core_FOUND)
    message("Qt5 not found, searching for Qt4")
    # Find Qt4
    find_package(Qt4 REQUIRED QtCore QtNetwork)

    # Include the cmake file needed to use qt4
    include(${QT_USE_FILE})
    set(QJDns_QT_PC_VERSION "QtCore QtNetwork")
  endif(Qt5Core_FOUND)

  if(NOT WIN32)
    set(QT_DONT_USE_QTGUI TRUE)
  endif(NOT WIN32)
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

if(NOT MSVC)
  set(JDNS_CONFIG_INSTALL_DIR "${LIB_INSTALL_DIR}/cmake/jdns")
  set(QJDNS_CONFIG_INSTALL_DIR "${LIB_INSTALL_DIR}/cmake/qjdns")
else(NOT MSVC)
  set(JDNS_CONFIG_INSTALL_DIR "${CMAKE_INSTALL_PREFIX}/cmake/")
  set(QJDNS_CONFIG_INSTALL_DIR "${CMAKE_INSTALL_PREFIX}/cmake/")
endif(NOT MSVC)

if(APPLE)
   set(CMAKE_INSTALL_NAME_DIR ${LIB_INSTALL_DIR})
endif(APPLE)

set(JDNS_LIB_MAJOR_VERSION "2")
set(JDNS_LIB_MINOR_VERSION "0")
set(JDNS_LIB_PATCH_VERSION "0")

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

  configure_file(${CMAKE_CURRENT_SOURCE_DIR}/qjdns.pc.in
                 ${CMAKE_CURRENT_BINARY_DIR}/qjdns.pc
                 @ONLY)
  install(FILES ${CMAKE_CURRENT_BINARY_DIR}/qjdns.pc
          DESTINATION ${LIB_INSTALL_DIR}/pkgconfig)
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

# figure out the relative path from the installed Config.cmake file to the install prefix (which may be at
# runtime different from the chosen CMAKE_INSTALL_PREFIX if under Windows the package was installed anywhere)
# This relative path will be configured into the QJDNSConfig.cmake
file(RELATIVE_PATH relInstallDir ${JDNS_CONFIG_INSTALL_DIR} ${CMAKE_INSTALL_PREFIX})
file(RELATIVE_PATH JDNS_INCLUDEDIR_REL ${CMAKE_INSTALL_PREFIX} "${INCLUDE_INSTALL_DIR}/jdns")
file(RELATIVE_PATH LIB_INSTALL_DIR_REL ${CMAKE_INSTALL_PREFIX} ${LIB_INSTALL_DIR})

# cmake-modules
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/JDnsConfig.cmake.in
               ${CMAKE_CURRENT_BINARY_DIR}/JDnsConfig.cmake
               @ONLY)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/JDnsConfigVersion.cmake.in
               ${CMAKE_CURRENT_BINARY_DIR}/JDnsConfigVersion.cmake
               @ONLY)
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/JDnsConfig.cmake
              ${CMAKE_CURRENT_BINARY_DIR}/JDnsConfigVersion.cmake
        DESTINATION ${JDNS_CONFIG_INSTALL_DIR})

install(EXPORT jdns-export DESTINATION ${JDNS_CONFIG_INSTALL_DIR} FILE JDnsTargets.cmake)
if(BUILD_QJDNS)
  install(EXPORT qjdns-export DESTINATION ${QJDNS_CONFIG_INSTALL_DIR} FILE QJDnsTargets.cmake)

  # cmake-modules
  configure_file(${CMAKE_CURRENT_SOURCE_DIR}/QJDnsConfig.cmake.in
                 ${CMAKE_CURRENT_BINARY_DIR}/QJDnsConfig.cmake
                 @ONLY)
               
  configure_file(${CMAKE_CURRENT_SOURCE_DIR}/QJDnsConfigVersion.cmake.in
                 ${CMAKE_CURRENT_BINARY_DIR}/QJDnsConfigVersion.cmake
                 @ONLY)

  install(FILES ${CMAKE_CURRENT_BINARY_DIR}/QJDnsConfig.cmake
                ${CMAKE_CURRENT_BINARY_DIR}/QJDnsConfigVersion.cmake
          DESTINATION ${QJDNS_CONFIG_INSTALL_DIR})
endif(BUILD_QJDNS)

add_custom_target(uninstall
  "${CMAKE_COMMAND}" -P "${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake")
