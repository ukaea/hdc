# stolen from https://bloerg.net/2012/11/10/cmake-and-distutils.html
set(SETUP_PY_IN "${CMAKE_CURRENT_SOURCE_DIR}/setup.py")
set(SETUP_PY    "${CMAKE_CURRENT_BINARY_DIR}/setup.py")
set(OUTPUT      "${CMAKE_CURRENT_BINARY_DIR}/build/timestamp")

# copy all from python directory
file(COPY "${CMAKE_CURRENT_SOURCE_DIR}" DESTINATION "${hdc_BINARY_DIR}")

# overwrite setup.py with cmake varibles replaced by values
configure_file(${SETUP_PY_IN} ${SETUP_PY})
