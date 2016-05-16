set(CMAKE_INCLUDE_CURRENT_DIR ON)
enable_language (Fortran)

set(CMAKE_Fortran_MODULE_DIRECTORY "${hdc_new_BINARY_DIR}/mod")

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

include_directories(inc)

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

# install(TARGETS test_hdc_cpp RUNTIME DESTINATION bin)
