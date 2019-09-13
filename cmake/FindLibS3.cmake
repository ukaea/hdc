# - Find LibS3
#
# To provide the module with a hint about where to find your LibS3
# installation, you can set the environment variable LIBS3_ROOT. The
# Find module will then look in this path when searching for LibS3 paths
# and libraries.
#
# Find the LibS3 includes and libraries
#
#  LIBS3_INCLUDE_DIR - where to find mdslib.h, etc
#  LIBS3_LIBRARIES   - Link these libraries when using LibS3
#  LIBS3_FOUND       - True if LibS3 found
#
# Normal usage would be:
#  find_package (LibS3 REQUIRED)
#  target_link_libraries (uses_LibS3 ${LIBS3_LIBRARIES})

if( LIBS3_INCLUDE_DIR AND LIBS3_LIBRARIES )
  # Already in cache, be silent
  set( LIBS3_FIND_QUIETLY TRUE )
endif( LIBS3_INCLUDE_DIR AND LIBS3_LIBRARIES )

find_path( LIBS3_INCLUDE_DIR libs3.h
  HINTS ${LIBS3_ROOT}
  ENV LIBS3_ROOT
  PATHS
    /usr/local
    /opt/local
    /sw
    /usr/lib/sfw
  PATH_SUFFIXES include
)

find_library( LIBS3_LIBRARIES NAMES s3
  HINTS ${LIBS3_ROOT}
  ENV LIBS3_ROOT
  PATHS
    /opt/local
    /sw
    /usr
    /usr/local
  PATH_SUFFIXES lib lib64
)

include( FindPackageHandleStandardArgs )
find_package_handle_standard_args( LIBS3 DEFAULT_MSG LIBS3_LIBRARIES LIBS3_INCLUDE_DIR )

mark_as_advanced( LIBS3_LIBRARIES LIBS3_INCLUDE_DIR )
