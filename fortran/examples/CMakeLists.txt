set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_Fortran_MODULE_DIRECTORY "${hdc_BINARY_DIR}/mod")
add_library(hdc_fortran_module SHARED hdc_fortran_module.f90)
target_link_libraries(hdc_fortran_module fhdc)

add_executable(hdc_example_fortran example_fortran.f90)
target_link_libraries(hdc_example_fortran fhdc)
set_target_properties(hdc_example_fortran PROPERTIES LINKER_LANGUAGE Fortran)

add_executable(hdc_example_cpo cpo_hdc.f90)
target_link_libraries(hdc_example_cpo fhdc)
set_target_properties(hdc_example_cpo PROPERTIES LINKER_LANGUAGE Fortran)

add_executable(hdc_example_lukas example_lukas.f90)
target_link_libraries(hdc_example_lukas fhdc)
set_target_properties(hdc_example_lukas PROPERTIES LINKER_LANGUAGE Fortran)

set(HDC_EXAMPLES hdc_example_fortran hdc_example_cpo hdc_fortran_module)

install(TARGETS ${HDC_EXAMPLES}
    LIBRARY DESTINATION lib
    RUNTIME DESTINATION bin)
