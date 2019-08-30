cmake_minimum_required(VERSION 3.3)

########################################################################################################################
# CMake configuration
cmake_policy(SET CMP0057 NEW)

project(hdc)
enable_language(CXX C Fortran)

set(CMAKE_POSITION_INDEPENDENT_CODE YES)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED YES)
set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED YES)
set(CMAKE_Fortran_FORMAT FREE)

set(hdc_TEST_DIR "${hdc_SOURCE_DIR}/test")
set(hdc_SOURCE_DIR "${hdc_SOURCE_DIR}/src")
set(CMAKE_Fortran_MODULE_DIRECTORY "mod")

# Extra cmake modules to find libraries
set( CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_CURRENT_LIST_DIR}/cmake )
include( CTest )
include( FileCopyTargets )

# Set the possible values of build type for cmake-gui
set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS "DEBUG" "RELEASE")
# Set a default build type if none was specified
if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
    message(STATUS "Setting build type to 'RELEASE' as none was specified.")
    set(CMAKE_BUILD_TYPE RELEASE CACHE STRING "Choose the type of build." FORCE)
endif()

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
if (CMAKE_C_COMPILER_ID STREQUAL "Intel")
    add_compile_options(-Wno-type-limits -Wall -Wextra -pedantic)
elseif(CMAKE_C_COMPILER_ID STREQUAL "PGI")
elseif(CMAKE_C_COMPILER_ID STREQUAL "GNU")
    add_compile_options(-Wno-type-limits -Wall -Wextra -pedantic)
else()
    message("Warning: Unsupported C compiler: ${CMAKE_C_COMPILER_ID}")
endif()
# This can be useful for Boost>=1.67
# add_definitions(-DBOOST_UUID_RANDOM_GENERATOR_COMPAT)

find_package(Boost 1.48.0 REQUIRED filesystem system regex)
include_directories(SYSTEM ${Boost_INCLUDE_DIRS})
link_directories(${Boost_LIBRARY_DIRS})
message("Boost_INCLUDE_DIRS: ${Boost_INCLUDE_DIRS}")
message("Boost_LIBRARY_DIRS: ${Boost_LIBRARY_DIRS}")

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
option(BUILD_TESTS "Build and install tests" ON)
option(BUILD_EXAMPLES "Build and install examples" ON)
option(BUILD_BENCHMARKS "Whether to build benchmarks - uses google benchmarks." OFF)
option(ENABLE_HDF5 "Enable HDF5 support" ON)
option(ENABLE_UDA "Enable UDA support" OFF)
option(ENABLE_MATLAB "Enable MATLAB support" OFF)
option(ENABLE_JAVA "Enable JAVA support" OFF)
option(ENABLE_COVERAGE "Enable coverage testing (using gcovr)" ON)
option(BUILD_OWN_HDF5 "Build own HDF5" OFF)
option(DEBUG "Turns some debug output on or off." OFF)
option(GOPTION "adds -g to compiler flags" ON)
option(BUILD_DOC "Create and install the HTML based API documentation (requires Doxygen)" OFF)
option(BUILD_MDBM_PLUGIN "Enables MDBM storage plugin." ON)
option(BUILD_OWN_MDBM "Build own MDBM" OFF)

