set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_Fortran_MODULE_DIRECTORY "${hdc_BINARY_DIR}/mod")
link_directories(${hdc_SOURCE_DIR}/src)
set(HDC_SOURCES)
list(APPEND HDC_SOURCES hdc.cpp plugins/storage_interface.cpp hdc_utils.cpp hdc_json.cpp hdc_uda.cpp hdc_errors.cpp hdc_hdf5.cpp)

if(ENABLE_UDA)
  find_package(UDA REQUIRED)
  include_directories(${UDA_INCLUDE_DIRS})
  link_directories(${UDA_FAT_CPP_LIBRARY_DIRS})
  add_definitions(${UDA_FAT_CPP_CFLAGS_OTHER})
endif(ENABLE_UDA)

add_library(hdc SHARED ${HDC_SOURCES})

if(ENABLE_UDA)
  target_link_libraries(hdc PRIVATE ${UDA_FAT_CPP_LIBRARIES} MdsLib netcdf)
endif(ENABLE_UDA)

target_link_libraries(hdc PUBLIC Pluma dl jsoncpp Boost::regex Boost::filesystem Boost::system)
if(HDF5_FOUND)
    target_link_libraries(hdc PUBLIC ${_hdf5_libs})
endif()

add_library(chdc SHARED hdc_c.cpp)
target_link_libraries(chdc hdc stdc++)

add_library(fhdc SHARED hdc_fortran.f90)
target_link_libraries(fhdc chdc)

install(TARGETS hdc chdc fhdc RUNTIME DESTINATION bin LIBRARY DESTINATION lib)
install(DIRECTORY ${CMAKE_Fortran_MODULE_DIRECTORY}/ DESTINATION include/hdc)

add_subdirectory(plugins)
