set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_Fortran_MODULE_DIRECTORY "${hdc_BINARY_DIR}/mod")
link_directories(${hdc_SOURCE_DIR}/src)
set(HDC_SOURCES)
list(APPEND HDC_SOURCES hdc.cpp storage_interface.cpp hdc_utils.cpp hdc_json.cpp hdc_uda.cpp hdc_errors.cpp hdc_hdf5.cpp)
set(HDC_LINK_LIBRARIES)
list(APPEND HDC_LINK_LIBRARIES ${Boost_LIBRARIES} Pluma dl jsoncpp)
if(HDF5_FOUND)
    list(APPEND HDC_LINK_LIBRARIES ${HDF5_LIBRARIES})
endif(HDF5_FOUND)
if(ENABLE_UDA)
    list(APPEND HDC_LINK_LIBRARIES ${UDA_LIBRARIES})
    include_directories(${UDA_INCLUDE_DIR})
endif(ENABLE_UDA)
add_library(hdc SHARED ${HDC_SOURCES})
target_link_libraries(hdc ${HDC_LINK_LIBRARIES})

add_library(chdc SHARED hdc_c.cpp)
target_link_libraries(chdc hdc stdc++)

add_library(fhdc SHARED hdc_fortran.f90)
target_link_libraries(fhdc chdc)

find_package(PythonInterp REQUIRED)
find_package(PythonLibs REQUIRED)
find_package(PythonExtensions REQUIRED)
find_package(Cython REQUIRED)

add_cython_target(cyhdc "CXX")
add_library(cyhdc MODULE ${cyhdc})
target_link_libraries(cyhdc hdc)
python_extension_module(cyhdc)

install(TARGETS hdc chdc fhdc RUNTIME DESTINATION bin LIBRARY DESTINATION lib)
# TODO temporary Python install
# create Python install package in dist in the future
install(TARGETS cyhdc RUNTIME DESTINATION bin LIBRARY DESTINATION python)
add_file_copy_target(pyhdc python pyhdc.py)
install_file_copy_target(pyhdc DESTINATION python)
