cmake_minimum_required(VERSION 2.6)
project(hdc_new)
enable_language (Fortran)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=c99 -fmax-errors=20 -Wno-type-limits -Wall -Wextra -lgfortran")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14 -fmax-errors=20 -Wno-type-limits -Wall -Wextra -lgfortran")

# set (CMAKE_Fortran_FLAGS "-c -ffree-line-length-none")
set(CMAKE_Fortran_FLAGS "-O2 -ffree-line-length-none -Wall -pedantic -fbacktrace -Wall -Wextra -fcheck=all -lstdc++")


add_executable(test_hdc_cpp test_hdc_cpp.cpp hdc.cpp)
target_link_libraries(test_hdc_cpp dynd dyndt jsoncpp)

add_executable(test_hdc_c test_hdc_c.c hdc_c.cpp hdc.cpp)
target_link_libraries(test_hdc_c dynd dyndt jsoncpp)

add_executable(test_hdc_fortran test_hdc_fortran.f90 hdc_fortran.f90 hdc_c.cpp hdc.cpp)
target_link_libraries(test_hdc_fortran stdc++ dynd dyndt jsoncpp)

# add_executable(cpo_hdc cpo_hdc.f90 hdc_fortran.f90 hdc_c.cpp hdc.cpp)
# target_link_libraries(cpo_hdc stdc++ dynd dyndt jsoncpp)


set(CMAKE_INSTALL_RPATH "/usr/local/lib")
set(CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE)

install(TARGETS test_hdc_cpp RUNTIME DESTINATION bin)
