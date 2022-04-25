# - Find LibS3
#
# To provide the module with a hint about where to find your LibS3
# installation, you can set the environment variable LibS3_ROOT. The
# Find module will then look in this path when searching for LibS3 paths
# and libraries.
#
# Find the LibS3 includes and libraries
#
#  LibS3_INCLUDE_DIR - where to find mdslib.h, etc
#  LibS3_LIBRARIES   - Link these libraries when using LibS3
#  LibS3_FOUND       - True if LibS3 found
#
# Normal usage would be:
#  find_package (LibS3 REQUIRED)
#  target_link_libraries (uses_LibS3 ${LibS3_LIBRARIES})

if( LibS3_INCLUDE_DIR AND LibS3_LIBRARIES )
  # Already in cache, be silent
  set( LibS3_FIND_QUIETLY TRUE )
endif( LibS3_INCLUDE_DIR AND LibS3_LIBRARIES )

find_path( LibS3_INCLUDE_DIR LibS3.h
  HINTS ${LibS3_ROOT}
  ENV LibS3_ROOT
  PATHS
    /usr/local
    /opt/local
    /sw
    /usr/lib/sfw
  PATH_SUFFIXES include
)

find_library( LibS3_LIBRARIES NAMES s3
  HINTS ${LibS3_ROOT}
  ENV LibS3_ROOT
  PATHS
    /opt/local
    /sw
    /usr
    /usr/local
  PATH_SUFFIXES lib lib64
)

include( FindPackageHandleStandardArgs )
find_package_handle_standard_args( LibS3 DEFAULT_MSG LibS3_LIBRARIES LibS3_INCLUDE_DIR )

mark_as_advanced( LibS3_LIBRARIES LibS3_INCLUDE_DIR )
