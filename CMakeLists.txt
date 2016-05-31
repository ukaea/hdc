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

site_name(HOSTNAME)
message("Hostname is: " ${HOSTNAME})
if (${HOSTNAME} STREQUAL "abacus")
    message("Setting libdynd path to /sw/libdynd...")
    link_directories(/sw/libdynd/lib)
    include_directories(/sw/libdynd/include .)
else()
    link_directories(/usr/local/lib)
endif()

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
add_subdirectory(src/tests)
