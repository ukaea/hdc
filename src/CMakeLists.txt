set(CMAKE_INCLUDE_CURRENT_DIR ON)
enable_language(Fortran)
set(
  CMAKE_Fortran_MODULE_DIRECTORY
  "${hdc_new_BINARY_DIR}/mod")

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

option(BUILD_PYBIND "Build and install Python pybind lib" OFF)

if (BUILD_PYBIND)

	find_package(PythonLibs REQUIRED)
	message(${PYTHON_INCLUDE_DIRS})
	message(${PYTHON_LIBRARIES})
	include_directories(${PYTHON_INCLUDE_DIRS})

	add_library(hdc_python SHARED hdc_python.cpp)
	target_link_libraries(hdc_python hdc ${PYTHON_LIBRARIES})

	install(TARGETS hdc_python
	    RUNTIME DESTINATION bin
	    LIBRARY DESTINATION lib
	    ARCHIVE DESTINATION lib/static
	    )
endif (BUILD_PYBIND)

link_directories(${hdc_SOURCE_DIR}/src)

set(HDC_SOURCES)
list(APPEND HDC_SOURCES hdc.cpp jsoncpp.cpp storage_interface.cpp utils.cpp hdc_json.cpp)
set(HDC_LINK_LIBRARIES)
list(APPEND HDC_LINK_LIBRARIES ${Boost_LIBRARIES} Pluma dl)
if(HDF5_FOUND)
list(APPEND HDC_SOURCES hdc_hdf5.cpp)
list(APPEND HDC_LINK_LIBRARIES ${HDF5_LIBRARIES})
message("----- ${HDF5_LIBRARIES}")
endif(HDF5_FOUND)
add_library(hdc SHARED ${HDC_SOURCES})
target_link_libraries(hdc ${HDC_LINK_LIBRARIES})

add_library(chdc SHARED hdc_c.cpp)
target_link_libraries(chdc hdc stdc++)

add_library(fhdc SHARED hdc_fortran.f90)
target_link_libraries(fhdc chdc)

#get_property(languages GLOBAL PROPERTY ENABLED_LANGUAGES)
#message( "------- LANGUAGES:  ${languages}" )

set( CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_CURRENT_LIST_DIR}/../cmake )
include( UseCython )

find_package(PythonInterp REQUIRED)
find_package(PythonLibs REQUIRED)
find_package(PythonExtensions REQUIRED)
find_package(Cython REQUIRED)

cmake_policy(SET CMP0057 NEW)

add_cython_target(cyhdc "CXX"
 )
add_library(cyhdc MODULE ${cyhdc})
target_link_libraries(cyhdc hdc)
python_extension_module(cyhdc)

# install(TARGETS hdc chdc fhdc RUNTIME DESTINATION bin LIBRARY DESTINATION lib
# ARCHIVE DESTINATION lib/static )
