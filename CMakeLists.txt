cmake_minimum_required(VERSION 2.6)
project(hdc_new)
enable_language (Fortran)

# set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")
# find_package(Sphinx REQUIRED)

set(hdc_new_SOURCE_DIR "${hdc_new_SOURCE_DIR}/src")
set(CMAKE_Fortran_MODULE_DIRECTORY "mod")

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -g -std=c99 -Wno-type-limits -Wall -Wextra -lgfortran -lstdc++")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -std=c++14 -Wno-type-limits -Wall -Wextra -lgfortran -lstdc++")
set(CMAKE_Fortran_FLAGS "-g -ffree-line-length-none -Wall -pedantic -fbacktrace -Wall -Wextra -lstdc++ -std=f2008")

find_package(PythonLibs REQUIRED)
message(${PYTHON_INCLUDE_DIRS})
message(${PYTHON_LIBRARIES})
include_directories(${PYTHON_INCLUDE_DIRS})

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



site_name(HOSTNAME)
message("Hostname is: " ${HOSTNAME})
if (${HOSTNAME} STREQUAL "abacus")
    message("Setting libdynd path to /sw/libdynd...")
    link_directories(/sw/libdynd/lib)
    include_directories(/sw/libdynd/include .)
else()
    link_directories(/usr/local/lib)
endif()


option(DEBUG "Turns some debug output on or off.")
if(DEBUG)
    add_definitions(-DDEBUG)
endif(DEBUG)


set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

message("Binary dir is: ${hdc_BINARY_DIR}")
message("Source dir is: ${hdc_SOURCE_DIR}")

# file(COPY inc DESTINATION ${CMAKE_BINARY_DIR}/inc)

include_directories(inc)
# add_subdirectory(docs)
add_subdirectory(doxy)
add_subdirectory(src)
add_subdirectory(test)
add_subdirectory(examples)


