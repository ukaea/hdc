cmake_minimum_required( VERSION 3.3 )

########################################################################################################################
# CMake configuration
cmake_policy( SET CMP0057 NEW )

project( hdc VERSION 0.18.2 )
enable_language( C Fortran )

set( hdc_SOURCE_DIR "${hdc_SOURCE_DIR}/src" )
set( CMAKE_Fortran_MODULE_DIRECTORY "mod" )

# Extra cmake modules to find libraries
set( CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_CURRENT_LIST_DIR}/cmake )
include( FileCopyTargets )
##include( CHDC )

# Set the possible values of build type for cmake-gui
set_property( CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS "DEBUG" "RELEASE" )
# Set a default build type if none was specified
if( NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES )
  message( STATUS "Setting build type to 'RELEASE' as none was specified." )
  set( CMAKE_BUILD_TYPE RELEASE CACHE STRING "Choose the type of build." FORCE )
endif()

########################################################################################################################
# Compiler configuration

# Set minimal compiler requirements
if( CMAKE_COMPILER_IS_GNUCC AND CMAKE_C_COMPILER_VERSION VERSION_LESS 4.9 )
  message(
    FATAL_ERROR

    "
    HDC requires at least gcc-4.9. Please, obtain higher version of gcc toolset and run me again after exporting path to a newer compiler.

    For example:

    export FC=/usr/bin/gfortran-4.9
    export CC=/usr/bin/gcc-4.9
    export CXX=/usr/bin/g++-4.9
    " )
endif()

if( CMAKE_Fortran_COMPILER_ID STREQUAL "Intel" )
  # -diag-enable=remark
  set( CMAKE_Fortran_FLAGS_RELEASE "${CMAKE_Fortran_FLAGS_RELEASE} -O2 -free -diag-enable=warn -fPIC" )
  set( CMAKE_Fortran_FLAGS_DEBUG "${CMAKE_Fortran_FLAGS_DEBUG} -O0 -free -diag-enable=warn -fPIC" )
elseif( CMAKE_Fortran_COMPILER_ID STREQUAL "PGI" )
  set( CMAKE_Fortran_FLAGS_RELEASE "${CMAKE_Fortran_FLAGS_RELEASE} -O2 -Mfree -fPIC" )
  set( CMAKE_Fortran_FLAGS_DEBUG "${CMAKE_Fortran_FLAGS_DEBUG} -O0 -Mfree -fPIC" )
elseif( CMAKE_Fortran_COMPILER_ID STREQUAL "GNU" )
  set( CMAKE_Fortran_FLAGS_RELEASE "${CMAKE_Fortran_FLAGS_RELEASE} -O2 -ffree-line-length-none -pedantic -fbacktrace -Wall -Wextra -std=f2008 -fPIC" )
  set( CMAKE_Fortran_FLAGS_DEBUG "${CMAKE_Fortran_FLAGS_DEBUG} -O0 -ffree-line-length-none -pedantic -fbacktrace -Wall -Wextra -std=f2008 -fPIC" )
else()
  message( "Warning: Unsupported Fortran compiler: ${CMAKE_Fortran_COMPILER_ID}" )
  set( CMAKE_Fortran_FLAGS_RELEASE "${CMAKE_Fortran_FLAGS_RELEASE} -O2" )
  set( CMAKE_Fortran_FLAGS_DEBUG "${CMAKE_Fortran_FLAGS_DEBUG} -O0" )
endif()

########################################################################################################################
# RPATH options
# https://cmake.org/Wiki/CMake_RPATH_handling#Always_full_RPATH

# use, i.e. don't skip the full RPATH for the build tree
set( CMAKE_SKIP_BUILD_RPATH FALSE )

# when building, don't use the install RPATH already
# (but later on when installing)
set( CMAKE_BUILD_WITH_INSTALL_RPATH FALSE )

set( CMAKE_INSTALL_RPATH "${CMAKE_INSTALL_PREFIX}/lib" )

# add the automatically determined parts of the RPATH
# which point to directories outside the build tree to the install RPATH
set( CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE )


# the RPATH to be used when installing, but only if it's not a system directory
list( FIND CMAKE_PLATFORM_IMPLICIT_LINK_DIRECTORIES "${CMAKE_INSTALL_PREFIX}/lib" isSystemDir )
if( "${isSystemDir}" STREQUAL "-1" )
  set( CMAKE_INSTALL_RPATH "${CMAKE_INSTALL_PREFIX}/lib:${CMAKE_INSTALL_PREFIX}/lib/hdc" )
endif( "${isSystemDir}" STREQUAL "-1" )

########################################################################################################################
# Build options

option( BUILD_EXAMPLES "Build and install examples" ON )
option( ENABLE_COVERAGE "Enable coverage testing (using gcovr)" ON )
option( DEBUG "Turns some debug output on or off." OFF )
option( GOPTION "adds -g to compiler flags" ON )

if( GOPTION )
  set(CMAKE_Fortran_FLAGS "${CMAKE_Fortran_FLAGS} -g")
endif() # GOPTION

if( DEBUG )
    add_definitions( -DDEBUG )
endif() # DEBUG

if( CMAKE_COMPILER_IS_GNUCXX AND CMAKE_BUILD_TYPE MATCHES "DEBUG" AND ENABLE_COVERAGE )
  include( CodeCoverage )
  append_coverage_compiler_flags()
endif()

message( "Binary dir is: ${hdc_BINARY_DIR}" )
message( "Source dir is: ${hdc_SOURCE_DIR}" )

########################################################################################################################
# Source directories

add_subdirectory( src )
add_subdirectory( pkgconfig )

if( BUILD_EXAMPLES )
  add_subdirectory( examples )
endif()

if( BUILD_BENCHMARKS )
  add_subdirectory( benchmark )
endif()
