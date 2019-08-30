# stolen from https://bloerg.net/2012/11/10/cmake-and-distutils.html
set(SETUP_PY_IN "${CMAKE_CURRENT_SOURCE_DIR}/setup.py.in")
set(SETUP_PY    "${CMAKE_CURRENT_BINARY_DIR}/setup.py")
set(OUTPUT      "${CMAKE_CURRENT_BINARY_DIR}/build/timestamp")

configure_file("${CMAKE_CURRENT_SOURCE_DIR}/cyhdc.pyx" "${CMAKE_CURRENT_BINARY_DIR}/cyhdc.pyx")
configure_file("${CMAKE_CURRENT_SOURCE_DIR}/pyhdc/__init__.py" "${hdc_BINARY_DIR}/python/pyhdc/__init__.py" COPYONLY)
# TODO move Python tests to ${CMAKE_CURRENT_SOURCE_DIR}
configure_file("${hdc_TEST_DIR}/test_pyhdc.py" "${hdc_BINARY_DIR}/python/test_pyhdc.py" COPYONLY)

configure_file(${SETUP_PY_IN} ${SETUP_PY})
