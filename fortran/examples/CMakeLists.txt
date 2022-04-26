set(CMAKE_INCLUDE_CURRENT_DIR ON)
message("xxxxxx ${fhdc_BINARY_DIR}")
set(CMAKE_Fortran_MODULE_DIRECTORY "${fhdc_BINARY_DIR}/mod")
add_library(hdc_fortran_module SHARED hdc_fortran_module.f90)
target_link_libraries(hdc_fortran_module fhdc chdc hdc)

add_executable(hdc_example_fortran example_fortran.f90)
target_link_libraries(hdc_example_fortran fhdc chdc hdc)
set_target_properties(hdc_example_fortran PROPERTIES LINKER_LANGUAGE Fortran)

add_executable(hdc_example_cpo cpo_hdc.f90)
target_link_libraries(hdc_example_cpo fhdc chdc hdc)
set_target_properties(hdc_example_cpo PROPERTIES LINKER_LANGUAGE Fortran)

add_executable(hdc_example_lukas example_lukas.f90)
target_link_libraries(hdc_example_lukas fhdc chdc hdc)
set_target_properties(hdc_example_lukas PROPERTIES LINKER_LANGUAGE Fortran)

install(TARGETS hdc_example_fortran hdc_example_cpo hdc_fortran_module
    LIBRARY DESTINATION lib
    RUNTIME DESTINATION bin)
