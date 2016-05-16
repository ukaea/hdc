set(CMAKE_INCLUDE_CURRENT_DIR ON)
enable_language (Fortran)
set(CMAKE_Fortran_MODULE_DIRECTORY "${hdc_new_BINARY_DIR}/mod")

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# set(CMAKE_Fortran_MODULE_DIRECTORY "${hdc_BINARY_DIR}/mod")


include_directories(inc)
# include_directories(${CMAKE_Fortran_MODULE_DIRECTORY})
link_directories(${hdc_SOURCE_DIR}/src)

add_library(hdc SHARED hdc.cpp jsoncpp.cpp)
target_link_libraries(hdc dynd dyndt)

add_library(chdc SHARED hdc_c.cpp)
target_link_libraries(chdc hdc stdc++)

add_library(fhdc SHARED hdc_fortran.f90)
target_link_libraries(fhdc chdc)

add_library(hdc_fortran_module SHARED hdc_fortran_module.f90)
target_link_libraries(hdc_fortran_module fhdc)

# add_library(hdc_python SHARED hdc_python.cpp)
# target_link_libraries(hdc_python hdc)