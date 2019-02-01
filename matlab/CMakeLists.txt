find_package(Matlab)

configure_file("${CMAKE_CURRENT_SOURCE_DIR}/Makefile.in" "${CMAKE_CURRENT_BINARY_DIR}/Makefile_")

foreach(FILE class_handle.hpp HDC.m hdc_mex.cpp test_matlab.m)
    configure_file("${CMAKE_CURRENT_SOURCE_DIR}/${FILE}" "${CMAKE_CURRENT_BINARY_DIR}/${FILE}")
endforeach()
