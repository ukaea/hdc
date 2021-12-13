# copy all from python directory
file(COPY "${CMAKE_CURRENT_SOURCE_DIR}" DESTINATION "${hdc_BINARY_DIR}")

# overwrite setup.py with cmake varibles replaced by values
configure_file("${CMAKE_CURRENT_SOURCE_DIR}/setup.py" "${CMAKE_CURRENT_BINARY_DIR}/setup.py")

# Update also test_binder/run
set(RUN_IN "${CMAKE_CURRENT_SOURCE_DIR}/tests_binder/run")
set(RUN "${CMAKE_CURRENT_BINARY_DIR}/tests_binder/run")
configure_file(${RUN_IN} ${RUN})
