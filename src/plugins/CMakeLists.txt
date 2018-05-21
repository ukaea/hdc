find_path(
  MDBM_INCLUDE_DIR
  NAMES mdbm.h
  HINTS "/usr/include" "/usr/local/include" "/sw/mdbm/include"
)

find_library(MDBM_LIBRARY
    NAMES libmdbm mdbm
    HINTS "/usr/local/lib64" "/usr/local/lib" "/usr/local/lib64" "/usr/lib" "/usr/lib64" "/sw/mdbm/lib64"
)
if (BUILD_MDBM_PLUGIN)
# Create MDBM plugin
    if (IS_DIRECTORY ${MDBM_INCLUDE_DIR} AND EXISTS ${MDBM_LIBRARY})
        message("Found MDBM include: ${MDBM_INCLUDE_DIR}")
        include_directories(${MDBM_INCLUDE_DIR})

        message("Found MDBM library: ${MDBM_LIBRARY}")

        add_library(hdc_plugin_mdbm SHARED mdbm_plugin.cpp storage_interface.cpp)
        target_link_libraries(hdc_plugin_mdbm ${MDBM_LIBRARY} Pluma dl ${Boost_LIBRARIES})
        install(TARGETS hdc_plugin_mdbm DESTINATION lib/hdc)
    else ()
        message("MDBM Not found, Building of MDBM Plugin will be skipped..."
    )
    endif()
endif()
# Create Unordered map storage
include_directories()
add_library(hdc_plugin_umap SHARED umap_plugin.cpp storage_interface.cpp)
target_link_libraries(hdc_plugin_umap Pluma dl)
install(TARGETS hdc_plugin_umap DESTINATION lib/hdc)
install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} DESTINATION include/hdc/plugins FILES_MATCHING PATTERN "*.h*")
