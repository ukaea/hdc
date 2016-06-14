set(CMAKE_INCLUDE_CURRENT_DIR ON)
enable_language (Fortran)

set(CMAKE_Fortran_MODULE_DIRECTORY "${hdc_new_BINARY_DIR}/mod")

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)


add_executable(example_cpp example_cpp.cpp)
target_link_libraries(example_cpp hdc)

add_executable(example_c example_c.c)
target_link_libraries(example_c chdc)

add_executable(example_fortran example_fortran.f90)
target_link_libraries(example_fortran fhdc)

add_executable(cpo_hdc cpo_hdc.f90)
target_link_libraries(cpo_hdc fhdc)

#add_executable(pokus calling_fortran_from_c.c)
#target_link_libraries(pokus fhdc)

set(CMAKE_INSTALL_RPATH "/usr/local/lib")
set(CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE)

# install(TARGETS test_hdc_cpp RUNTIME DESTINATION bin)