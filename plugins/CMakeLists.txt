# Ensure CMake to find mdbm:
include_directories("/usr/local/include")
link_directories("/usr/local/lib64")

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin/plugins)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin/plugins)


# Create MDBM plugin
add_library(MDBMPlugin SHARED mdbm_plugin.cpp  ../src/storage_interface.cpp)
target_link_libraries(MDBMPlugin mdbm jsoncpp Pluma dl)

# Create Unordered map storage
add_library(UMapPlugin SHARED umap_plugin.cpp ../src/storage_interface.cpp)
target_link_libraries(UMapPlugin Pluma dl)