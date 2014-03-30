project(qjdns)

# Force cmake 2.8.8 in order to have a decent support of Qt5
cmake_minimum_required(VERSION 2.8.8)
cmake_policy(SET CMP0003 NEW)

# Do not link against qtmain on Windows
cmake_policy(SET CMP0020 OLD)

set(CMAKE_INSTALL_NAME_DIR ${LIB_INSTALL_DIR})

# On Windows debug library should have 'd' postfix.
if(WIN32)
  set(CMAKE_DEBUG_POSTFIX "d")
endif(WIN32)

# OPTION(OSX_FRAMEWORK "Build a Mac OS X Framework")
# SET(FRAMEWORK_INSTALL_DIR "${CMAKE_INSTALL_PREFIX}/Library/Frameworks"
#     CACHE PATH "Where to place qjdns.framework if OSX_FRAMEWORK is selected")

# Don't use absolute path in qjdns-targets-*.cmake
# (This will have no effect with CMake < 2.8)
set(QT_USE_IMPORTED_TARGETS TRUE)

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
else(Qt5Core_FOUND)
  message("Qt5 not found, searching for Qt4")
  # Find Qt4
  find_package(Qt4 REQUIRED QtCore QtNetwork)

  # Include the cmake file needed to use qt4
  include(${QT_USE_FILE})
endif(Qt5Core_FOUND)

if(NOT WIN32)
  set(QT_DONT_USE_QTGUI TRUE)
endif(NOT WIN32)


#add extra search paths for libraries and includes
set(LIB_SUFFIX "" CACHE STRING "Define suffix of directory name (32/64)" )
set(LIB_INSTALL_DIR "${CMAKE_INSTALL_PREFIX}/lib${LIB_SUFFIX}" CACHE STRING "Directory where lib will install")
set(INCLUDE_INSTALL_DIR "${CMAKE_INSTALL_PREFIX}/include" CACHE PATH "The directory the headers are installed in")
set(CMAKECONFIG_INSTALL_DIR "${LIB_INSTALL_DIR}/cmake/${CMAKE_PROJECT_NAME}" CACHE PATH "Directory where to install QJDNSConfig.cmake")

set(QJDNS_LIB_MAJOR_VERSION "1")
set(QJDNS_LIB_MINOR_VERSION "0")
set(QJDNS_LIB_PATCH_VERSION "0")

set(QJDNS_LIB_VERSION_STRING "${QJDNS_LIB_MAJOR_VERSION}.${QJDNS_LIB_MINOR_VERSION}.${QJDNS_LIB_PATCH_VERSION}")

set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib" )

# pkg-config
if(NOT WIN32)
  configure_file(${CMAKE_CURRENT_SOURCE_DIR}/qjdns.pc.in
                 ${CMAKE_CURRENT_BINARY_DIR}/qjdns.pc
                 @ONLY)
  install(FILES ${CMAKE_CURRENT_BINARY_DIR}/qjdns.pc
          DESTINATION ${LIB_INSTALL_DIR}/pkgconfig)
endif(NOT WIN32)

# Subdirs
add_subdirectory(src)

configure_file(
  "${CMAKE_CURRENT_SOURCE_DIR}/cmake_uninstall.cmake.in"
  "${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake"
  IMMEDIATE @ONLY)

install(EXPORT qjdns-export DESTINATION ${CMAKECONFIG_INSTALL_DIR} FILE QJDNSTargets.cmake)

# figure out the relative path from the installed Config.cmake file to the install prefix (which may be at
# runtime different from the chosen CMAKE_INSTALL_PREFIX if under Windows the package was installed anywhere)
# This relative path will be configured into the QJDNSConfig.cmake
file(RELATIVE_PATH relInstallDir ${CMAKE_INSTALL_PREFIX}/${CMAKECONFIG_INSTALL_DIR} ${CMAKE_INSTALL_PREFIX} )

# cmake-modules
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/QJDNSConfig.cmake.in
               ${CMAKE_CURRENT_BINARY_DIR}/QJDNSConfig.cmake
               @ONLY)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/QJDNSConfigVersion.cmake.in
               ${CMAKE_CURRENT_BINARY_DIR}/QJDNSConfigVersion.cmake
               @ONLY)
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/QJDNSConfig.cmake
              ${CMAKE_CURRENT_BINARY_DIR}/QJDNSConfigVersion.cmake
        DESTINATION "${CMAKECONFIG_INSTALL_DIR}")

add_custom_target(uninstall
  "${CMAKE_COMMAND}" -P "${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake")
