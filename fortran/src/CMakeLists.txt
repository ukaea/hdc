set( CMAKE_INCLUDE_CURRENT_DIR ON )
set( CMAKE_Fortran_MODULE_DIRECTORY "${fhdc_BINARY_DIR}/mod" )

add_library( fhdc SHARED hdc_fortran.f90 )

if ( ENABLE_FORTRAN )
  link_directories( ${fhdc_SOURCE_DIR} )
  target_link_libraries( fhdc chdc )
else()
  target_include_directories( fhdc PUBLIC ${CHDC_INCLUDE_DIRS} )
  target_link_directories( fhdc PUBLIC ${CHDC_LIBRARY_DIRS} )
  target_link_libraries( fhdc PUBLIC ${CHDC_LIBRARIES} )  ## TODO: switch
endif( ENABLE_FORTRAN )

install( TARGETS fhdc RUNTIME DESTINATION bin LIBRARY DESTINATION lib )
install( DIRECTORY ${CMAKE_Fortran_MODULE_DIRECTORY}/ DESTINATION include/hdc )
