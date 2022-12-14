cmake_minimum_required( VERSION 3.3 )

########################################################################################################################
# CMake configuration
cmake_policy( SET CMP0057 NEW )

project( hdc VERSION 0.21.0 )
enable_language( CXX C )

set( hdc_TEST_DIR "${hdc_SOURCE_DIR}/test" )
set( hdc_SOURCE_DIR "${hdc_SOURCE_DIR}/src" )
set( CMAKE_Fortran_MODULE_DIRECTORY "mod" )

# Extra cmake modules to find libraries
set( CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_CURRENT_LIST_DIR}/cmake )
include( CTest )
include( FileCopyTargets )

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

#set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")
#find_package(Sphinx REQUIRED)
if( CMAKE_C_COMPILER_ID STREQUAL "Intel" )
  set( CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -O3 -std=c99 -Wno-type-limits -Wall -Wextra -fPIC" )
  set( CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -O0 -std=c99 -Wno-type-limits -Wall -Wextra -fPIC" )
elseif( CMAKE_C_COMPILER_ID STREQUAL "PGI" )
  set( CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -O3 -std=c99 -fPIC" )
  set( CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -O0 -std=c99 -fPIC" )
elseif( CMAKE_C_COMPILER_ID STREQUAL "GNU" )
  set( CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -O2 -std=c99 -Wno-type-limits -Wall -Wextra -fPIC -pedantic" )
  set( CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -O0 -std=c99 -Wno-type-limits -Wall -Wextra -fPIC -pedantic" )
elseif( CMAKE_C_COMPILER_ID STREQUAL "AppleClang" )
  set( CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -O3 -std=c99 -Wno-type-limits -Wall -Wextra -fPIC -pedantic" )
  set( CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -O0 -std=c99 -Wno-type-limits -Wall -Wextra -fPIC -pedantic" )
elseif( CMAKE_C_COMPILER_ID STREQUAL "Clang" )
  set( CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -O3 -std=c99 -Wno-type-limits -Wall -Wextra -fPIC -pedantic" )
  set( CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -O0 -std=c99 -Wno-type-limits -Wall -Wextra -fPIC -pedantic" )
else()
  message( "Warning: Unsupported C compiler: ${CMAKE_C_COMPILER_ID}" )
  set( CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -O3 -std=c99" )
  set( CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -O0 -std=c99" )
endif()
if( CMAKE_CXX_COMPILER_ID STREQUAL "Intel" )
  set( CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O3 -std=c++14 -Wno-type-limits -Wall -Wextra -fPIC" )
  set( CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -O0 -std=c++14 -Wno-type-limits -Wall -Wextra -fPIC" )
elseif( CMAKE_CXX_COMPILER_ID STREQUAL "PGI" )
  set( CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O3 -std=c++14 -fPIC" )
  set( CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -O0 -std=c++14 -fPIC" )
elseif( CMAKE_CXX_COMPILER_ID STREQUAL "GNU" )
  set( CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O2 -std=c++14 -Wno-type-limits -Wall -Wextra -fPIC" )
  set( CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -O0 -std=c++14 -Wno-type-limits -Wall -Wextra -fPIC" )
elseif( CMAKE_CXX_COMPILER_ID STREQUAL "AppleClang" )
  set( CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O3 -std=c++14 -Wno-type-limits -Wall -Wextra -fPIC -Werror" )
  set( CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_RELEASE} -O0 -std=c++14 -Wno-type-limits -Wall -Wextra -fPIC" )
elseif( CMAKE_CXX_COMPILER_ID STREQUAL "Clang" )
  set( CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O3 -std=c++14 -Wno-type-limits -Wall -Wextra -fPIC -Werror" )
  set( CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -O0 -std=c++14 -Wno-type-limits -Wall -Wextra -fPIC" )
else()
  message( "Warning: Unsupported CXX compiler: ${CMAKE_CXX_COMPILER_ID}" )
  set( CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O3 -std=c++14 -pedantic" )
  set( CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -O0 -std=c++14 -pedantic" )
endif()

find_package( Boost 1.48.0 REQUIRED filesystem system regex )
include_directories( SYSTEM ${Boost_INCLUDE_DIRS} )
link_directories( ${Boost_LIBRARY_DIRS} )
message( "Boost_INCLUDE_DIRS: ${Boost_INCLUDE_DIRS}" )
message( "Boost_LIBRARY_DIRS: ${Boost_LIBRARY_DIRS}" )

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

option( BUILD_TESTS "Build and install tests" ON )
option( BUILD_EXAMPLES "Build and install examples" ON )
option( BUILD_BENCHMARKS "Whether to build benchmarks - uses google benchmarks." OFF )
option( ENABLE_HDF5 "Enable HDF5 support" ON )
option( ENABLE_UDA "Enable UDA support" OFF )
option( ENABLE_JAVA "Enable JAVA support" OFF )
option( ENABLE_LIBS3 "Enable libs3 serialization support" ON )
option( ENABLE_FLATBUFFERS "Enable flatbuffers serialization support" ON )
option( ENABLE_REDIS "Enable redis storage backend" OFF )
option( ENABLE_MATLAB "Enable MATLAB support" OFF )
option( ENABLE_FORTRAN "Enable FORTRAN support" ON )
option( ENABLE_COVERAGE "Enable coverage testing (using gcovr)" ON )
option( BUILD_OWN_HDF5 "Build own HDF5" OFF )
option( DEBUG "Turns some debug output on or off." OFF )
option( GOPTION "adds -g to compiler flags" ON )
option( BUILD_DOC "Create and install the HTML based API documentation (requires Doxygen)" OFF )
option( BUILD_MDBM_PLUGIN "Enables MDBM storage plugin." ON )
option( BUILD_OWN_MDBM "Build own MDBM" OFF )
option( INSTALL_JAVA_DEPENDENCIES "Install jHDC JAR dependencies" OFF )
option( JAR_WITH_DEPENDENCIES "Bundle all dependencies into a single jar - the result can be large (~80 MB)" OFF )
set( JAVACPP_PLATFORM "linux-x86_64" CACHE STRING "Strip unnecessary native libraries from bundled jar. Only relevant when JAR_WITH_DEPENDENCIES=ON" )
set( JAR_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}/share/java" CACHE STRING "JAR install prefix" )

if( GOPTION )
  set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -g")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g")
endif() # GOPTION

if( BUILD_BENCHMARKS )
  include( ExternalProject )
  externalproject_add(
    googlebenchmark
    GIT_REPOSITORY https://github.com/google/benchmark.git
    GIT_TAG v1.4.0
    TIMEOUT 10
    LOG_DOWNLOAD ON
    PREFIX "${hdc_BINARY_DIR}/gbench"
    BINARY_DIR "${hdc_BINARY_DIR}/gbench_build"
    SOURCE_DIR "${hdc_BINARY_DIR}/gbench_src"
    CMAKE_CACHE_ARGS
    -DBENCHMARK_BUILD_32_BITS:BOOL=OFF
    -DBENCHMARK_DOWNLOAD_DEPENDENCIES:BOOL=OFF
    -DBENCHMARK_ENABLE_ASSEMBLY_TESTS:BOOL=OFF
    -DBENCHMARK_ENABLE_EXCEPTIONS:BOOL=OFF
    -DBENCHMARK_ENABLE_GTEST_TESTS:BOOL=OFF
    -DBENCHMARK_ENABLE_INSTALL:BOOL=OFF
    -DBENCHMARK_ENABLE_LTO:BOOL=OFF
    -DBENCHMARK_ENABLE_TESTING:BOOL=OFF
    -DBENCHMARK_USE_LIBCXX:BOOL=OFF
    INSTALL_COMMAND ""
  )
endif() # BUILD_BENCHMARKS

if( ENABLE_HDF5 )
  find_package( HDF5 COMPONENTS C CXX HL REQUIRED )
  if( HDF5_FOUND AND NOT BUILD_OWN_HDF5 )
    include_directories( ${HDF5_INCLUDE_DIRS} )
    message( "HDF5 include dirs: ${HDF5_INCLUDE_DIRS}" )
    message( "HDF5 CXX library found: ${HDF5_CXX_LIBRARIES}" )
    set( _hdf5_libs ${HDF5_HL_LIBRARIES} ${HDF5_LIBRARIES} )
  else()
    message( "HDF5 library not found or wanted, trying to build it..." )
    include( ExternalProject )
    set( ExternalProjectCMakeArgs
      -DHDF5_BUILD_CPP_LIB=ON
      )
    set( _hdf5_version "1.10.3" )
    set( _hdf5_version_short "1.10" )
    #         set(_hdf5_version "1.8.21")
    #         set(_hdf5_version_short "1.8")
    file( MAKE_DIRECTORY ${PROJECT_BINARY_DIR}/hdf5_local/src )
    externalproject_add( hdf5_local
      DOWNLOAD_COMMAND curl https://support.hdfgroup.org/ftp/HDF5/releases/hdf5-${_hdf5_version_short}/hdf5-${_hdf5_version}/src/hdf5-${_hdf5_version}.tar.gz | tar xz -C "${PROJECT_BINARY_DIR}/hdf5_local/src"
      #DOWNLOAD_COMMAND cat ${hdc_SOURCE_DIR}/../tmp/hdf5-${_hdf5_version}.tar.gz | tar xz -C "${PROJECT_BINARY_DIR}/hdf5_local/src"
      CMAKE_ARGS ${ExternalProjectCMakeArgs}
      CMAKE_CACHE_ARGS "-DCMAKE_C_COMPILER:FILEPATH=${CMAKE_C_COMPILER}" "-DCMAKE_CXX_COMPILER:FILEPATH=${CMAKE_CXX_COMPILER}" "-DCMAKE_Fortran_COMPILER:FILEPATH=${CMAKE_Fortran_COMPILER}" # Forward compiler
      SOURCE_DIR ${PROJECT_BINARY_DIR}/hdf5_local/src/hdf5-${_hdf5_version}
      BINARY_DIR ${PROJECT_BINARY_DIR}/hdf5_local/build
      STAMP_DIR ${PROJECT_BINARY_DIR}/hdf5_local/stamp
      TMP_DIR ${PROJECT_BINARY_DIR}/hdf5_local/tmp
      INSTALL_COMMAND ""
      )
    include_directories(
      ${PROJECT_BINARY_DIR}/hdf5_local/src/hdf5-${_hdf5_version}/src
      ${PROJECT_BINARY_DIR}/hdf5_local/src/hdf5-${_hdf5_version}/c++/src
      ${PROJECT_BINARY_DIR}/hdf5_local/build
    )
    set( _hdf5_libs
      ${PROJECT_BINARY_DIR}/hdf5_local/build/bin/libhdf5_cpp.a
      ${PROJECT_BINARY_DIR}/hdf5_local/build/bin/libhdf5.a
      -ldl
      )
  endif() # BUILD_OWN_HDF5
  add_definitions( -D_USE_HDF5=1 )
endif() # ENABLE_HDF5

if( BUILD_MDBM_PLUGIN )
  find_path(
    MDBM_INCLUDE_DIR
    NAMES mdbm.h
    HINTS "/usr/include" "/usr/local/include" "/sw/mdbm/include"
  )
  find_library( MDBM_LIBRARY
    NAMES libmdbm mdbm
    HINTS "/usr/local/lib64" "/usr/local/lib" "/usr/local/lib64" "/usr/lib" "/usr/lib64" "/sw/mdbm/lib64"
  )
  if( IS_DIRECTORY ${MDBM_INCLUDE_DIR} AND EXISTS ${MDBM_LIBRARY} AND NOT BUILD_OWN_MDBM )
    message( "Found MDBM include: ${MDBM_INCLUDE_DIR}" )
    message( "Found MDBM library: ${MDBM_LIBRARY}" )
    set( _BUILD_MDBM TRUE )
  elseif( BUILD_OWN_MDBM )
    include( ExternalProject )
    externalproject_add( mdbmDownload
      GIT_REPOSITORY https://github.com/yahoo/mdbm.git
      CONFIGURE_COMMAND ${PROJECT_SOURCE_DIR}/scripts/mdbm_configure.sh ${CMAKE_CURRENT_BINARY_DIR}/mdbmDownload-prefix/src/mdbmDownload ${CMAKE_CURRENT_BINARY_DIR}/mdbmDownload-prefix
      BUILD_COMMAND $(MAKE)
      INSTALL_COMMAND $(MAKE) install
      BUILD_IN_SOURCE 1
      )
    install( DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/mdbmDownload-prefix/include/ DESTINATION include/hdc )
  if( APPLE )
    install( DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/mdbmDownload-prefix/lib/ DESTINATION lib/hdc )
  else()
    install( DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/mdbmDownload-prefix/lib64/ DESTINATION lib/hdc )
  endif()

    externalproject_get_property( mdbmDownload INSTALL_DIR )
    set( MDBM_INCLUDE_DIR ${INSTALL_DIR}/include )
  if( APPLE )
    set( MDBM_LIBRARY ${INSTALL_DIR}/lib/libmdbm.dylib )
  else()
    set( MDBM_LIBRARY ${INSTALL_DIR}/lib64/libmdbm.so )
  endif()
    set( _BUILD_MDBM TRUE )
    add_library( mdbm SHARED IMPORTED )
    set_target_properties( mdbm PROPERTIES IMPORTED_LOCATION ${MDBM_LIBRARY} )
  else()
    message( WARNING "libmdbm not found and not set to build. Skipping build of MDBM plugin..." )
  endif()
else() # BUILD_MDBM_PLUGIN
  message( "MDBM plugin disabled. Skipping..." )
endif() # BUILD_MDBM_PLUGIN

if( _BUILD_MDBM )
  add_definitions( -D_USE_MDBM=1 )
endif()

if( ENABLE_UDA )
  add_definitions( -D_USE_UDA=1 )
endif() # ENABLE_UDA

if( ENABLE_REDIS )
    add_definitions( -D_USE_REDIS=1 )
endif() # ENABLE_REDIS

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

add_subdirectory( thirdparty )
include_directories( thirdparty/Pluma-1.1/include )

# jsoncpp
include_directories( thirdparty/jsoncpp )
add_subdirectory( thirdparty/jsoncpp )

include_directories( thirdparty )

include_directories( include )
include_directories( include/plugins )
add_subdirectory( include )
add_subdirectory( src )
add_subdirectory( pkgconfig )
add_subdirectory( python ) # TODO: move to separate repo?

if( ENABLE_MATLAB )
  add_subdirectory( matlab )
endif()

if( ENABLE_FORTRAN )
  add_subdirectory( fortran )
endif()

if( ENABLE_JAVA )
  add_subdirectory( java )
endif()

if( BUILD_DOC )
  add_subdirectory( docs )
endif()

if( BUILD_TESTS )
  enable_testing()
  add_subdirectory( test )
endif()

if( BUILD_EXAMPLES )
  add_subdirectory( examples )
endif()

if( BUILD_BENCHMARKS )
  add_subdirectory( benchmark )
endif()
