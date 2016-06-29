set(CMAKE_INCLUDE_CURRENT_DIR ON)
enable_language (Fortran)
set(CMAKE_Fortran_MODULE_DIRECTORY "${hdc_new_BINARY_DIR}/mod")

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

option(BUILD_PYBIND "Build and install Python pybind lib" OFF)

if (BUILD_PYBIND)
	find_package(PythonLibs REQUIRED)
	#include_directories(${hdc_new_BINARY_DIR}/src)
	include_directories(${PYTHON_INCLUDE_DIRS})
	#include_directories(/sw/python2/anaconda/include/)
	#include_directories(/compass/home/fridrich/workspace/pybind11/include/)

	add_library(hdc_python SHARED hdc_python.cpp)
	target_link_libraries(hdc_python hdc ${PYTHON_LIBRARIES})

	install(TARGETS hdc_python
	    RUNTIME DESTINATION bin
	    LIBRARY DESTINATION lib
	    ARCHIVE DESTINATION lib/static
	    )
endif (BUILD_PYBIND)

#include_directories(inc)
link_directories(${hdc_SOURCE_DIR}/src)

add_library(hdc SHARED hdc.cpp jsoncpp.cpp)
target_link_libraries(hdc dynd dyndt)

add_library(chdc SHARED hdc_c.cpp)
target_link_libraries(chdc hdc stdc++)

add_library(fhdc SHARED hdc_fortran.f90)
target_link_libraries(fhdc chdc)

add_library(hdc_fortran_module SHARED hdc_fortran_module.f90)
target_link_libraries(hdc_fortran_module fhdc)


install(TARGETS hdc chdc fhdc hdc_fortran_module 
    RUNTIME DESTINATION bin
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib/static
    )