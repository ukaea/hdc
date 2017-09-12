# Ensure CMake to find mdbm:
# include_directories("/usr/local/include")
# link_directories("/usr/local/lib64")
find_path(MDBM_INCLUDE_DIR
  NAMES mdbm.h
  HINTS "/usr/include" "/usr/local/include" "/sw/mdbm/include"
)


find_library(MDBM_LIBRARY
    NAMES libmdbm mdbm
    HINTS "/usr/local/lib" "/usr/local/lib64" "/usr/lib" "/usr/lib64" "/sw/mdbm/lib64"
)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin/plugins)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin/plugins)

# Create MDBM plugin
if (IS_DIRECTORY ${MDBM_INCLUDE_DIR} AND EXISTS ${MDBM_LIBRARY})
    message("Found MDBM include: ${MDBM_INCLUDE_DIR}")
    include_directories(${MDBM_INCLUDE_DIR})

    message("Found MDBM library: ${MDBM_LIBRARY}")

    add_library(MDBMPlugin SHARED mdbm_plugin.cpp  ../src/storage_interface.cpp ../src/jsoncpp.cpp)
    target_link_libraries(MDBMPlugin ${MDBM_LIBRARY} Pluma dl)
else ()
    message("MDBM Not found, Building of MDBM Plugin will be skipped...")
endif()

# Create Unordered map storage
add_library(UMapPlugin SHARED umap_plugin.cpp ../src/storage_interface.cpp)
target_link_libraries(UMapPlugin Pluma dl)
