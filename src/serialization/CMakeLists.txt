set( SOURCES
  hdc_flatbuffers_serializer.cpp
  hdc_hdc_serializer.cpp
  hdc_hdf5_serializer.cpp
  hdc_json_serializer.cpp
  hdc_s3_serializer.cpp
  hdc_serializer.cpp
  hdc_uda_serializer.cpp
)

if( HDF5_FOUND )
  list (APPEND SOURCES hdc_hdf5_serializer.cpp hdc_hdc_serializer.cpp)
endif( HDF5_FOUND )
if( ENABLE_UDA )
  list (APPEND SOURCES hdc_uda_serializer.cpp )
endif( ENABLE_UDA )

add_library( serialization ${SOURCES} )
target_include_directories( serialization PUBLIC ${CMAKE_CURRENT_LIST_DIR} )

find_package( Flatbuffers )
if( Flatbuffers_FOUND )
  add_definitions( -D_USE_FLATBUFFERS )
  target_link_libraries( serialization PUBLIC flatbuffers::flatbuffers )
endif( Flatbuffers_FOUND )

find_package( LibS3 )
if( LIBS3_FOUND )
  add_definitions( -D_USE_S3 )
  target_include_directories( serialization PRIVATE ${LIBS3_INCLUDE_DIR} )
  target_link_libraries( serialization PUBLIC ${LIBS3_LIBRARIES} )
endif( LIBS3_FOUND )

#install headers
install(DIRECTORY ./ DESTINATION include/hdc FILES_MATCHING PATTERN "*.h*")

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