if (GOPTION)
    set( CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -g" )
    set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g" )
    set( CMAKE_Fortran_FLAGS "${CMAKE_Fortran_FLAGS} -g" )
endif() # GOPTION

if (BUILD_BENCHMARKS)
    include(ExternalProject)
    ExternalProject_Add(
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

if(ENABLE_HDF5)
    find_package(HDF5 COMPONENTS C CXX HL REQUIRED)
    if(HDF5_FOUND AND NOT BUILD_OWN_HDF5)
        include_directories(${HDF5_INCLUDE_DIRS})
        message("HDF5 include dirs: ${HDF5_INCLUDE_DIRS}")
        message("HDF5 CXX library found: ${HDF5_CXX_LIBRARIES}")
        set(_hdf5_libs ${HDF5_HL_LIBRARIES} ${HDF5_LIBRARIES})
    else()
        message("HDF5 library not found or wanted, trying to build it...")
        include(ExternalProject)
        set(ExternalProjectCMakeArgs
            -DHDF5_BUILD_CPP_LIB=ON
            )
        set(_hdf5_version "1.10.3")
        set(_hdf5_version_short "1.10")
#         set(_hdf5_version "1.8.21")
#         set(_hdf5_version_short "1.8")
        file(MAKE_DIRECTORY ${PROJECT_BINARY_DIR}/hdf5_local/src)
    ExternalProject_Add(hdf5_local
            DOWNLOAD_COMMAND curl https://support.hdfgroup.org/ftp/HDF5/releases/hdf5-${_hdf5_version_short}/hdf5-${_hdf5_version}/src/hdf5-${_hdf5_version}.tar.gz | tar xz -C "${PROJECT_BINARY_DIR}/hdf5_local/src"
            #DOWNLOAD_COMMAND cat ${hdc_SOURCE_DIR}/../tmp/hdf5-${_hdf5_version}.tar.gz | tar xz -C "${PROJECT_BINARY_DIR}/hdf5_local/src"
            CMAKE_ARGS ${ExternalProjectCMakeArgs}
            CMAKE_CACHE_ARGS "-DCMAKE_C_COMPILER:FILEPATH=${CMAKE_C_COMPILER}" "-DCMAKE_CXX_COMPILER:FILEPATH=${CMAKE_CXX_COMPILER}" "-DCMAKE_Fortran_COMPILER:FILEPATH=${CMAKE_Fortran_COMPILER}" # Forward compiler
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

if (BUILD_MDBM_PLUGIN)
    find_path(
        MDBM_INCLUDE_DIR
        NAMES mdbm.h
        HINTS "/usr/include" "/usr/local/include" "/sw/mdbm/include"
    )
    find_library(MDBM_LIBRARY
        NAMES libmdbm mdbm
        HINTS "/usr/local/lib64" "/usr/local/lib" "/usr/local/lib64" "/usr/lib" "/usr/lib64" "/sw/mdbm/lib64"
    )
    if (IS_DIRECTORY ${MDBM_INCLUDE_DIR} AND EXISTS ${MDBM_LIBRARY})
        message("Found MDBM include: ${MDBM_INCLUDE_DIR}")
        message("Found MDBM library: ${MDBM_LIBRARY}")
        set(_BUILD_MDBM TRUE)
    elseif(BUILD_OWN_MDBM)
        include(ExternalProject)
        ExternalProject_Add(mdbm
            GIT_REPOSITORY https://github.com/yahoo/mdbm.git
            CONFIGURE_COMMAND ""
            BUILD_COMMAND make -j PREFIX=${CMAKE_CURRENT_BINARY_DIR}/mdbm
            INSTALL_COMMAND make  install PREFIX=${CMAKE_CURRENT_BINARY_DIR}/mdbm
            PREFIX=${CMAKE_CURRENT_BINARY_DIR}/mdbm
            BUILD_IN_SOURCE 1
        )
        set(MDBM_INCLUDE_DIR ${CMAKE_CURRENT_BINARY_DIR}/mdbm/include)
        set(MDBM_LIBRARY ${CMAKE_CURRENT_BINARY_DIR}/mdbm/lib64/libmdbm.so)
        set(_BUILD_MDBM TRUE)
    else()
        message("libmdbm not found and not set to build. Skipping build of MDBM plugin...")
    endif()
else() # BUILD_MDBM_PLUGIN
    message("MDBM plugin disabled. Skipping...")
endif() # BUILD_MDBM_PLUGIN

if(ENABLE_UDA)
    add_definitions(-D_USE_UDA=1)
endif() # ENABLE_UDA

if(DEBUG)
    add_definitions(-DDEBUG)
endif() # DEBUG

if(CMAKE_COMPILER_IS_GNUCXX AND CMAKE_BUILD_TYPE MATCHES "DEBUG" AND ENABLE_COVERAGE)
    include(CodeCoverage)
    APPEND_COVERAGE_COMPILER_FLAGS()
endif()

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
include_directories(include/plugins)
add_subdirectory(include)
add_subdirectory(src)
add_subdirectory(python)
if (ENABLE_MATLAB)
    add_subdirectory(matlab)
endif()
add_subdirectory(misc)
if (ENABLE_JAVA)
    add_subdirectory(java)
endif()
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
