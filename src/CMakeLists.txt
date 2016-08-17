set(CMAKE_INCLUDE_CURRENT_DIR ON)
enable_language (Fortran)
set(CMAKE_Fortran_MODULE_DIRECTORY "${hdc_new_BINARY_DIR}/mod")

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

#include_directories(inc)
link_directories(${hdc_SOURCE_DIR}/src)

add_library(hdc SHARED hdc.cpp jsoncpp.cpp storage_interface.cpp utils.cpp hdc_json.cpp)
target_link_libraries(hdc ${Boost_LIBRARIES} Pluma dl)

# add_library(chdc SHARED hdc_c.cpp)
# target_link_libraries(chdc hdc stdc++)
# 
# add_library(fhdc SHARED hdc_fortran.f90)
# target_link_libraries(fhdc chdc)


# install(TARGETS hdc chdc fhdc 
#     RUNTIME DESTINATION bin
#     LIBRARY DESTINATION lib
#     ARCHIVE DESTINATION lib/static
#     )