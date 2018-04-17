# SET(Python_ADDITIONAL_VERSIONS 3 3.6 3.5 3.4 3.3 3.2 3.1 3.0 )
# find_package(PythonInterp 3 REQUIRED)
# find_package(PythonLibs 3 REQUIRED)
# find_package(PythonExtensions 3 REQUIRED)
find_package(PythonInterp REQUIRED)
find_package(PythonLibs REQUIRED)
find_package(PythonExtensions REQUIRED)
find_package(Cython REQUIRED)

# add_cython_target(cyhdc "CXX")
# add_library(cyhdc MODULE ${cyhdc})
# target_link_libraries(cyhdc hdc ${HDC_LINK_LIBRARIES})
# python_extension_module(cyhdc)
#
# # TODO temporary Python install
# # create Python install package in dist in the future
# install(TARGETS cyhdc RUNTIME DESTINATION bin LIBRARY DESTINATION python)
# add_file_copy_target(pyhdc python pyhdc.py)
# install_file_copy_target(pyhdc DESTINATION python)
# add_file_copy_target(setup_py python setup.py)
# install_file_copy_target(setup_py DESTINATION python)
# add_file_copy_target(cyhdc_pyx python cyhdc.pyx)
# install_file_copy_target(cyhdc_pyx DESTINATION python)


# stolen from https://bloerg.net/2012/11/10/cmake-and-distutils.html
set(SETUP_PY_IN "${CMAKE_CURRENT_SOURCE_DIR}/setup.py.in")
set(SETUP_PY    "${CMAKE_CURRENT_BINARY_DIR}/setup.py")
set(OUTPUT      "${CMAKE_CURRENT_BINARY_DIR}/build/timestamp")

configure_file("${CMAKE_CURRENT_SOURCE_DIR}/cyhdc.pyx" "${CMAKE_CURRENT_BINARY_DIR}/cyhdc.pyx")

configure_file(${SETUP_PY_IN} ${SETUP_PY})

add_custom_command(OUTPUT ${OUTPUT}
                    COMMAND ${PYTHON_EXECUTABLE} ${SETUP_PY} build
                    COMMAND ${CMAKE_COMMAND} -E touch ${OUTPUT}
                    DEPENDS ${DEPS})

add_custom_target(target ALL DEPENDS ${OUTPUT})

# install(CODE "execute_process(COMMAND ${PYTHON_EXECUTABLE} ${SETUP_PY} install --prefix=${CMAKE_INSTALL_PREFIX})")

## TODO https://groups.google.com/forum/#!topic/cython-users/eSuXvK78Z6A
