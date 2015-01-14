##########################################################################
# CMakeLists.txt
##########################################################################

cmake_minimum_required(VERSION 2.8.11)

project(minibee)

#set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake/modules ${CMAKE_MODULE_PATH})
include(FindPkgConfig)

set(VERSION_MAJOR "0")
set(VERSION_MINOR "1")
set(VERSION_PATCH "0")
set(VERSION "${VERSION_MAJOR}.${VERSION_MINOR}.${VERSION_PATCH}")

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/Modules/")

# liblo
pkg_check_modules(liblo REQUIRED liblo)

# libxbee
find_package(LibXBee)

# Set up compiler flags

#set the default path for built executables to the "bin" directory
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)
#set the default path for built libraries to the "lib" directory
set(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/lib)




##########################################################################
# Dependencies
##########################################################################

# set(Boost_USE_MULTITHREADED ON)
# find_package( Boost 1.50.0 COMPONENTS thread system filesystem program_options )
# test_exec_monitor

# if (Boost_FOUND)
#   link_directories(${Boost_LIBRARY_DIRS})
#   include_directories(${Boost_INCLUDE_DIRS})
# endif()


include_directories(
  ${CMAKE_BINARY_DIR}
  ${liblo_INCLUDE_DIRS}
#   ${Boost_INCLUDE_DIRS}
  headers/
)

link_directories(
  ${liblo_LIBRARY_DIRS}
  ${libxbee_LIBRARY_DIRS}
#   ${Boost_LIBRARY_DIRS}
  ${CMAKE_BINARY_DIR}
#   lib/
)

add_definitions("-std=c++11")

##########################################################################
# 
##########################################################################

# set(xbee_osc_lib_SRCS
# #   src/lo_extensions.cpp
#   sources/BoostSerialPort.cpp
# )
# 
# add_library( xbee_osc_lib SHARED ${xbee_osc_lib_SRCS} )
# # add_library( xbee_osc_lib ${xbee_osc_lib_SRCS} )
# 
# # target_link_libraries(xbee-osc-lib ${liblo_LIBRARIES} ${Boost_LIBRARY_DIRS} )
# target_link_libraries(xbee_osc_lib LINK_PUBLIC ${Boost_THREAD_LIBRARY} ${Boost_SYSTEM_LIBRARY} ${Boost_FILESYSTEM_LIBRARY} )
# # target_include_libraries(xbee_osc_lib PUBLIC ${Boost_THREAD_LIBRARY} ${Boost_SYSTEM_LIBRARY} ${Boost_FILESYSTEM_LIBRARY} )
# 
# install(TARGETS xbee_osc_lib DESTINATION lib)

# set( xbee-osc_main_SRCS
#   sources/xbee-osc_main.cpp
# )
# 
# add_executable( xbee-osc ${xbee-osc_main_SRCS} )
# 
# target_link_libraries(xbee-osc xbee-osc)
# 
# install(TARGETS xbee-osc DESTINATION bin)

add_subdirectory (sources)


##########################################################################
# Tests
##########################################################################

#enable_testing()

# set(example_serial_port_SRCS
#   examples/test_serial_port.cpp
# )
# 
# add_executable( test_serial_port ${example_serial_port_SRCS} )
# 
# target_link_libraries(test_serial_port xbee_osc_lib)
# 
# install(TARGETS test_serial_port DESTINATION bin)

# add_subdirectory (examples)


##########################################################################
# Uninstall
##########################################################################

#CONFIGURE_FILE(
#  "${CMAKE_CURRENT_SOURCE_DIR}/cmake/cmake_uninstall.cmake.in"
#  "${CMAKE_CURRENT_BINARY_DIR}/cmake/cmake_uninstall.cmake"
#  IMMEDIATE @ONLY)

#ADD_CUSTOM_TARGET(uninstall
#  "${CMAKE_COMMAND}" -P "${CMAKE_CURRENT_BINARY_DIR}/cmake/cmake_uninstall.cmake")

##########################################################################
# Packaging
##########################################################################

# if(DEFINED PACKAGE)
# 
#   set(CPACK_PACKAGE_NAME "videorecosc")
#   set(CPACK_PACKAGE_VENDOR "Marije Baalman <sensestage@nescivi.nl")
#   set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "videorec/playosc - osc controllable video recorder and player")
#   set(CPACK_PACKAGE_DESCRIPTION "videorec/playosc - osc controllable video recorder and player")
#   set(CPACK_PACKAGE_VERSION_MAJOR "${VERSION_MAJOR}")
#   set(CPACK_PACKAGE_VERSION_MINOR "${VERSION_MINOR}")
#   set(CPACK_PACKAGE_VERSION_PATCH "${VERSION_PATCH}")
#   set(CPACK_PACKAGE_INSTALL_DIRECTORY "videoosc-${VERSION}")
#   set(CPACK_PACKAGE_DESCRIPTION_FILE "${CMAKE_SOURCE_DIR}/README")
#   set(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_SOURCE_DIR}/COPYING")
#   set(CPACK_STRIP_FILES TRUE)
#   set(CPACK_TOPLEVEL_TAG "package")
#   set(CPACK_PACKAGE_FILE_NAME "videoosc-${VERSION}")
# 
#   if(PACKAGE STREQUAL "RPM")
#     set(CPACK_GENERATOR "RPM")
#     if(NOT DEFINED CPACK_RPM_PACKAGE_RELEASE)
#       set(CPACK_RPM_PACKAGE_RELEASE "1")
#     endif(NOT DEFINED CPACK_RPM_PACKAGE_RELEASE)
#     set(CPACK_RPM_PACKAGE_LICENSE "GPLv3+")
#     set(CPACK_RPM_PACKAGE_REQUIRES "opencv, liblo")
#     set(CPACK_PACKAGE_FILE_NAME "videoosc-${VERSION}-${CPACK_RPM_PACKAGE_RELEASE}")
#   endif(PACKAGE STREQUAL "RPM")
# 
#   if(PACKAGE STREQUAL "DEB")
#     set(CPACK_GENERATOR "DEB")
#     set(CPACK_DEBIAN_PACKAGE_MAINTAINER "Marije Baalman <sensestage@nescivi.nl>")
#     set(CPACK_DEBIAN_PACKAGE_DEPENDS "libhighgui2.1, libcvaux2.1, libcv2.1, liblo7")
#     set(CPACK_DEBIAN_PACKAGE_SECTION "video")
#     set(CPACK_DEBIAN_PACKAGE_VERSION "${VERSION}")
#   endif(PACKAGE STREQUAL "DEB")
# 
#   set(CPACK_SOURCE_GENERATOR "TGZ")
#   set(CPACK_SOURCE_PACKAGE_FILE_NAME "videoosc-${VERSION}" CACHE INTERNAL "tarball basename")
#   set(CPACK_IGNORE_FILES "/\\\\.git/;CMakeLists.txt.user;/build/")
#   set(CPACK_SOURCE_IGNORE_FILES ${CPACK_IGNORE_FILES})
# 
#   include(CPack)
# 
# endif(DEFINED PACKAGE)

