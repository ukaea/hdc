set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_Fortran_MODULE_DIRECTORY "${hdc_BINARY_DIR}/mod")

add_executable(hdc_test hdc_test_cpp.cpp main.cpp)
target_link_libraries(hdc_test hdc ${Boost_LIBRARIES})
add_test(NAME hdc_test COMMAND hdc_test WORKING_DIRECTORY "${hdc_BINARY_DIR}")
if (_BUILD_MDBM)
    target_compile_definitions(hdc_test PRIVATE -D_USE_MDBM=1)
endif()
install(TARGETS hdc_test DESTINATION bin)

add_executable(hdc_test_c hdc_test_c.cpp main.cpp)
target_link_libraries(hdc_test_c hdc chdc ${Boost_LIBRARIES})
add_test(NAME hdc_test_c COMMAND hdc_test_c WORKING_DIRECTORY "${hdc_BINARY_DIR}")
if (_BUILD_MDBM)
    target_compile_definitions(hdc_test_c PRIVATE -D_USE_MDBM=1)
endif()
install(TARGETS hdc_test_c DESTINATION bin)

if(CMAKE_COMPILER_IS_GNUCXX AND CMAKE_BUILD_TYPE MATCHES "DEBUG" AND ENABLE_COVERAGE)
    set(COVERAGE_GCOVR_EXCLUDES '../build/*' '../thirdparty/*' '../examples/*' '../test/*')
    SETUP_TARGET_FOR_COVERAGE_GCOVR_HTML(NAME test_coverage EXECUTABLE hdc_test DEPENDENCIES hdc_test)
endif()

# Just to make CTest happy:
configure_file("${CMAKE_SOURCE_DIR}/python/pyhdc.py" "${hdc_BINARY_DIR}/python/pyhdc.py" COPYONLY)
configure_file("${hdc_TEST_DIR}/test_pyhdc.py" "${hdc_BINARY_DIR}/python/test_pyhdc.py" COPYONLY)


# add_file_copy_target(test_pyhdc python test_pyhdc.py)
# install_file_copy_target(test_pyhdc DESTINATION python)

# add_test(NAME python_test COMMAND ${PYTHON_EXECUTABLE} "${hdc_BINARY_DIR}/python/test_pyhdc.py" WORKING_DIRECTORY "")
# TODO: make this more portable
# add_test(NAME python_test COMMAND ${PYTHON_EXECUTABLE} "-m pytest" WORKING_DIRECTORY "${hdc_BINARY_DIR}/src")
