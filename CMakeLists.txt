cmake_minimum_required(VERSION 2.6)
project(hdc_new)
enable_language (Fortran)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -g -std=c99 -fmax-errors=20 -Wno-type-limits -Wall -Wextra -lgfortran -lstdc++")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -std=c++14 -fmax-errors=20 -Wno-type-limits -Wall -Wextra -lgfortran -lstdc++")
set(CMAKE_Fortran_FLAGS "-g -ffree-line-length-none -Wall -pedantic -fbacktrace -Wall -Wextra -lstdc++ -std=f2008")

site_name(HOSTNAME)
message("Hostname is: " ${HOSTNAME})
if (${HOSTNAME} STREQUAL "abacus")
    message("Setting libdynd path to /sw/libdynd...")
    link_directories(/sw/libdynd/lib)
    include_directories(/sw/libdynd/include .)
else()
    link_directories(/usr/local/lib)
endif()

add_library(hdc SHARED hdc.cpp jsoncpp.cpp)
target_link_libraries(hdc dynd dyndt)

add_library(chdc SHARED hdc_c.cpp)
target_link_libraries(chdc hdc stdc++)

add_library(fhdc SHARED hdc_fortran.f90)
target_link_libraries(fhdc chdc)

add_executable(test_hdc_cpp test_hdc_cpp.cpp)
target_link_libraries(test_hdc_cpp hdc)

add_executable(test_hdc_c test_hdc_c.c)
target_link_libraries(test_hdc_c chdc)

add_executable(test_hdc_fortran test_hdc_fortran.f90)
target_link_libraries(test_hdc_fortran fhdc)

add_executable(cpo_hdc cpo_hdc.f90)
target_link_libraries(cpo_hdc fhdc)


set(CMAKE_INSTALL_RPATH "/usr/local/lib")
set(CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE)

install(TARGETS test_hdc_cpp RUNTIME DESTINATION bin)
