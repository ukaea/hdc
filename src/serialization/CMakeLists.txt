set( SOURCES
  hdc_hdc_serializer.cpp
  hdc_hdf5_serializer.cpp
  hdc_json_serializer.cpp
  hdc_serializer.cpp
  hdc_uda_serializer.cpp
)

add_library( serialization ${SOURCES} )
target_include_directories( serialization PUBLIC ${CMAKE_CURRENT_LIST_DIR} )

if( HDF5_FOUND )
  target_link_libraries( serialization PRIVATE ${_hdf5_libs} )
endif( HDF5_FOUND )

if( ENABLE_UDA )
  find_package( UDA REQUIRED )
  target_include_directories( serialization PRIVATE ${UDA_INCLUDE_DIRS} )
  target_link_directories( serialization PUBLIC ${UDA_FAT_CPP_LIBRARY_DIRS} )
#  target_compile_definitions( serialization PRIVATE ${UDA_FAT_CPP_CFLAGS_OTHER} )
  target_link_libraries( serialization PUBLIC ${UDA_FAT_CPP_LIBRARIES} )
  target_compile_definitions( serialization PRIVATE FATCLIENT )
endif( ENABLE_UDA )