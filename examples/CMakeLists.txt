set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_Fortran_MODULE_DIRECTORY "${hdc_BINARY_DIR}/mod")

add_library(hdc_fortran_module SHARED hdc_fortran_module.f90)
target_link_libraries(hdc_fortran_module fhdc)

add_executable(hdc_example_cpp example_cpp.cpp)
target_link_libraries(hdc_example_cpp hdc)

add_executable(hdc_example_c example_c.c)
target_link_libraries(hdc_example_c chdc)

add_executable(hdc_example_fortran example_fortran.f90)
target_link_libraries(hdc_example_fortran fhdc)
set_target_properties(hdc_example_fortran PROPERTIES LINKER_LANGUAGE Fortran)

add_executable(hdc_example_cpo cpo_hdc.f90)
target_link_libraries(hdc_example_cpo fhdc)
set_target_properties(hdc_example_cpo PROPERTIES LINKER_LANGUAGE Fortran)

add_executable(hdc_example_lukas example_lukas.f90)
target_link_libraries(hdc_example_lukas fhdc)
set_target_properties(hdc_example_lukas PROPERTIES LINKER_LANGUAGE Fortran)

add_executable(hdc_example_interprocess example_interprocess.cpp)
if( APPLE )
  target_link_libraries(hdc_example_interprocess hdc)
else()
  target_link_libraries(hdc_example_interprocess hdc rt)
endif()

if(ENABLE_UDA)
    add_executable(hdc_example_uda example_uda.cpp)
    target_link_libraries(hdc_example_uda hdc)
    install(TARGETS hdc_example_uda
    LIBRARY DESTINATION lib
    RUNTIME DESTINATION bin)
endif(ENABLE_UDA)

install(TARGETS hdc_fortran_module
            hdc_example_cpp
            hdc_example_c
            hdc_example_fortran
            hdc_example_cpo
            hdc_example_interprocess
    LIBRARY DESTINATION lib
    RUNTIME DESTINATION bin)
