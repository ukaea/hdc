set( CMAKE_INCLUDE_CURRENT_DIR ON )
set( CMAKE_Fortran_MODULE_DIRECTORY "${hdc_BINARY_DIR}/mod" )
link_directories( ${hdc_SOURCE_DIR} )

add_library( fhdc SHARED hdc_fortran.f90 )
target_link_libraries( fhdc chdc serialization )

install( TARGETS fhdc RUNTIME DESTINATION bin LIBRARY DESTINATION lib )
install( DIRECTORY ${CMAKE_Fortran_MODULE_DIRECTORY}/ DESTINATION include/hdc )

add_subdirectory( plugins )
