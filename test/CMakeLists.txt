set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_Fortran_MODULE_DIRECTORY "${hdc_BINARY_DIR}/mod")
file(GLOB SRCS *.cpp)
add_executable(hdc_test ${SRCS})
target_link_libraries(hdc_test hdc chdc gtest gmock_main ${Boost_LIBRARIES})
add_test(NAME hdc_test COMMAND hdc_test WORKING_DIRECTORY "${hdc_BINARY_DIR}")
install(TARGETS hdc_test DESTINATION bin)

# Just to make CTest happy:
configure_file("${hdc_SOURCE_DIR}/pyhdc.py" "${hdc_BINARY_DIR}/src/pyhdc.py" COPYONLY)
configure_file("${hdc_TEST_DIR}/test_pyhdc.py" "${hdc_BINARY_DIR}/src/test_pyhdc.py" COPYONLY)


add_file_copy_target(test_pyhdc python test_pyhdc.py)
install_file_copy_target(test_pyhdc DESTINATION python)

add_test(NAME python_test COMMAND ${PYTHON_EXECUTABLE} "${hdc_BINARY_DIR}/src/test_pyhdc.py" WORKING_DIRECTORY "")
