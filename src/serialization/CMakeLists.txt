set( SOURCES
    hdc_json_serializer.cpp
    hdc_serializer.cpp
)

if( _BUILD_MDBM )
    list( APPEND SOURCES hdc_hdc_serializer.cpp )
endif( _BUILD_MDBM )


if( HDF5_FOUND )
    list( APPEND SOURCES hdc_hdf5_serializer.cpp)
endif( HDF5_FOUND )

if( ENABLE_UDA )
    list( APPEND SOURCES hdc_uda_serializer.cpp )
endif( ENABLE_UDA )

if( ENABLE_FLATBUFFERS )
    find_package( Flatbuffers QUIET )
endif( ENABLE_FLATBUFFERS )
if( Flatbuffers_FOUND )
    get_target_property( FLATBUFFERS_INCLUDE_DIR flatbuffers::flatbuffers INTERFACE_INCLUDE_DIRECTORIES )
    get_filename_component( FLATBUFFERS_DIR ${FLATBUFFERS_INCLUDE_DIR} DIRECTORY )
    find_program( FLATC_EXE flatc HINTS ${FLATBUFFERS_DIR} PATH_SUFFIXES bin )
    add_definitions( -D_USE_FLATBUFFERS )
    add_custom_command(
        OUTPUT hdc_generated.h
        COMMAND ${FLATC_EXE} ${CMAKE_CURRENT_LIST_DIR}/hdc.fbs
        ARGS --cpp
        MAIN_DEPENDENCY hdc.fbs )
    list( APPEND SOURCES hdc_flatbuffers_serializer.cpp hdc_generated.h )
endif( Flatbuffers_FOUND )

if( ENABLE_LIBS3 )
    find_package( LibS3 )
endif( ENABLE_LIBS3 )
if( LibS3_FOUND )
    add_definitions( -D_USE_S3 )
    list( APPEND SOURCES hdc_s3_serializer.cpp )
endif( LibS3_FOUND )

add_library( serialization ${SOURCES} )
target_include_directories( serialization PUBLIC ${CMAKE_CURRENT_LIST_DIR} )

if( Flatbuffers_FOUND )
    target_link_libraries( serialization PUBLIC flatbuffers::flatbuffers )
endif( Flatbuffers_FOUND )

if( LibS3_FOUND )
    target_include_directories( serialization PRIVATE ${LibS3_INCLUDE_DIR} )
    target_link_libraries( serialization PUBLIC ${LibS3_LIBRARIES} )
endif( LibS3_FOUND )

#install headers
install( DIRECTORY ./ DESTINATION include/hdc FILES_MATCHING PATTERN "*.h*" )

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
