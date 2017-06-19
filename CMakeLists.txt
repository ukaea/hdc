cmake_minimum_required(VERSION 2.8)
project(hdc_new)
enable_language (Fortran)

### Set minimal compiler requirements
if (CMAKE_COMPILER_IS_GNUCC AND CMAKE_C_COMPILER_VERSION VERSION_LESS 4.9)
    message(FATAL_ERROR "
    I require at least gcc-4.9. Please, obtain higher version of gcc toolset and run me again after exporting path to newer compiler.

    For example:

    export FC=/usr/bin/gfortran-4.9
    export CC=/usr/bin/gcc-4.9
    export CXX=/usr/bin/g++-4.9
    ")
endif()### 



#set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")
#find_package(Sphinx REQUIRED)

set(hdc_new_SOURCE_DIR "${hdc_new_SOURCE_DIR}/src")
set(CMAKE_Fortran_MODULE_DIRECTORY "mod")

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -g -O2 -std=c99 -Wno-type-limits -Wall -Wextra -lgfortran -lstdc++ -fmax-errors=1 -fPIC -lc")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -O2 -std=c++11 -Wno-type-limits -Wall -Wextra -lgfortran -lstdc++ -lrt -fPIC -fmax-errors=1 -fPIC")
set(CMAKE_Fortran_FLAGS "-g -O2 -ffree-line-length-none -Wall -pedantic -fbacktrace -Wall -Wextra -lstdc++ -std=f2008 -fmax-errors=1 -fPIC -lc")

set(CMAKE_INCLUDE_PATH "/usr/include/boost148" ${CMAKE_INCLUDE_PATH} )
set(CMAKE_LIBRARY_PATH "/usr/lib64/boost148" ${CMAKE_LIBRARY_PATH} )
find_package(Boost 1.48.0 REQUIRED filesystem system program_options)
include_directories(${Boost_INCLUDE_DIRS})
link_directories(${Boost_LIBRARY_DIRS})

option(BUILD_TESTS "Build and install Google tests and benchmarks" OFF)
if (BUILD_TESTS)
### gtest
include(CTest)

if (CMAKE_VERSION VERSION_LESS 3.2)
    set(UPDATE_DISCONNECTED_IF_AVAILABLE "")
else()
    set(UPDATE_DISCONNECTED_IF_AVAILABLE "UPDATE_DISCONNECTED 1")
endif()

include(cmake/DownloadProject.cmake)
download_project(PROJ                googletest
                 GIT_REPOSITORY      https://github.com/google/googletest.git
                 GIT_TAG             release-1.8.0
                 ${UPDATE_DISCONNECTED_IF_AVAILABLE}
)

# Prevent GoogleTest from overriding our compiler/linker options
# when building with Visual Studio
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

add_subdirectory(${googletest_SOURCE_DIR} ${googletest_BINARY_DIR})

# When using CMake 2.8.11 or later, header path dependencies
# are automatically added to the gtest and gmock targets.
# For earlier CMake versions, we have to explicitly add the
# required directories to the header search path ourselves.
if (CMAKE_VERSION VERSION_LESS 2.8.11)
    include_directories("${gtest_SOURCE_DIR}/include"
                        "${gmock_SOURCE_DIR}/include")
endif()
endif()
### gtest

### gbenchmark
option(BUILD_BENCHMARKS "Whether to build benchmarks - uses google benchmarks." ON)
if (BUILD_BENCHMARKS)
if (CMAKE_VERSION VERSION_LESS 3.2)
    set(UPDATE_DISCONNECTED_IF_AVAILABLE "")
else()
    set(UPDATE_DISCONNECTED_IF_AVAILABLE "UPDATE_DISCONNECTED 1")
endif()

include(cmake/DownloadProject.cmake)
download_project(PROJ                googlebenchmark
                 GIT_REPOSITORY      https://github.com/google/benchmark.git
                 GIT_TAG             v1.1.0
                 ${UPDATE_DISCONNECTED_IF_AVAILABLE}
)

add_subdirectory(${googlebenchmark_SOURCE_DIR} ${googlebenchmark_BINARY_DIR})

# When using CMake 2.8.11 or later, header path dependencies
# are automatically added to the gtest and gmock targets.
# For earlier CMake versions, we have to explicitly add the
# required directories to the header search path ourselves.
# if (CMAKE_VERSION VERSION_LESS 2.8.11)
#     include_directories("${googlebenchmark_SOURCE_DIR}/include")
# endif()

include_directories("${googlebenchmark_SOURCE_DIR}/include")
endif()
### gbenchmark

### HDF5 begin

# the option is here so that we can plug it into other
# codes where HDF5 may be optional
option(ENABLE_HDF5 "Enable HDF5 support" ON)
option(HDF5_BUILD_OWN "Build own HDF5" OFF)
if(ENABLE_HDF5)
    find_package(HDF5 COMPONENTS C CXX HL REQUIRED)
    if(HDF5_FOUND AND NOT HDF5_BUILD_OWN)
        include_directories(${HDF5_INCLUDE_DIR})
        set(_hdf5_libs hdf5 hdf5_cpp)
        message("HDF5 library found: ${HDF5_INCLUDE_DIR}")
        message("HDF5 CXX library found: ${HDF5_CXX_LIBRARIES}")
    else()
        message("HDF5 library not found or wanted, trying to build it...")
	message("aaaaa curl https://support.hdfgroup.org/ftp/HDF5/current/src/hdf5-${_hdf5_version}.tar.gz | tar xvz -C ${PROJECT_BINARY_DIR}/hdf5_local/src")
        # we did not find it so we fetch it from the web, configure, compile, and link
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
    endif()
    add_definitions(-D_USE_HDF5=1)
endif()

### HDF5 end

option(DEBUG "Turns some debug output on or off." OFF)
if(DEBUG)
    add_definitions(-DDEBUG)
endif(DEBUG)


set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

message("Binary dir is: ${hdc_BINARY_DIR}")
message("Source dir is: ${hdc_SOURCE_DIR}")

########################## Pluma ###############################
file(GLOB Pluma_HEADERS ${CMAKE_SOURCE_DIR}/thirdparty/Pluma-1.1/include/Pluma/*)
file(GLOB Pluma_SOURCES ${CMAKE_SOURCE_DIR}/thirdparty/Pluma-1.1/src/Pluma/*)
include_directories(${CMAKE_SOURCE_DIR}/thirdparty/Pluma-1.1/include ${CMAKE_SOURCE_DIR}/thirdparty/Pluma-1.1/src/)
add_library(Pluma STATIC ${Pluma_SOURCES})


# file(COPY include DESTINATION ${CMAKE_BINARY_DIR}/include)

include_directories(include)
# add_subdirectory(docs)
add_subdirectory(doxy)
add_subdirectory(src)

if (BUILD_TESTS)
    add_subdirectory(test)
endif()

add_subdirectory(examples)
if (BUILD_BENCHMARKS)
    add_subdirectory(benchmark)
endif()

add_subdirectory(plugins)
