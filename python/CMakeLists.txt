# SET(Python_ADDITIONAL_VERSIONS 3 3.6 3.5 3.4 3.3 3.2 3.1 3.0 )
# find_package(PythonInterp 3 REQUIRED)
# find_package(PythonLibs 3 REQUIRED)
# find_package(PythonExtensions 3 REQUIRED)
#find_package(PythonInterp REQUIRED)
#find_package(PythonLibs REQUIRED)
#find_package(PythonExtensions REQUIRED)
# Let us keep cython optional and adapt template if not found
#find_package(Cython)

# stolen from https://bloerg.net/2012/11/10/cmake-and-distutils.html
set(SETUP_PY_IN "${CMAKE_CURRENT_SOURCE_DIR}/setup.py.in")
set(SETUP_PY    "${CMAKE_CURRENT_BINARY_DIR}/setup.py")
set(OUTPUT      "${CMAKE_CURRENT_BINARY_DIR}/build/timestamp")

configure_file("${CMAKE_CURRENT_SOURCE_DIR}/cyhdc.pyx" "${CMAKE_CURRENT_BINARY_DIR}/cyhdc.pyx")

configure_file(${SETUP_PY_IN} ${SETUP_PY})
