# - Find CHDC
#
# Once done this will define
#
#  CHDC_FOUND - System has CHDC
#  CHDC_INCLUDE_DIRS - The CHDC include directory
#  CHDC_LIBRARIES - The libraries needed to use CHDC
#  CHDC_DEFINITIONS - Compiler switches required for using CHDC
#=======================================================================================================================

# use pkg-config to get the directories and then use these values
# in the FIND_PATH() and FIND_LIBRARY() calls
find_package( PkgConfig )

pkg_check_modules( CHDC chdc )

set( CHDC_DEFINITIONS  ${CHDC_CFLAGS_OTHER} )

# handle the QUIETLY and REQUIRED arguments and set CHDC_FOUND to TRUE if
# all listed variables are TRUE
include( FindPackageHandleStandardArgs )
find_package_handle_standard_args( CHDC DEFAULT_MSG CHDC_LIBRARIES CHDC_INCLUDE_DIRS )
