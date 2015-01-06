# - Try to find LibXBee
# Once done this will define
#  LIBXBEE_FOUND - System has LibXBee
#  LIBXBEE_INCLUDE_DIRS - The LibXBee include directories
#  LIBXBEE_LIBRARIES - The libraries needed to use LibXBee
#  LIBXBEE_DEFINITIONS - Compiler switches required for using LibXBee
#  LIBXBEEP_INCLUDE_DIRS - The LibXBee include directories for xpeep
#  LIBXBEEP_LIBRARIES - The libraries needed to use LibXBee for xbeep

# find_package(PkgConfig)
# pkg_check_modules(PC_LIBXML QUIET libxml-2.0)
# set(LIBXML2_DEFINITIONS ${PC_LIBXML_CFLAGS_OTHER})

find_path(LIBXBEE_INCLUDE_DIR xbee.h )

find_library(LIBXBEE_LIBRARY NAMES libxbee xbee )

find_path(LIBXBEEP_INCLUDE_DIR xbeep.h )

find_library(LIBXBEEP_LIBRARY NAMES libxbeep xbeep )

set(LIBXBEE_LIBRARIES ${LIBXBEE_LIBRARY} )
set(LIBXBEE_INCLUDE_DIRS ${LIBXBEE_INCLUDE_DIR} )
set(LIBXBEEP_LIBRARIES ${LIBXBEEP_LIBRARY} )
set(LIBXBEEP_INCLUDE_DIRS ${LIBXBEEP_INCLUDE_DIR} )

# include(FindPackageHandleStandardArgs)
# handle the QUIETLY and REQUIRED arguments and set LIBXML2_FOUND to TRUE
# if all listed variables are TRUE
# find_package_handle_standard_args(LibXml2  DEFAULT_MSG
#                                   LIBXML2_LIBRARY LIBXML2_INCLUDE_DIR)

mark_as_advanced(LIBXBEE_INCLUDE_DIR LIBXBEE_LIBRARY )
mark_as_advanced(LIBXBEEP_INCLUDE_DIR LIBXBEEP_LIBRARY )