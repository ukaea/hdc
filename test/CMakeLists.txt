set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_Fortran_MODULE_DIRECTORY "${hdc_BINARY_DIR}/mod")
file(GLOB SRCS *.cpp)
add_executable(hdc_test ${SRCS})
target_link_libraries(hdc_test hdc chdc gtest gmock_main ${Boost_LIBRARIES})
add_test(NAME cpp_test COMMAND hdc_test)

install(TARGETS hdc_test DESTINATION bin)
