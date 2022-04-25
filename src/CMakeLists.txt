set( CMAKE_INCLUDE_CURRENT_DIR ON )
set( CMAKE_Fortran_MODULE_DIRECTORY "${hdc_BINARY_DIR}/mod" )
link_directories( ${hdc_SOURCE_DIR} )
set( HDC_SOURCES )
list( APPEND HDC_SOURCES hdc.cpp plugins/storage_interface.cpp hdc_utils.cpp hdc_errors.cpp )
set( HDC_LINK_LIBRARIES )
list( APPEND HDC_LINK_LIBRARIES ${Boost_LIBRARIES} Pluma dl hdcjsoncpp )

add_library( hdc SHARED ${HDC_SOURCES} )

add_subdirectory( serialization )

target_link_libraries( hdc PUBLIC ${HDC_LINK_LIBRARIES} serialization )

add_library( chdc SHARED hdc_c.cpp )
target_link_libraries( chdc hdc stdc++ serialization )


install( TARGETS hdc chdc RUNTIME DESTINATION bin LIBRARY DESTINATION lib )
install( DIRECTORY ${CMAKE_Fortran_MODULE_DIRECTORY}/ DESTINATION include/hdc )

add_subdirectory( plugins )
