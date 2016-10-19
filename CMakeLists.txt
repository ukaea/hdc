cmake_minimum_required(VERSION 2.8)
project(hdc_new)
enable_language (Fortran)

#set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")
#find_package(Sphinx REQUIRED)

set(hdc_new_SOURCE_DIR "${hdc_new_SOURCE_DIR}/src")
set(CMAKE_Fortran_MODULE_DIRECTORY "mod")

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -g -std=c99 -Wno-type-limits -Wall -Wextra -lgfortran -lstdc++ -fmax-errors=1")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -std=c++14 -Wno-type-limits -Wall -Wextra -lgfortran -lstdc++ -lrt -fPIC -fmax-errors=1")
set(CMAKE_Fortran_FLAGS "-g -ffree-line-length-none -Wall -pedantic -fbacktrace -Wall -Wextra -lstdc++ -std=f2008 -fmax-errors=1")

set(CMAKE_INCLUDE_PATH "/usr/include/boost148" ${CMAKE_INCLUDE_PATH} )
set(CMAKE_LIBRARY_PATH "/usr/lib64/boost148" ${CMAKE_LIBRARY_PATH} )
find_package(Boost 1.48.0 REQUIRED filesystem system program_options)
include_directories(${Boost_INCLUDE_DIRS})
link_directories(${Boost_LIBRARY_DIRS})

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
                 GIT_TAG             master
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

### gtest

### gbenchmark

if (CMAKE_VERSION VERSION_LESS 3.2)
    set(UPDATE_DISCONNECTED_IF_AVAILABLE "")
else()
    set(UPDATE_DISCONNECTED_IF_AVAILABLE "UPDATE_DISCONNECTED 1")
endif()

include(cmake/DownloadProject.cmake)
download_project(PROJ                googlebenchmark
                 GIT_REPOSITORY      https://github.com/google/benchmark.git
                 GIT_TAG             master
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
### gbenchmark

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
add_subdirectory(test)
add_subdirectory(examples)
add_subdirectory(benchmark)
add_subdirectory(plugins)
