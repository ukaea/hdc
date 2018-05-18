cmake_minimum_required(VERSION 3.3)

########################################################################################################################
# CMake configuration
cmake_policy(SET CMP0057 NEW)

project(hdc)
enable_language(CXX C Fortran)

set(hdc_TEST_DIR "${hdc_SOURCE_DIR}/test")
set(hdc_SOURCE_DIR "${hdc_SOURCE_DIR}/src")
set(CMAKE_Fortran_MODULE_DIRECTORY "mod")

# Extra cmake modules to find libraries
set( CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_CURRENT_LIST_DIR}/cmake )
include( CTest )
include( FileCopyTargets )

########################################################################################################################
# Compiler configuration

# Set minimal compiler requirements
if(CMAKE_COMPILER_IS_GNUCC AND CMAKE_C_COMPILER_VERSION VERSION_LESS 4.9)
  message(
    FATAL_ERROR

    "
    I require at least gcc-4.9. Please, obtain higher version of gcc toolset and run me again after exporting path to a newer compiler.

    For example:

    export FC=/usr/bin/gfortran-4.9
    export CC=/usr/bin/gcc-4.9
    export CXX=/usr/bin/g++-4.9
    ")
endif()

#set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")
#find_package(Sphinx REQUIRED)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O2 -std=c99 -Wno-type-limits -Wall -Wextra -lgfortran -lstdc++ -fmax-errors=1 -fPIC -lc")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O2 -std=c++11 -Wno-type-limits -Wall -Wextra -lgfortran -lstdc++ -lrt -fPIC -fmax-errors=1")
set(CMAKE_Fortran_FLAGS "-O2 -ffree-line-length-none -Wall -pedantic -fbacktrace -Wall -Wextra -lstdc++ -std=f2008 -fmax-errors=1 -fPIC -lc")

set(CMAKE_INCLUDE_PATH "/usr/include/boost148" ${CMAKE_INCLUDE_PATH} )
set(CMAKE_LIBRARY_PATH "/usr/lib64/boost148" ${CMAKE_LIBRARY_PATH} )
find_package(Boost 1.48.0 REQUIRED filesystem system program_options regex)
include_directories(${Boost_INCLUDE_DIRS})
link_directories(${Boost_LIBRARY_DIRS})

########################################################################################################################
# RPATH options
# https://cmake.org/Wiki/CMake_RPATH_handling#Always_full_RPATH

# use, i.e. don't skip the full RPATH for the build tree
SET(CMAKE_SKIP_BUILD_RPATH  FALSE)

# when building, don't use the install RPATH already
# (but later on when installing)
SET(CMAKE_BUILD_WITH_INSTALL_RPATH FALSE)

SET(CMAKE_INSTALL_RPATH "${CMAKE_INSTALL_PREFIX}/lib")

# add the automatically determined parts of the RPATH
# which point to directories outside the build tree to the install RPATH
SET(CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE)


# the RPATH to be used when installing, but only if it's not a system directory
LIST(FIND CMAKE_PLATFORM_IMPLICIT_LINK_DIRECTORIES "${CMAKE_INSTALL_PREFIX}/lib" isSystemDir)
IF("${isSystemDir}" STREQUAL "-1")
   SET(CMAKE_INSTALL_RPATH "${CMAKE_INSTALL_PREFIX}/lib")
ENDIF("${isSystemDir}" STREQUAL "-1")

########################################################################################################################
# Build options
option(BUILD_TESTS "Build and install Google tests" ON)
option(BUILD_EXAMPLES "Build and install examples" ON)
option(BUILD_BENCHMARKS "Whether to build benchmarks - uses google benchmarks." ON)
option(ENABLE_HDF5 "Enable HDF5 support" ON)
option(ENABLE_UDA "Enable UDA support" OFF)
option(BUILD_OWN_HDF5 "Build own HDF5" OFF)
option(DEBUG "Turns some debug output on or off." OFF)
option(GOPTION "adds -g to compiler flags" ON)
option(BUILD_DOC "Create and install the HTML based API documentation (requires Doxygen)" OFF)
option(BUILD_MDBM_PLUGIN "Enables MDBM storage plugin." ON)

if (GOPTION)
    set( CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -g" )
    set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g" )
    set( CMAKE_Fortran_FLAGS "${CMAKE_Fortran_FLAGS} -g" )
endif() # GOPTION

if (BUILD_TESTS)
    # Prevent GoogleTest from overriding our compiler/linker options
    # when building with Visual Studio
    set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
    add_subdirectory("thirdparty/googletest/")
endif() # BUILD_TESTS

if (BUILD_BENCHMARKS)
    add_subdirectory("thirdparty/googlebenchmark/")
endif() # BUILD_BENCHMARKS

if(ENABLE_HDF5)
    find_package(HDF5 COMPONENTS C CXX HL REQUIRED)
    if(HDF5_FOUND AND NOT BUILD_OWN_HDF5)
        include_directories(${HDF5_INCLUDE_DIR})
        set(_hdf5_libs hdf5 hdf5_cpp)
        message("HDF5 library found: ${HDF5_INCLUDE_DIR}")
        message("HDF5 CXX library found: ${HDF5_CXX_LIBRARIES}")
    else()
        message("HDF5 library not found or wanted, trying to build it...")
        include(ExternalProject)
        set(ExternalProjectCMakeArgs
            -DHDF5_BUILD_CPP_LIB=ON
            )
        set(_hdf5_version "1.10.0-patch1")
        file(MAKE_DIRECTORY ${PROJECT_BINARY_DIR}/hdf5_local/src)
	ExternalProject_Add(hdf5_local
            DOWNLOAD_COMMAND curl https://support.hdfgroup.org/ftp/HDF5/current/src/hdf5-${_hdf5_version}.tar.gz | tar xvz -C "${PROJECT_BINARY_DIR}/hdf5_local/src"
            CMAKE_ARGS ${ExternalProjectCMakeArgs}
            SOURCE_DIR ${PROJECT_BINARY_DIR}/hdf5_local/src/hdf5-${_hdf5_version}
            BINARY_DIR ${PROJECT_BINARY_DIR}/hdf5_local/build
            STAMP_DIR  ${PROJECT_BINARY_DIR}/hdf5_local/stamp
            TMP_DIR    ${PROJECT_BINARY_DIR}/hdf5_local/tmp
            INSTALL_COMMAND ""
            )
        include_directories(
            ${PROJECT_BINARY_DIR}/hdf5_local/src/hdf5-${_hdf5_version}/src
            ${PROJECT_BINARY_DIR}/hdf5_local/src/hdf5-${_hdf5_version}/c++/src
            ${PROJECT_BINARY_DIR}/hdf5_local/build
            )
        set(_hdf5_libs
            ${PROJECT_BINARY_DIR}/hdf5_local/build/bin/libhdf5_cpp.a
            ${PROJECT_BINARY_DIR}/hdf5_local/build/bin/libhdf5.a
            -ldl
            )
    endif() # BUILD_OWN_HDF5
    add_definitions(-D_USE_HDF5=1)
endif() # ENABLE_HDF5

if(ENABLE_UDA)
    find_package( PkgConfig QUIET)
    pkg_check_modules( UDA QUIET uda-fat-cpp uda-fat-client )
    string(REPLACE ";" " " UDA_CFLAGS_OTHER "${UDA_CFLAGS_OTHER}")
    set( CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${UDA_CFLAGS_OTHER}" )
    set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${UDA_CFLAGS_OTHER}" )
    set( CMAKE_Fortran_FLAGS "${CMAKE_Fortran_FLAGS} ${UDA_CFLAGS_OTHER}" )
    link_directories(${UDA_LIBRARY_DIRS})
    add_definitions(-D_USE_UDA=1)
endif() # ENABLE_UDA

if(DEBUG)
    add_definitions(-DDEBUG)
endif() # DEBUG

message("Binary dir is: ${hdc_BINARY_DIR}")
message("Source dir is: ${hdc_SOURCE_DIR}")


########################################################################################################################
# Source directories

add_subdirectory(thirdparty)
include_directories(thirdparty/Pluma-1.1/include)

# jsoncpp
include_directories(thirdparty/jsoncpp)
add_subdirectory(thirdparty/jsoncpp)

include_directories(thirdparty)

include_directories(include)
add_subdirectory(include)
add_subdirectory(src)
add_subdirectory(plugins)
add_subdirectory(python)
add_subdirectory(misc)

if (BUILD_DOC)
    # add_subdirectory(docs)
    add_subdirectory(doxy)
endif()
if(BUILD_TESTS)
    enable_testing()
    add_subdirectory(test)
endif()
if(BUILD_EXAMPLES)
    add_subdirectory(examples)
endif()
if(BUILD_BENCHMARKS)
    add_subdirectory(benchmark)
endif()
